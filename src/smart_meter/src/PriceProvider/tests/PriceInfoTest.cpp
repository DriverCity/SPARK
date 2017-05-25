#include "gtest/gtest.h"
#include "PriceInfo.h"
#include "Logger/Logger.h"

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


TEST (PriceInfoTest, NegativePriceIsInvalid)
{
    spark::PriceInfo info(-1.0, 120, 10);
    EXPECT_FALSE(info.isValid());
}


TEST (PriceInfoTest, NegativeLimitIsInvalid)
{
    spark::PriceInfo info(1.2, -120, 10);
    EXPECT_FALSE(info.isValid());
}


TEST (PriceInfoTest, NonPositiveResolutionIsInvalid)
{
    spark::PriceInfo info(1.2, 120, 0);
    EXPECT_FALSE(info.isValid());
}


TEST (PriceInfoTest, ToStringTest)
{
    spark::PriceInfo info(2.5, 300, 10);
    std::string repr = info.toString();

    EXPECT_EQ("2.5;300;10", repr);
}


TEST (PriceInfoTest, FromStringWithDefaultSeparator)
{
    const std::string str = "1.2;120;10";
    spark::PriceInfo price1 = spark::PriceInfo::fromString(str);
    spark::PriceInfo price2 = spark::PriceInfo::fromString(str, ';');

    EXPECT_TRUE(price1.isValid());
    EXPECT_TRUE(price2.isValid());
    EXPECT_FLOAT_EQ(1.2, price1.pricePerHour());
    EXPECT_FLOAT_EQ(1.2, price2.pricePerHour());
    EXPECT_EQ(120, price1.timeLimit());
    EXPECT_EQ(120, price2.timeLimit());
    EXPECT_EQ(10, price1.timeResolution());
    EXPECT_EQ(10, price2.timeResolution());
}


TEST (PriceInfoTest, FromStringWithCustomSeparator)
{
    const std::string str = "1.2:0:10";
    spark::PriceInfo price = spark::PriceInfo::fromString(str, ':');

    EXPECT_TRUE(price.isValid());
    EXPECT_FLOAT_EQ(1.2, price.pricePerHour());
    EXPECT_EQ(0, price.timeLimit());
    EXPECT_EQ(10, price.timeResolution());
}


TEST (PriceInfoTest, FromStringTooFewFields)
{
    const std::string str = "1.2:120";
    spark::PriceInfo price = spark::PriceInfo::fromString(str, ':');

    LOG_DEBUG(price.toString());
    EXPECT_FALSE(price.isValid());
}


TEST (PriceInfoTest, FromStringTooManyFields)
{
    const std::string str = "1.2:120:10:20";
    spark::PriceInfo price = spark::PriceInfo::fromString(str, ':');

    EXPECT_FALSE(price.isValid());
}


TEST (PriceInfoTest, FromStringPriceNotANumber)
{
    const std::string str = "asd:120:10";
    spark::PriceInfo price = spark::PriceInfo::fromString(str, ':');

    EXPECT_FALSE(price.isValid());
}


TEST (PriceInfoTest, FromStringNegativePrice)
{
    const std::string str = "-1.0:120:10";
    spark::PriceInfo price = spark::PriceInfo::fromString(str, ':');

    EXPECT_FALSE(price.isValid());
}


TEST (PriceInfoTest, FromStringLimitNotANumber)
{
    const std::string str = "1.2:asd:10";
    spark::PriceInfo price = spark::PriceInfo::fromString(str, ':');

    EXPECT_FALSE(price.isValid());
}


TEST (PriceInfoTest, FromStringNegativeLimit)
{
    const std::string str = "1.2:-120:10";
    spark::PriceInfo price = spark::PriceInfo::fromString(str, ':');

    EXPECT_FALSE(price.isValid());
}


TEST (PriceInfoTest, FromStringResolutionNotANumber)
{
    const std::string str = "1.2:120:10asd";
    spark::PriceInfo price = spark::PriceInfo::fromString(str, ':');

    EXPECT_FALSE(price.isValid());
}


TEST (PriceInfoTest, FromStringResolutionNotPositive)
{
    const std::string str = "1.2:120:0";
    spark::PriceInfo price = spark::PriceInfo::fromString(str, ':');

    EXPECT_FALSE(price.isValid());
}
