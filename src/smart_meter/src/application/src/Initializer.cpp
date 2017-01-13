
#include "Initializer.h"
#include "GlobalConfiguration/GlobalConfiguration.h"
#include "Logger/Logger.h"
#include "BLEService/BLEService.h"
#include "BLEService/DemoModeBLEService.h"
#include "PriceProvider/PriceProvider.h"
#include "VerifyParking/VerifyParking.h"
#include "CloudService/CloudService.h"
#include "ParkingDatabase/ParkingDatabase.h"
#include "Logger/Logger.h"

#define PARKING_AREA_ID "parkingAreaId"
#define PRICE_API "priceAPI"
#define EVENT_API "parkingEventAPI"
#define EVENT_DB "eventDb"
#define BLE_INPUT_FIFO "BLEInputFifo"
#define BLE_RESPONSE_FIFO "BLEResponseFifo"
#define DEMO_MODE "demoMode"
#define BINARY_DIR "binaryDir"
#define CONFIG_DIR "configDir"
#define LOG_FILE "logFile"


namespace spark
{


void Initializer::InitializeLogger(const GlobalConfiguration* conf, std::ostream& printOut) const
{
    std::string logFile = conf->getValue(BINARY_DIR) + "/" + conf->getValue(LOG_FILE);
    LOG_DEBUG("Out: " << logFile);
    Logger::init(logFile, printOut);
}


void Initializer::InitializeConfiguration(std::string appPath) const
{
    std::string binDir = appPath.substr(0, appPath.find_last_of('/'));
    std::string confDir = binDir + "/../config";
    GlobalConfiguration::init(confDir + "/global.txt");
    GlobalConfiguration::instance()->insert(BINARY_DIR, binDir);
    GlobalConfiguration::instance()->insert(CONFIG_DIR, confDir);
}


Application::Setup Initializer::createSetup(const GlobalConfiguration* conf) const
{
    Application::Setup setup;
    createCloudService(conf, setup);
    createPriceProvider(conf, setup);
    createParkingDatabase(conf, setup);
    createVerifyParking(setup);
    createBLEService(conf, setup);
    return std::move(setup);
}


void Initializer::createCloudService(const GlobalConfiguration *conf, Application::Setup &setup) const
{
    try
    {
        int parkingAreaId = std::stoi(conf->getValue(PARKING_AREA_ID));

        if (!conf->hasKey(PRICE_API)){
            LOG_ERROR("Missing mandatory parameter: " << PRICE_API);
            throw std::exception();
        }
        std::string priceAPI = conf->getValue(PRICE_API);

        if (!conf->hasKey(EVENT_API)){
            LOG_ERROR("Missing mandatory parameter: " << EVENT_API);
            throw std::exception();
        }
        std::string eventAPI = conf->getValue(EVENT_API);

        setup.cloudService.reset(new spark::CloudService);
        setup.cloudService->init(parkingAreaId, priceAPI, eventAPI);
    }
    catch (std::invalid_argument&){
        LOG_ERROR("Missing mandatory integer parameter: " << PARKING_AREA_ID);
        throw std::exception();
    }
}


void Initializer::createPriceProvider(const GlobalConfiguration *conf, Application::Setup &setup) const
{
    setup.priceProvider.reset(new spark::PriceProvider);
    setup.priceProvider-> init(setup.cloudService.get());
}


void Initializer::createParkingDatabase(const GlobalConfiguration *conf, Application::Setup &setup) const
{
    if (!conf->hasKey(EVENT_DB)){
        LOG_ERROR("Missing mandatory parmeter: " << EVENT_DB);
        throw std::exception();
    }
    std::string dbFile = conf->getValue("binaryDir") + "/" + conf->getValue(EVENT_DB);
    setup.parkingDb.reset(new spark::ParkingDatabase(dbFile));
}


void Initializer::createVerifyParking(Application::Setup &setup) const
{
    setup.verifier.reset(new spark::VerifyParking);
    setup.verifier->init(setup.cloudService.get());
}


void Initializer::createBLEService(const GlobalConfiguration *conf, Application::Setup &setup) const
{
    if (!conf->hasKey(BLE_INPUT_FIFO)){
        LOG_ERROR("Missing mandatory parmeter: " << BLE_INPUT_FIFO);
        throw std::exception();
    }
    else if (!conf->hasKey(BLE_RESPONSE_FIFO)){
        LOG_ERROR("Missing mandatory parmeter: " << BLE_RESPONSE_FIFO);
        throw std::exception();
    }

    if (conf->hasKey(DEMO_MODE) && conf->getValue(DEMO_MODE) != "0"){
        // Demo mode
        setup.bleService.reset(new spark::DemoModeBLEService(conf->getValue(BINARY_DIR) + "/" + conf->getValue(BLE_INPUT_FIFO),
                                                             conf->getValue(BINARY_DIR) + "/" + conf->getValue(BLE_RESPONSE_FIFO),
                                                             conf->getValue(CONFIG_DIR) + "/demoConfig.txt") );
    }
    else {
        // Normal mode
        setup.bleService.reset(new spark::BLEService(conf->getValue(BINARY_DIR) + "/" + conf->getValue(BLE_INPUT_FIFO),
                                                     conf->getValue(BINARY_DIR) + "/" + conf->getValue(BLE_RESPONSE_FIFO)));
    }

    setup.bleService->init(setup.priceProvider.get(), setup.verifier.get());
}


} // spark
