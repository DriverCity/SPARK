#include <gtest/gtest.h>
#include "PriceProvider.h"
#include "CloudService/CloudServiceMock.h"


TEST(PriceProviderTest,constructortest)
{

    spark::PriceProvider provider;
    sparktest::CloudServiceMock cloudmock;

    provider.init(&cloudmock);

    EXPECT_EQ(&cloudmock,provider.getCloudService());
}

TEST(PriceInfo, getpricetest)
{
    spark::PriceProvider provider;
    sparktest::CloudServiceMock cloudmock;

    provider.init(&cloudmock);

    cloudmock.m_pricePerHour = 2.5;
    cloudmock.m_parkingTimeResolution = 2;
    cloudmock.m_timeLimit =5;

    spark::PriceInfo info = provider.getPriceInfo();
    EXPECT_EQ(2.5,info.pricePerHour());
    EXPECT_EQ(5,info.timeLimit());
    EXPECT_EQ(2,info.timeResolution());


}
