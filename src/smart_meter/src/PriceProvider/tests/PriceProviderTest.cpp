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

