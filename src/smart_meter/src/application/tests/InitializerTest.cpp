
#include <gtest/gtest.h>

#include "../src/Initializer.h"
#include "GlobalConfiguration/GlobalConfiguration.h"
#include "VerifyParking/VerifyParking.h"
#include "PriceProvider/PriceProvider.h"
#include "ParkingDatabase/ParkingDatabase.h"
#include "BLEService/BLEService.h"
#include "BLEService/DemoModeBLEService.h"
#include "Logger/Logger.h"
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <limits.h>
#include <unistd.h>

using namespace spark;

namespace
{

std::string getBinDir()
{
    char result[ PATH_MAX ];
    ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
    std::string fullPath = std::string( result, (count > 0) ? count : 0 );
    return fullPath.substr(0, fullPath.find_last_of('/'));
}

}

class InitializerTest : public ::testing::Test
{
protected:

    void SetUp()
    {
        system("mkdir ../config");
        system("mkfifo ./testInputFifo");
        system("mkfifo ./testResponseFifo");
        system("cp " TEST_DATA_DIR "/dummyConfig.txt ../config/global.txt");
    }

    void TearDown()
    {
        system("rm ./testInputFifo");
        system("rm ./testResponseFifo");
        system("rm -r ../config");
    }
};


TEST_F(InitializerTest, ShouldInitializeConfiguration)
{
    Initializer().InitializeConfiguration(getBinDir() + "/app");
    GlobalConfiguration* conf = GlobalConfiguration::instance();

    ASSERT_TRUE(conf != nullptr);
    EXPECT_EQ(getBinDir(), conf->getValue("binaryDir"));
    EXPECT_EQ(getBinDir() + "/../config", conf->getValue("configDir"));
    EXPECT_EQ("fooValue", conf->getValue("foo"));
}


TEST_F(InitializerTest, ShouldInitializeLogger)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("logFile", "testLog.log");
    LOG_DEBUG(getBinDir());
    conf->insert("binaryDir", getBinDir());
    conf->insert("logFile", "testLog.log");

    std::ostringstream oss;
    Initializer().InitializeLogger(conf, oss);

    LOG_DEBUG("Hello");
    Logger::close();

    std::string msg = oss.str();
    EXPECT_EQ("Hello\n", msg.substr(msg.length()-6));

    std::ifstream ifs(getBinDir() + "/testLog.log");
    EXPECT_TRUE((bool)ifs);
    std::string result;
    std::string tmp;
    while(std::getline(ifs, tmp)){
        result += tmp + "\n";
    }
    ifs.close();
    system("rm ./testLog.log");
    EXPECT_EQ("Hello\n", result.substr(result.size()-6));
}

TEST_F(InitializerTest, ShouldCreateAndInitializeAllComponents)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("BLEInputFifo", "testInputFifo");
    conf->insert("BLEResponseFifo", "testResponseFifo");
    conf->insert("eventDb", "testDb.db");
    conf->insert("priceAPI", "http://priceapi.com/");
    conf->insert("parkingEventAPI", "http://eventapi.com/");
    conf->insert("parkingAreaId", "1234");
    conf->insert("configDir", ".");
    conf->insert("binaryDir", ".");

    Application::Setup setup = Initializer().createSetup(conf);

    ASSERT_TRUE(setup.bleService != nullptr);
    ASSERT_TRUE(setup.cloudService != nullptr);
    ASSERT_TRUE(setup.parkingDb != nullptr);
    ASSERT_TRUE(setup.priceProvider != nullptr);
    ASSERT_TRUE(setup.verifier != nullptr);

    VerifyParking* verifier = dynamic_cast<VerifyParking*>(setup.verifier.get());
    ASSERT_TRUE(verifier != nullptr);
    EXPECT_TRUE(verifier->getCloudService() == setup.cloudService.get());

    PriceProvider* priceProvider = dynamic_cast<PriceProvider*>(setup.priceProvider.get());
    ASSERT_TRUE(priceProvider != nullptr);
    EXPECT_TRUE(priceProvider->getCloudService() == setup.cloudService.get());

    ParkingDatabase* db = dynamic_cast<ParkingDatabase*>(setup.parkingDb.get());
    ASSERT_TRUE(db != nullptr);
    EXPECT_TRUE(db->isValid());

    BLEService* ble = dynamic_cast<BLEService*>(setup.bleService.get());
    EXPECT_TRUE(ble != nullptr);
    DemoModeBLEService* demo_ble = dynamic_cast<DemoModeBLEService*>(setup.bleService.get());
    EXPECT_TRUE(demo_ble == nullptr);
}


