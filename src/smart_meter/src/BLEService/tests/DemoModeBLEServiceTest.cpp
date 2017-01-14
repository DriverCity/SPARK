
#include <gtest/gtest.h>
#include "DemoModeBLEService.h"
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

using namespace spark;
using namespace ::testing;

class DemoModeBLEServiceTest : public Test
{
public:

    DemoModeBLEServiceTest():
        ::testing::Test(), m_responseFd(0)
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


TEST_F(DemoModeBLEServiceTest, PriceRequestNormal)
{
    DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/empty.txt");
    ble.init(&m_priceProvider, &m_verifier);
    PriceInfo expectedPrice(1.2, 60, 1);

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(expectedPrice));
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    ble.start();
    writeInputFifo("price\n");
    sleep(2);
    std::string response = readResponseFifo();
    EXPECT_EQ(expectedPrice.toString()+"\n", response);
}


TEST_F(DemoModeBLEServiceTest, PriceRequestDemo)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/PriceDemo.txt");
    ble.init(&m_priceProvider, &m_verifier);

    EXPECT_CALL(m_priceProvider, getPriceInfo()).Times(0);
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    ble.start();
    writeInputFifo("price\n");
    sleep(2);
    std::string response = readResponseFifo();

    spark::PriceInfo expected(1.8, 0, 10);
    EXPECT_EQ(expected.toString()+"\n", response);
}


TEST_F(DemoModeBLEServiceTest, NoDemoResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/empty.txt");
    ble.init(&m_priceProvider, &m_verifier);
    PriceInfo expectedPrice(1.2, 120, 1);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(expectedPrice));
    EXPECT_CALL(m_verifier, verify(e)).WillOnce(Return(spark::IVerifyParking::OK));
    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("OK\n", response);
}


TEST_F(DemoModeBLEServiceTest, OkDemoResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/OkDemo.txt");
    ble.init(&m_priceProvider, &m_verifier);
    spark::PriceInfo expectedPrice(1.2, 120, 1);
    spark::ParkingEvent act("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("real_service", "real_token"));
    spark::ParkingEvent exp("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).WillOnce(Return(expectedPrice));
    EXPECT_CALL(m_verifier, verify(exp)).WillOnce(Return(spark::IVerifyParking::OK));
    ble.start();
    writeInputFifo(act.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("OK\n", response);
}


TEST_F(DemoModeBLEServiceTest, TimeoutResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/TimeoutDemo.txt");
    ble.init(&m_priceProvider, &m_verifier);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).Times(0);
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Request timed out.\n", response);
}


TEST_F(DemoModeBLEServiceTest, InvalidTokenResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/InvalidTokenDemo.txt");
    ble.init(&m_priceProvider, &m_verifier);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).Times(0);
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Payment token is invalid.\n", response);
}


TEST_F(DemoModeBLEServiceTest, OtherErrorResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/OtherErrorDemo.txt");
    ble.init(&m_priceProvider, &m_verifier);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).Times(0);
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Unknown error.\n", response);
}


TEST_F(DemoModeBLEServiceTest, ForcedOkResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/ForcedOkDemo.txt");
    ble.init(&m_priceProvider, &m_verifier);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "invalid"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).Times(0);
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("OK\n", response);
}


TEST_F(DemoModeBLEServiceTest, UnknownCommand)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/UnknownDemo.txt");
    ble.init(&m_priceProvider, &m_verifier);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).Times(0);
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Unknown command.\n", response);
}


TEST_F(DemoModeBLEServiceTest, ConfigError)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/InvalidDemo.txt");
    ble.init(&m_priceProvider, &m_verifier);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    EXPECT_CALL(m_priceProvider, getPriceInfo()).Times(0);
    EXPECT_CALL(m_verifier, verify(_)).Times(0);
    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Invalid demo configuration.\n", response);
}

