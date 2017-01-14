#include <gtest/gtest.h>
#include "BLEService.h"
#include "Logger/Logger.h"
#include "PriceProvider/PriceProviderMock.h"
#include "VerifyParking/VerifyParkingMock.h"

#include <cstdlib>
#include <memory>
#include <fstream>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>

#define INPUT_FIFO "testInputFifo"
#define RESPONSE_FIFO "testResponseFifo"

using namespace ::testing;

class BLEServiceTest : public ::testing::Test
{
public:

    BLEServiceTest():
        ::testing::Test(),
        m_service(nullptr)
    {
    }


protected:

    std::shared_ptr<spark::BLEService> m_service;
    sparktest::PriceProviderMock m_priceProvider;
    sparktest::VerifyParkingMock m_verifier;
    int m_responseFd;

    void SetUp()
    {
        // Create fifos.
        system("mkfifo " INPUT_FIFO);
        std::ofstream ofs(RESPONSE_FIFO);
        ofs << "";
        ofs.close();

        spark::Logger::init();
        m_service.reset(new spark::BLEService(INPUT_FIFO, RESPONSE_FIFO));
        m_responseFd = open(RESPONSE_FIFO, O_NONBLOCK, O_RDONLY);

    }

    void TearDown()
    {
        m_service.reset();
        spark::Logger::close();
        close(m_responseFd);
        system("rm " INPUT_FIFO);
        system("rm " RESPONSE_FIFO);
    }


    void writeInputFifo(const std::string& str)
    {
        FILE* fp = fopen(INPUT_FIFO, "w");
        fputs(str.c_str(), fp);
        fclose(fp);
    }


    std::string readResponseFifo()
    {
        char buff[100];
        read(m_responseFd, buff, 100);
        return std::string(buff);
    }

};



TEST_F(BLEServiceTest, ConstructorTest)
{
    EXPECT_TRUE(m_service->priceProvider() == nullptr);
    EXPECT_TRUE(m_service->verifier() == nullptr);
}


TEST_F(BLEServiceTest, InitTest)
{
    m_service->init(&m_priceProvider, &m_verifier);

    EXPECT_TRUE(m_service->priceProvider() == &m_priceProvider);
    EXPECT_TRUE(m_service->verifier() == &m_verifier);
}


TEST_F(BLEServiceTest, GetPriceSuccess)
{
    m_service->init(&m_priceProvider, &m_verifier);
    spark::PriceInfo price(1.2, 0, 10);

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(price));
    m_service->start();
    writeInputFifo("price\n");
    sleep(1); // Give service time to respond.
    EXPECT_EQ(price.toString()+"\n", readResponseFifo());
}


TEST_F(BLEServiceTest, GetPriceFailure)
{
    m_service->init(&m_priceProvider, &m_verifier);

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(spark::PriceInfo()));
    m_service->start();
    writeInputFifo("price\n");
    sleep(1); // Give service time to respond.
    EXPECT_EQ("Error: Service is not available.\n", readResponseFifo());
}


TEST_F (BLEServiceTest, UnknownCommand)
{
    EXPECT_CALL(m_priceProvider, getPriceInfo()).Times(0);
    m_service->init(&m_priceProvider, &m_verifier);
    m_service->start();
    writeInputFifo("NotACommand");
    sleep(1); // Give service time to respond.
    EXPECT_EQ("Error: Unknown command.\n", readResponseFifo());
}


TEST_F (BLEServiceTest, RegisterSuccess)
{
    m_service->init(&m_priceProvider, &m_verifier);
    spark::PriceInfo price(1.2, 0, 10);
    spark::ParkingEvent e("ABC123", "2016-11-23 12:30", 30, spark::PaymentToken("ver", "123"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(price));
    EXPECT_CALL(m_verifier, verify(e)).WillOnce(Return(spark::IVerifyParking::OK));
    m_service->start();
    writeInputFifo(e.toString() + "\n");
    sleep(1);
    EXPECT_EQ("OK\n", readResponseFifo());
}


TEST_F (BLEServiceTest, RegisterFailureTimeout)
{
    m_service->init(&m_priceProvider, &m_verifier);
    spark::PriceInfo price(1.2, 0, 10);
    spark::ParkingEvent e("ABC123", "2016-11-23 12:30", 30, spark::PaymentToken("ver", "123"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(price));
    EXPECT_CALL(m_verifier, verify(e)).WillOnce(Return(spark::IVerifyParking::TIMEOUT));
    m_service->start();

    writeInputFifo(e.toString() + "\n");
    sleep(1);
    EXPECT_EQ("Error: Request timed out.\n", readResponseFifo());
}


TEST_F (BLEServiceTest, RegisterFailureInvalidToken)
{
    m_service->init(&m_priceProvider, &m_verifier);
    spark::PriceInfo price(1.2, 0, 10);
    spark::ParkingEvent e("ABC123", "2016-11-23 12:30", 30, spark::PaymentToken("ver", "123"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(price));
    EXPECT_CALL(m_verifier, verify(e)).WillOnce(Return(spark::IVerifyParking::INVALID_TOKEN));
    m_service->start();

    writeInputFifo(e.toString() + "\n");
    sleep(1);
    EXPECT_EQ("Error: Payment token is invalid.\n", readResponseFifo());
}


TEST_F (BLEServiceTest, RegisterFailureOtherError)
{
    m_service->init(&m_priceProvider, &m_verifier);
    spark::PriceInfo price(1.2, 0, 10);
    spark::ParkingEvent e("ABC123", "2016-11-23 12:30", 30, spark::PaymentToken("ver", "123"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(price));
    EXPECT_CALL(m_verifier, verify(e)).WillOnce(Return(spark::IVerifyParking::OTHER));
    m_service->start();
    writeInputFifo(e.toString() + "\n");
    sleep(1);
    EXPECT_EQ("Error: Unknown error.\n", readResponseFifo());
}


TEST_F (BLEServiceTest, ParkingTimeExceedsTheLimit)
{
    m_service->init(&m_priceProvider, &m_verifier);
    spark::PriceInfo price(1.2, 120, 10);
    spark::ParkingEvent e("ABC123", "2016-11-23 12:30", 200, spark::PaymentToken("ver", "123"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(price));
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    m_service->start();
    writeInputFifo(e.toString() + "\n");
    sleep(1);
    EXPECT_EQ("Error: Duration exceeds time limit.\n", readResponseFifo());
}


TEST_F (BLEServiceTest, ParkingTimeNotAMultipleOfResolution)
{
    m_service->init(&m_priceProvider, &m_verifier);
    spark::PriceInfo price(1.2, 120, 10);

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(price));
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    m_service->start();
    spark::ParkingEvent e("ABC123", "2016-11-23 12:30", 15, spark::PaymentToken("ver", "123"));
    writeInputFifo(e.toString() + "\n");
    sleep(1);
    EXPECT_EQ("Error: Invalid parking duration.\n", readResponseFifo());
}
