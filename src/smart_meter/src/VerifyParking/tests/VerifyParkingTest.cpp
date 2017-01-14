#include <gtest/gtest.h>
#include "VerifyParking.h"
#include "ParkingDatabase/ParkingDatabaseMock.h"
#include "CloudService/CloudServiceMock.h"

using namespace ::testing;


class VerifyParkingTest : public ::testing::Test
{
protected:

    void SetUp()
    {
        m_verifier = new spark::VerifyParking();
    }

    void TearDown()
    {
        delete m_verifier;
    }

    sparktest::CloudServiceMock m_cloudMock;
    spark::VerifyParking* m_verifier;
};



TEST_F(VerifyParkingTest, constructorTest)
{
    EXPECT_EQ(nullptr, m_verifier->getCloudService());
}


TEST_F(VerifyParkingTest, initTest)
{
    m_verifier->init(&m_cloudMock);
    EXPECT_TRUE(m_verifier->getCloudService() == &m_cloudMock);
}


TEST_F(VerifyParkingTest, VerifyParkingShouldReturnOk)
{
    m_verifier->init(&m_cloudMock);
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    EXPECT_CALL(m_cloudMock, checkConnection()).WillOnce(Return(true));
    EXPECT_CALL(m_cloudMock, verifyParkingEvent(e)).WillOnce(Return(spark::ICloudService::OK));
    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::OK, result);
}


TEST_F(VerifyParkingTest, VerifyParkingShouldReturnTimeoutFromCloud)
{
    m_verifier->init(&m_cloudMock);
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    EXPECT_CALL(m_cloudMock, checkConnection()).WillOnce(Return(true));
    EXPECT_CALL(m_cloudMock, verifyParkingEvent(e)).WillOnce(Return(spark::ICloudService::TIMEOUT));
    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::TIMEOUT, result);
}


TEST_F(VerifyParkingTest, VerifyParkingShouldReturnInvalidToken)
{
    m_verifier->init(&m_cloudMock);
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    EXPECT_CALL(m_cloudMock, checkConnection()).WillOnce(Return(true));
    EXPECT_CALL(m_cloudMock, verifyParkingEvent(e)).WillOnce(Return(spark::ICloudService::INVALID_TOKEN));
    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::INVALID_TOKEN, result);
}


TEST_F(VerifyParkingTest, VerifyParkingShouldReturnOtherError)
{
    m_verifier->init(&m_cloudMock);
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    EXPECT_CALL(m_cloudMock, checkConnection()).WillOnce(Return(true));
    EXPECT_CALL(m_cloudMock, verifyParkingEvent(e)).WillOnce(Return(spark::ICloudService::OTHER));
    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::OTHER, result);
}


TEST_F(VerifyParkingTest, VerifyParkingNoConnection)
{
    m_verifier->init(&m_cloudMock);
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    EXPECT_CALL(m_cloudMock, checkConnection()).WillOnce(Return(false));
    EXPECT_CALL(m_cloudMock, verifyParkingEvent(_)).Times(0);
    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::TIMEOUT, result);
}


TEST_F(VerifyParkingTest, SetCloudService)
{
    m_verifier->setCloudService(&m_cloudMock);
    EXPECT_TRUE(&m_cloudMock == m_verifier->getCloudService());
}