TEST_F(InitializerTest, ShouldCreateAndInitializeAllComponentsInNormalMode)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("BLEInputFifo", "testInputFifo");
    conf->insert("BLEResponseFifo", "testResponseFifo");
    conf->insert("eventDb", "testDb.db");
    conf->insert("priceAPI", "http://priceapi.com/");
    conf->insert("parkingEventAPI", "http://eventapi.com/");
    conf->insert("parkingAreaId", "1234");
    conf->insert("demoMode", "0");
    conf->insert("configDir", ".");
    conf->insert("binaryDir", ".");

    Application::Setup setup = Initializer().createSetup(conf);

    ASSERT_TRUE(setup.bleService != nullptr);
    ASSERT_TRUE(setup.cloudService != nullptr);
    ASSERT_TRUE(setup.parkingDb != nullptr);
    ASSERT_TRUE(setup.priceProvider != nullptr);
    ASSERT_TRUE(setup.verifier != nullptr);

    VerifyParking* verifier = dynamic_cast<VerifyParking*>(setup.verifier.get());
    ASSERT_TRUE(verifier != nullptr);
    EXPECT_TRUE(verifier->getCloudService() == setup.cloudService.get());

    PriceProvider* priceProvider = dynamic_cast<PriceProvider*>(setup.priceProvider.get());
    ASSERT_TRUE(priceProvider != nullptr);
    EXPECT_TRUE(priceProvider->getCloudService() == setup.cloudService.get());

    ParkingDatabase* db = dynamic_cast<ParkingDatabase*>(setup.parkingDb.get());
    ASSERT_TRUE(db != nullptr);
    EXPECT_TRUE(db->isValid());

    BLEService* ble = dynamic_cast<BLEService*>(setup.bleService.get());
    EXPECT_TRUE(ble != nullptr);
    DemoModeBLEService* demo_ble = dynamic_cast<DemoModeBLEService*>(setup.bleService.get());
    EXPECT_TRUE(demo_ble == nullptr);
}


TEST_F(InitializerTest, ShouldInitializeWithDemoBLEService)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("BLEInputFifo", "testInputFifo");
    conf->insert("BLEResponseFifo", "testResponseFifo");
    conf->insert("eventDb", "testDb.db");
    conf->insert("priceAPI", "http://priceapi.com/");
    conf->insert("parkingEventAPI", "http://eventapi.com/");
    conf->insert("parkingAreaId", "1234");
    conf->insert("demoMode", "1");
    conf->insert("configDir", ".");
    conf->insert("binaryDir", ".");

    Application::Setup setup = Initializer().createSetup(conf);

    ASSERT_TRUE(setup.bleService != nullptr);
    ASSERT_TRUE(setup.cloudService != nullptr);
    ASSERT_TRUE(setup.parkingDb != nullptr);
    ASSERT_TRUE(setup.priceProvider != nullptr);
    ASSERT_TRUE(setup.verifier != nullptr);

    VerifyParking* verifier = dynamic_cast<VerifyParking*>(setup.verifier.get());
    ASSERT_TRUE(verifier != nullptr);
    EXPECT_TRUE(verifier->getCloudService() == setup.cloudService.get());

    PriceProvider* priceProvider = dynamic_cast<PriceProvider*>(setup.priceProvider.get());
    ASSERT_TRUE(priceProvider != nullptr);
    EXPECT_TRUE(priceProvider->getCloudService() == setup.cloudService.get());

    ParkingDatabase* db = dynamic_cast<ParkingDatabase*>(setup.parkingDb.get());
    ASSERT_TRUE(db != nullptr);
    EXPECT_TRUE(db->isValid());

    DemoModeBLEService* ble = dynamic_cast<DemoModeBLEService*>(setup.bleService.get());
    EXPECT_TRUE(ble != nullptr);
}


