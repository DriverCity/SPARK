#include <gtest/gtest.h>
#include "PriceProvider.h"
#include "CloudService/CloudServiceMock.h"

using namespace ::testing;

TEST(PriceProviderTest,constructortest)
{

    spark::PriceProvider provider;
    sparktest::CloudServiceMock cloudmock;

    provider.init(&cloudmock);

    EXPECT_EQ(&cloudmock,provider.getCloudService());
}

TEST(PriceProviderTest, getpricetest)
{
    spark::PriceProvider provider;
    sparktest::CloudServiceMock cloudmock;
    provider.init(&cloudmock);

    EXPECT_CALL(cloudmock, getPricePerHour()).WillOnce(Return(2.5));
    EXPECT_CALL(cloudmock, getParkingTimeResolution()).WillOnce(Return(2));
    EXPECT_CALL(cloudmock, getTimeLimit()).WillOnce(Return(5));

    spark::PriceInfo info = provider.getPriceInfo();
    EXPECT_EQ(2.5,info.pricePerHour());
    EXPECT_EQ(5,info.timeLimit());
    EXPECT_EQ(2,info.timeResolution());


}
