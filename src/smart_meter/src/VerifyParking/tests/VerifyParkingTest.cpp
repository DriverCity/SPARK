#include <gtest/gtest.h>
#include "VerifyParking.h"
#include "ParkingDatabase/ParkingDatabaseMock.h"
#include "CloudService/CloudServiceMock.h"

namespace
{

void CmpEvents(spark::ParkingEvent expected, spark::ParkingEvent actual)
{
    EXPECT_EQ(expected.registerNumber(), actual.registerNumber());
    EXPECT_EQ(expected.startingTime(), actual.startingTime());
    EXPECT_EQ(expected.duration(), actual.duration());
    EXPECT_EQ(expected.token().verifier(), actual.token().verifier());
    EXPECT_EQ(expected.token().uid(), actual.token().uid());
}

}


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
    m_cloudMock.m_connectionOk = true;
    m_cloudMock.m_verifyResult = spark::ICloudService::OK;
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::OK, result);
    CmpEvents(e, m_cloudMock.m_lastEvent);
}


TEST_F(VerifyParkingTest, VerifyParkingShouldReturnTimeoutFromCloud)
{
    m_verifier->init(&m_cloudMock);
    m_cloudMock.m_connectionOk = true;
    m_cloudMock.m_verifyResult = spark::ICloudService::TIMEOUT;
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::TIMEOUT, result);
    CmpEvents(e, m_cloudMock.m_lastEvent);
}


TEST_F(VerifyParkingTest, VerifyParkingShouldReturnInvalidToken)
{
    m_verifier->init(&m_cloudMock);
    m_cloudMock.m_connectionOk = true;
    m_cloudMock.m_verifyResult = spark::ICloudService::INVALID_TOKEN;
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::INVALID_TOKEN, result);
    CmpEvents(e, m_cloudMock.m_lastEvent);
}


TEST_F(VerifyParkingTest, VerifyParkingShouldReturnOtherError)
{
    m_verifier->init(&m_cloudMock);
    m_cloudMock.m_connectionOk = true;
    m_cloudMock.m_verifyResult = spark::ICloudService::OTHER;
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::OTHER, result);
    CmpEvents(e, m_cloudMock.m_lastEvent);
}


TEST_F(VerifyParkingTest, VerifyParkingNoConnection)
{
    m_verifier->init(&m_cloudMock);
    m_cloudMock.m_connectionOk = false;
    m_cloudMock.m_verifyResult = spark::ICloudService::OK;
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, spark::PaymentToken("ver", "id123"));

    spark::IVerifyParking::Result result = m_verifier->verify(e);

    EXPECT_EQ(spark::IVerifyParking::TIMEOUT, result);
    EXPECT_FALSE(m_cloudMock.m_lastEvent.isValid());
}


TEST_F(VerifyParkingTest, SetCloudService)
{
    m_verifier->setCloudService(&m_cloudMock);
    EXPECT_TRUE(&m_cloudMock == m_verifier->getCloudService());
}