TEST_F(InitializerTest, ShouldFailIfBLEInputFifoIsMissing)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("BLEResponseFifo", "testResponseFifo");
    conf->insert("eventDb", "testDb.db");
    conf->insert("priceAPI", "http://priceapi.com/");
    conf->insert("parkingEventAPI", "http://eventapi.com/");
    conf->insert("parkingAreaId", "1234");
    conf->insert("demoMode", "1");
    conf->insert("configDir", ".");
    conf->insert("binaryDir", ".");

    EXPECT_THROW(Initializer().createSetup(conf), std::exception);
}


TEST_F(InitializerTest, ShouldFailIfBLEResponseFifoIsMissing)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("BLEInputFifo", "testResponseFifo");
    conf->insert("eventDb", "testDb.db");
    conf->insert("priceAPI", "http://priceapi.com/");
    conf->insert("parkingEventAPI", "http://eventapi.com/");
    conf->insert("parkingAreaId", "1234");
    conf->insert("demoMode", "1");
    conf->insert("configDir", ".");
    conf->insert("binaryDir", ".");

    EXPECT_THROW(Initializer().createSetup(conf), std::exception);
}


TEST_F(InitializerTest, ShouldFailIfDatabaseFileIsMissing)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("BLEInputFifo", "testInputFifo");
    conf->insert("BLEResponseFifo", "testResponseFifo");
    conf->insert("priceAPI", "http://priceapi.com/");
    conf->insert("parkingEventAPI", "http://eventapi.com/");
    conf->insert("parkingAreaId", "1234");
    conf->insert("demoMode", "1");
    conf->insert("configDir", ".");
    conf->insert("binaryDir", ".");

    EXPECT_THROW(Initializer().createSetup(conf), std::exception);
}


TEST_F(InitializerTest, ShouldFailIfPriceApiIsMissing)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("BLEInputFifo", "testInputFifo");
    conf->insert("BLEResponseFifo", "testResponseFifo");
    conf->insert("eventDb", "testDb.db");
    conf->insert("parkingEventAPI", "http://eventapi.com/");
    conf->insert("parkingAreaId", "1234");
    conf->insert("configDir", ".");
    conf->insert("binaryDir", ".");

    EXPECT_THROW(Initializer().createSetup(conf), std::exception);
}


TEST_F(InitializerTest, ShouldFailIfEventApiIsMissing)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("BLEInputFifo", "testInputFifo");
    conf->insert("BLEResponseFifo", "testResponseFifo");
    conf->insert("eventDb", "testDb.db");
    conf->insert("priceAPI", "http://priceapi.com/");
    conf->insert("parkingAreaId", "1234");
    conf->insert("configDir", ".");
    conf->insert("binaryDir", ".");

    EXPECT_THROW(Initializer().createSetup(conf), std::exception);
}


TEST_F(InitializerTest, ShouldFailIfAreaIdIsMissing)
{
    GlobalConfiguration::init();
    GlobalConfiguration* conf = GlobalConfiguration::instance();
    conf->insert("BLEInputFifo", "testInputFifo");
    conf->insert("BLEResponseFifo", "testResponseFifo");
    conf->insert("eventDb", "testDb.db");
    conf->insert("priceAPI", "http://priceapi.com/");
    conf->insert("parkingEventAPI", "http://eventapi.com/");
    conf->insert("configDir", ".");
    conf->insert("binaryDir", ".");

    EXPECT_THROW(Initializer().createSetup(conf), std::exception);
}
