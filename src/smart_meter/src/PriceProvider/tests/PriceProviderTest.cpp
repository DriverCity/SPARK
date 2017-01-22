#include <gtest/gtest.h>
#include "PriceProvider.h"
#include "CloudService/CloudServiceMock.h"

using namespace ::testing;

namespace
{

double parkingPrice = 0;
int parkingTimeLimit = 0;

void setPriceInformation(double& price, int& limit)
{
    price = parkingPrice;
    limit = parkingTimeLimit;
}

}


TEST(PriceProviderTest,constructortest)
{

    spark::PriceProvider provider;
    sparktest::CloudServiceMock cloudmock;

    provider.init(&cloudmock);

    EXPECT_EQ(&cloudmock,provider.getCloudService());
}

TEST(PriceProviderTest, GetPriceSuccess)
{
    spark::PriceProvider provider;
    sparktest::CloudServiceMock cloudmock;
    provider.init(&cloudmock);

    parkingPrice = 2.5;
    parkingTimeLimit = 120;
    EXPECT_CALL(cloudmock, getPriceInformation(_, _)).WillOnce(DoAll(Invoke(setPriceInformation), Return(true)));

    spark::PriceInfo info = provider.getPriceInfo();
    EXPECT_TRUE(info.isValid());
    EXPECT_EQ(parkingPrice, info.pricePerHour());
    EXPECT_EQ(parkingTimeLimit, info.timeLimit());
    EXPECT_EQ(5,info.timeResolution());
}


TEST(PriceProviderTest, GetPriceFailure)
{
    spark::PriceProvider provider;
    sparktest::CloudServiceMock cloudmock;
    provider.init(&cloudmock);

    EXPECT_CALL(cloudmock, getPriceInformation(_, _)).WillOnce(Return(false));

    spark::PriceInfo info = provider.getPriceInfo();
    EXPECT_FALSE(info.isValid());
}
