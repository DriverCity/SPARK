#include "gtest/gtest.h"
#include "PriceInfo.h"

TEST (PriceInfoTest, DefaultConstructor)
{
    spark::PriceInfo info;

    EXPECT_FLOAT_EQ(0.0, info.pricePerHour());
    EXPECT_EQ(0, info.timeLimit());
    EXPECT_EQ(0, info.timeResolution());
    EXPECT_FALSE(info.isValid());
}


TEST (PriceInfoTest, Constructor)
{
    spark::PriceInfo info(1.2, 120, 10);

    EXPECT_FLOAT_EQ(1.2, info.pricePerHour());
    EXPECT_EQ(120, info.timeLimit());
    EXPECT_EQ(10, info.timeResolution());
    EXPECT_TRUE(info.isValid());
}
