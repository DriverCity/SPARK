
#include "DemoModeBLEService.h"
#include "Logger/Logger.h"

namespace spark
{


DemoModeBLEService::DemoModeBLEService(const std::string &inputFifo,
                                       const std::string &responseFifo,
                                       const std::string& demoConfFile) :

    BLEService(inputFifo, responseFifo), m_demoConf(), m_step(0)
{
    m_demoConf = ConfigurationReader().readFile(demoConfFile);
    LOG_DEBUG("Created BLEService in demo mode.");
}


DemoModeBLEService::~DemoModeBLEService()
{
}


void DemoModeBLEService::handleMessage(const std::string &msg)
{
    std::string step_str = std::to_string(m_step);

    if (msg == "price"){
        BLEService::handleMessage(msg);
    }
    else if (m_demoConf.find(step_str) == m_demoConf.end()){
        BLEService::handleMessage(msg);
        ++m_step;
    }
    else {

        try
        {
            int demoValue = std::stoi(m_demoConf[step_str]);
            hanleDemoValue(demoValue, msg);
        }
        catch (std::invalid_argument&){
            LOG_ERROR("Could not convert demo response value: " << m_demoConf[step_str]);
            sendResponse("Error: Invalid demo configuration.");
        }

        ++m_step;
    }
}


void DemoModeBLEService::hanleDemoValue(int demoValue, const std::string& msg)
{
    if (demoValue == (int)DemoResult::OK){
        // Replace actual token with valid test token.
        LOG_DEBUG("Ok demo");
        ParkingEvent orig = ParkingEvent::fromString(msg);
        ParkingEvent newEvent = ParkingEvent(orig.registerNumber(), orig.startingTime(), orig.duration(),
                                             PaymentToken("SERVICE_1", "valid_test_hash"));
        BLEService::handleMessage(newEvent.toString());
    }
    else if (demoValue == (int)DemoResult::TIMEOUT){
        LOG_DEBUG("Timeout demo");
        sendResponse("Error: Request timed out.");
    }
    else if (demoValue == (int)DemoResult::INVALID_TOKEN){
        LOG_DEBUG("Invalid token demo");
        sendResponse("Error: Payment token is invalid.");
    }
    else if (demoValue == (int)DemoResult::OTHER){
        LOG_DEBUG("Other error demo");
        sendResponse("Error: Unknown error.");
    }
    else {
        LOG_DEBUG("Unknown command demo");
        sendResponse(("Error: Unknown command."));
    }
}


} // spark
