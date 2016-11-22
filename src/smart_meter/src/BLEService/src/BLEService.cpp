
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


namespace
{

const std::string GET_PRICE_MESSAGE = "price\n";

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
            read( filedesc, buff, len ); /* there was data to read */
            handleMessage(buff);
            LOG_DEBUG("Message handled!");
        }

        close(filedesc);
    }

    LOG_DEBUG("BLEService has quited!");
}


void BLEService::handleMessage(const std::string& msg)
{
    LOG_DEBUG("Received message: " << msg);
    if (msg == GET_PRICE_MESSAGE){
        LOG_DEBUG("Requesting price information...");
        spark::PriceInfo info = m_priceProvider->getPriceInfo();
        if (info.isValid()){
            LOG_DEBUG("Service is available! Price: " << info.toString());
            sendResponse(info.toString());
        }
        else {
            LOG_ERROR("Service is not available");
            sendResponse("Error: Service is not available.");
        }
    }
    else {
        LOG_ERROR("Unknown command: " << msg);
        sendResponse("Error: unknown command");
    }
}


void BLEService::sendResponse(std::string msg)
{
    msg += "\0";
    int filedesc = open( m_responseFifo.c_str(), O_WRONLY );
    write(filedesc, msg.c_str(), msg.length()+1);
}

} // spark
