
#include "BLEService.h"
#include <cassert>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

#include "Logger/Logger.h"
#include "misc/ParkingEvent.h"


namespace
{

const std::string GET_PRICE_MESSAGE = "price";

}

namespace spark
{


BLEService::BLEService(const std::string& inputFifo, const std::string& responseFifo) :
    IBLEService(), m_priceProvider(nullptr), m_verifier(nullptr),
    m_inputFifo(inputFifo), m_responseFifo(responseFifo),
    m_serviceThread(), m_stopService(false)
{
    LOG_DEBUG("Input fifo: " << m_inputFifo);
    LOG_DEBUG("Response fifo: " << m_responseFifo);
}


BLEService::~BLEService()
{
    stop();
}


void BLEService::init(IPriceProvider *priceProvider, IVerifyParking *verifier)
{
    assert(priceProvider != nullptr);
    assert(verifier != nullptr);
    m_priceProvider = priceProvider;
    m_verifier = verifier;
}


void BLEService::start()
{
    stop();
    m_stopService.store(false);
    m_serviceThread = std::thread(&BLEService::startServiceThread, this);
}


void BLEService::stop()
{
    m_stopService.store(true);
    if (m_serviceThread.joinable()){
        m_serviceThread.join();
    }
}


IPriceProvider* BLEService::priceProvider() const
{
    return m_priceProvider;
}


IVerifyParking* BLEService::verifier() const
{
    return m_verifier;
}


void BLEService::startServiceThread()
{
    LOG_DEBUG("BLEService has started!");

    while (true)
    {
        // Check if quiting is requested.
        if (m_stopService.load()){
            break;
        }

        fd_set set;
        int filedesc = open( m_inputFifo.c_str(), O_NONBLOCK, O_RDONLY );

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        FD_ZERO(&set); /* clear the set */
        FD_SET(filedesc, &set); /* add our file descriptor to the set */

        LOG_DEBUG("Waiting for BLE message...");
        char buff[100];
        int len = 100;
        int rv = select(filedesc + 1, &set, NULL, NULL, &timeout);
        if(rv == -1){
            LOG_ERROR("Error reading input fifo");
        }
        else if(rv == 0){
            LOG_DEBUG("Timeout");
        }
        else{
            int count = read( filedesc, buff, len ); /* there was data to read */
            LOG_DEBUG("Count " << count);
            if (count != 0){
                handleMessage(std::string(buff, count-1)); // Omit newline.
                LOG_DEBUG("Message handled!");
            }
        }

        close(filedesc);
    }

    LOG_DEBUG("BLEService has quited!");
}


void BLEService::handleMessage(const std::string& msg)
{
    LOG_DEBUG("Received message: " << msg);

    if (msg == GET_PRICE_MESSAGE){
        PriceInfo info = m_priceProvider->getPriceInfo();
        priceInfoRequest(info);
        return;
    }

    ParkingEvent event = ParkingEvent::fromString(msg);
    if (event.isValid()){
        PriceInfo info = m_priceProvider->getPriceInfo();
        verifyRequest(info, event);
    }

    else {
        LOG_ERROR("Unknown command: " << msg);
        sendResponse("Error: Unknown command.");
    }
}


void BLEService::sendResponse(std::string msg)
{
    msg += "\n\0";
    int filedesc = open( m_responseFifo.c_str(), O_WRONLY );
    write(filedesc, msg.c_str(), msg.length()+1);
    close(filedesc);
}


void BLEService::priceInfoRequest(const PriceInfo& info)
{
    LOG_DEBUG("Requesting price information...");
    if (info.isValid()){
        LOG_DEBUG("Service is available! Price: " << info.toString());
        sendResponse(info.toString());
    }
    else {
        LOG_ERROR("Service is not available");
        sendResponse("Error: Service is not available.");
    }
}


void BLEService::verifyRequest(const PriceInfo& info, const ParkingEvent& event)
{
    LOG_DEBUG("Registering parking...");
    if (event.duration() % info.timeResolution() != 0){
        LOG_ERROR("Duration is not multiply of resolution");
        sendResponse("Error: Invalid parking duration.");
    }
    else if (info.timeLimit() != 0 && event.duration() > info.timeLimit()){
        LOG_ERROR("Duration exceeds time limit.");
        sendResponse("Error: Duration exceeds time limit.");
    }
    else {
        IVerifyParking::Result result = m_verifier->verify(event);
        if (result == IVerifyParking::OK){
            LOG_DEBUG("Registering parking succeeded.");
            sendResponse("OK");
        }
        else if (result == IVerifyParking::TIMEOUT){
            LOG_ERROR("Timeout");
            sendResponse("Error: Request timed out.");
        }
        else if (result == IVerifyParking::INVALID_TOKEN){
            LOG_ERROR("Invalid token.");
            sendResponse("Error: Payment token is invalid.");
        }
        else {
            LOG_ERROR("Other error.");
            sendResponse("Error: Unknown error.");
        }
    }
}


} // spark
