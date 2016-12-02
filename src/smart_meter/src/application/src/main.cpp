
#include "GlobalConfiguration/GlobalConfiguration.h"
#include "Logger/Logger.h"
#include "BLEService/BLEService.h"
#include "PriceProvider/PriceProviderMock.h"
#include "VerifyParking/VerifyParkingMock.h"
#include "CloudService/CloudServiceMock.h"
#include "Application.h"
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
    setup.cloudService.reset(new sparktest::CloudServiceMock);
    setup.priceProvider.reset(new sparktest::PriceProviderMock);
    setup.verifier.reset(new sparktest::VerifyParkingMock);
    std::string bleInputFifo = config->getValue("BLEInputFifo");
    std::string bleResponseFifo = config->getValue("BLEResponseFifo");
    setup.bleService.reset(new spark::BLEService(binaryDir + "/" + bleInputFifo,
                                                 binaryDir + "/" + bleResponseFifo));

    // Initialize components.
    setup.bleService->init(setup.priceProvider.get(), setup.verifier.get());

    // Hard-coded mockup-price
    ((sparktest::PriceProviderMock*)(setup.priceProvider.get()))->m_info = spark::PriceInfo(1.5, 0, 30);

    spark::Application app(std::move(setup));
    return app.exec();
}
