
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


class DemoModeBLEServiceTest : public ::testing::Test
{
public:

    DemoModeBLEServiceTest():
        ::testing::Test(),
        m_priceProvider(new sparktest::PriceProviderMock()),
        m_verifier(new sparktest::VerifyParkingMock()), m_responseFd(0)
    {
    }

protected:

    std::shared_ptr<spark::BLEService> m_service;
    std::shared_ptr<sparktest::PriceProviderMock> m_priceProvider;
    std::shared_ptr<sparktest::VerifyParkingMock> m_verifier;
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
        m_priceProvider->reset();
        m_verifier->reset();
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
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/empty.txt");
    ble.init(m_priceProvider.get(), m_verifier.get());
    m_priceProvider->m_info = spark::PriceInfo(1.2, 60, 1);

    ble.start();
    writeInputFifo("price\n");
    sleep(2);
    std::string response = readResponseFifo();
    EXPECT_EQ(m_priceProvider->m_info.toString()+"\n", response);
}


TEST_F(DemoModeBLEServiceTest, PriceRequestDemo)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/PriceDemo.txt");
    ble.init(m_priceProvider.get(), m_verifier.get());
    m_priceProvider->m_info = spark::PriceInfo(1.2, 60, 1);

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
    ble.init(m_priceProvider.get(), m_verifier.get());
    m_verifier->m_result = spark::IVerifyParking::OK;
    m_priceProvider->m_info = spark::PriceInfo(1.2, 120, 1);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("OK\n", response);
    EXPECT_EQ(e.registerNumber(), m_verifier->m_lastEvent.registerNumber());
    EXPECT_EQ(e.startingTime(), m_verifier->m_lastEvent.startingTime());
    EXPECT_EQ(e.duration(), m_verifier->m_lastEvent.duration());
    EXPECT_EQ(e.token().uid(), m_verifier->m_lastEvent.token().uid());
    EXPECT_EQ(e.token().verifier(), m_verifier->m_lastEvent.token().verifier());
}


TEST_F(DemoModeBLEServiceTest, OkDemoResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/OkDemo.txt");
    ble.init(m_priceProvider.get(), m_verifier.get());
    m_verifier->m_result = spark::IVerifyParking::OK;
    m_priceProvider->m_info = spark::PriceInfo(1.2, 120, 1);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("real_service", "real_token"));

    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("OK\n", response);
    EXPECT_EQ(e.registerNumber(), m_verifier->m_lastEvent.registerNumber());
    EXPECT_EQ(e.startingTime(), m_verifier->m_lastEvent.startingTime());
    EXPECT_EQ(e.duration(), m_verifier->m_lastEvent.duration());
    EXPECT_EQ("valid_test_hash", m_verifier->m_lastEvent.token().uid());
    EXPECT_EQ("SERVICE_1", m_verifier->m_lastEvent.token().verifier());
}


TEST_F(DemoModeBLEServiceTest, TimeoutResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/TimeoutDemo.txt");
    ble.init(m_priceProvider.get(), m_verifier.get());
    m_verifier->m_result = spark::IVerifyParking::OK;
    m_priceProvider->m_info = spark::PriceInfo(1.2, 120, 1);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Request timed out.\n", response);
    EXPECT_FALSE(m_verifier->m_lastEvent.isValid());
}


TEST_F(DemoModeBLEServiceTest, InvalidTokenResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/InvalidTokenDemo.txt");
    ble.init(m_priceProvider.get(), m_verifier.get());
    m_verifier->m_result = spark::IVerifyParking::OK;
    m_priceProvider->m_info = spark::PriceInfo(1.2, 120, 1);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Payment token is invalid.\n", response);
    EXPECT_FALSE(m_verifier->m_lastEvent.isValid());
}


TEST_F(DemoModeBLEServiceTest, OtherErrorResponse)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/OtherErrorDemo.txt");
    ble.init(m_priceProvider.get(), m_verifier.get());
    m_verifier->m_result = spark::IVerifyParking::OK;
    m_priceProvider->m_info = spark::PriceInfo(1.2, 120, 1);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Unknown error.\n", response);
    EXPECT_FALSE(m_verifier->m_lastEvent.isValid());
}


TEST_F(DemoModeBLEServiceTest, UnknownCommand)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/UnknownDemo.txt");
    ble.init(m_priceProvider.get(), m_verifier.get());
    m_verifier->m_result = spark::IVerifyParking::OK;
    m_priceProvider->m_info = spark::PriceInfo(1.2, 120, 1);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Unknown command.\n", response);
    EXPECT_FALSE(m_verifier->m_lastEvent.isValid());
}


TEST_F(DemoModeBLEServiceTest, ConfigError)
{
    spark::DemoModeBLEService ble(INPUT_FIFO, RESPONSE_FIFO, std::string(TEST_DATA_DIR)+"/InvalidDemo.txt");
    ble.init(m_priceProvider.get(), m_verifier.get());
    m_verifier->m_result = spark::IVerifyParking::OK;
    m_priceProvider->m_info = spark::PriceInfo(1.2, 120, 1);
    spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));

    ble.start();
    writeInputFifo(e.toString() + "\n");
    sleep(2);
    std::string response = readResponseFifo();

    EXPECT_EQ("Error: Invalid demo configuration.\n", response);
    EXPECT_FALSE(m_verifier->m_lastEvent.isValid());
}

