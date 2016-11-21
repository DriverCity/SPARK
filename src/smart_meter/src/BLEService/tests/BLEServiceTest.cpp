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



class BLEServiceTest : public ::testing::Test
{
public:

    BLEServiceTest():
        ::testing::Test(),
        m_service(nullptr),
        m_priceProvider(new sparktest::PriceProviderMock()),
        m_verifier(new sparktest::VerifyParkingMock())
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
        std::ofstream ofs1(INPUT_FIFO);
        ofs1 << "";
        ofs1.close();
        std::ofstream ofs2(RESPONSE_FIFO);
        ofs2 << "";
        ofs2.close();

        spark::Logger::init();
        m_service.reset(new spark::BLEService(INPUT_FIFO, RESPONSE_FIFO));
        m_responseFd = open(RESPONSE_FIFO, O_NONBLOCK, O_RDONLY);

    }

    void TearDown()
    {
        m_service.reset();
        m_priceProvider->reset();
        m_verifier->reset();
        spark::Logger::close();
        close(m_responseFd);
        system("rm " INPUT_FIFO);
        system("rm " RESPONSE_FIFO);
    }


    void writeInputFifo(const std::string& str)
    {
        std::ofstream ofs(INPUT_FIFO);
        ofs << str << std::endl;
        ofs.close();
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
    m_service->init(m_priceProvider.get(), m_verifier.get());

    EXPECT_TRUE(m_service->priceProvider() == m_priceProvider.get());
    EXPECT_TRUE(m_service->verifier() == m_verifier.get());
}


TEST_F(BLEServiceTest, GetPriceSuccess)
{
    m_service->init(m_priceProvider.get(), m_verifier.get());
    m_priceProvider->m_info = spark::PriceInfo(1.2, 0, 10);

    m_service->start();
    writeInputFifo("price");
    sleep(1); // Give service time to response.
    EXPECT_EQ(readResponseFifo(), m_priceProvider->m_info.toString());
}

