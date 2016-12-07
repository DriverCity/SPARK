
#include "GlobalConfiguration/GlobalConfiguration.h"
#include "Logger/Logger.h"
#include "BLEService/BLEService.h"
#include "PriceProvider/PriceProvider.h"
#include "VerifyParking/VerifyParking.h"
#include "CloudService/CloudService.h"
#include "Application.h"
#include "ParkingDatabase/ParkingDatabase.h"
#include <string>


int main(int argc, char** argv)
{
    // Set up configuration.
    std::string binaryDir = spark::Application::getBinaryDir(argv[0]);
    std::string confDir = spark::Application::getConfigDir(argv[0]);
    spark::GlobalConfiguration::init(confDir + "/global.txt");
    spark::GlobalConfiguration* config = spark::GlobalConfiguration::instance();
    config->insert("configDir", confDir);
    config->insert("binaryDir", binaryDir);
    spark::Logger::init( binaryDir + "/" + config->getValue("logFile"));

    // Instantiate components.
    spark::Application::Setup setup;
    setup.cloudService.reset(new spark::CloudService);
    setup.priceProvider.reset(new spark::PriceProvider);
    setup.parkingDb.reset(new spark::ParkingDatabase("Parking.db"));
    setup.priceProvider-> init(setup.cloudService.get());
    setup.verifier.reset(new spark::VerifyParking);
    setup.verifier->init(setup.cloudService.get(), setup.parkingDb.get());

    std::string bleInputFifo = config->getValue("BLEInputFifo");
    std::string bleResponseFifo = config->getValue("BLEResponseFifo");
    setup.bleService.reset(new spark::BLEService(binaryDir + "/" + bleInputFifo,
                                                 binaryDir + "/" + bleResponseFifo));

    // Initialize components.
    setup.bleService->init(setup.priceProvider.get(), setup.verifier.get());


    spark::Application app(std::move(setup));
    return app.exec();
}
