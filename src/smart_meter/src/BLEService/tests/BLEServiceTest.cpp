#include <gtest/gtest.h>
#include "BLEService.h"
#include "Logger/Logger.h"
#include "PriceProvider/PriceProviderMock.h"
#include "VerifyParking/VerifyParkingMock.h"


class BLEServiceTest : public ::testing::Test
{
protected:

    void SetUp()
    {
        spark::Logger::init();
    }

    void TearDown()
    {
        spark::Logger::close();
    }
};



TEST_F(BLEServiceTest, ConstructorTest)
{
    spark::BLEService s;
    EXPECT_TRUE(s.priceProvider() == nullptr);
    EXPECT_TRUE(s.verifier() == nullptr);
}


TEST_F(BLEServiceTest, InitTest)
{
    spark::BLEService s;
    sparktest::PriceProviderMock ppMock;
    sparktest::VerifyParkingMock pvMock;
    s.init(&ppMock, &pvMock);

    EXPECT_TRUE(s.priceProvider() == &ppMock);
    EXPECT_TRUE(s.verifier() == &pvMock);
}
