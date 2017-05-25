
#include <gtest/gtest.h>
#include "../src/Application.h"
#include "BLEService/BLEServiceMock.h"
#include "VerifyParking/VerifyParkingMock.h"
#include "PriceProvider/PriceProviderMock.h"
#include "CloudService/CloudServiceMock.h"
#include "ParkingDatabase/ParkingDatabaseMock.h"
#include <future>
#include <csignal>

using namespace spark;
using namespace sparktest;

class ApplicationTest : public ::testing::Test
{
public:

    ApplicationTest() :
        m_bleServiceMock(),
        m_cloudServiceMock(),
        m_verifyParkingMock(),
        m_priceProviderMock(),
        m_parkingDbMock(),
        m_app()
    {
    }


protected:

    std::shared_ptr<BLEServiceMock> m_bleServiceMock;
    std::shared_ptr<CloudServiceMock> m_cloudServiceMock;
    std::shared_ptr<VerifyParkingMock> m_verifyParkingMock;
    std::shared_ptr<PriceProviderMock> m_priceProviderMock;
    std::shared_ptr<ParkingDatabaseMock> m_parkingDbMock;
    std::shared_ptr<Application> m_app;

    void SetUp()
    {
        Application::Setup setup;
        m_bleServiceMock.reset(new BLEServiceMock);
        setup.bleService = m_bleServiceMock;
        m_cloudServiceMock.reset(new CloudServiceMock);
        setup.cloudService = m_cloudServiceMock;
        m_parkingDbMock.reset(new ParkingDatabaseMock);
        setup.parkingDb = m_parkingDbMock;
        m_priceProviderMock.reset(new PriceProviderMock);
        setup.priceProvider = m_priceProviderMock;
        m_verifyParkingMock.reset(new VerifyParkingMock);
        setup.verifier = m_verifyParkingMock;

        EXPECT_CALL(*m_bleServiceMock, init(testing::_, testing::_)).Times(0);
        EXPECT_CALL(*m_bleServiceMock, start()).Times(0);
        EXPECT_CALL(*m_bleServiceMock, stop()).Times(0);

        m_app.reset(new Application(setup));
    }

    void TearDown()
    {
    }

};



TEST_F (ApplicationTest, ShouldInvokeBLEServiceStartAndStop)
{
    {
        testing::InSequence dummy;
        EXPECT_CALL(*m_bleServiceMock, start()).Times(1);
        EXPECT_CALL(*m_bleServiceMock, stop()).Times(1);
    }
    EXPECT_CALL(*m_bleServiceMock, init(testing::_, testing::_)).Times(0);


    std::future<int> rval = std::async(&Application::exec, m_app.get());
    m_app->stop();
    EXPECT_EQ(0, rval.get());
}
