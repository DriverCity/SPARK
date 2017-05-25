#include <gtest/gtest.h>
#include "ParkingEvent.h"


TEST (ParkingEventTest, TokenTest)
{
    spark::PaymentToken token("verifier", "uid123456");

    EXPECT_EQ("verifier", token.verifier());
    EXPECT_EQ("uid123456", token.uid());
}


TEST (ParkingEventTest, EventDefaultConstructor)
{
    spark::ParkingEvent event;

    EXPECT_EQ("", event.registerNumber());
    EXPECT_EQ("", event.startingTime());
    EXPECT_EQ(0, event.duration());
    EXPECT_EQ("", event.token().verifier());
    EXPECT_EQ("", event.token().uid());
}


TEST (ParkingEventTest, EventTest)
{
    spark::PaymentToken token("verifier", "uid123456");
    spark::ParkingEvent event("ABC123", "2016-11-15 12:30:00", 90, token);

    EXPECT_EQ("ABC123", event.registerNumber());
    EXPECT_EQ("2016-11-15 12:30:00", event.startingTime());
    EXPECT_EQ(90, event.duration());
    EXPECT_EQ("verifier", event.token().verifier());
    EXPECT_EQ("uid123456", event.token().uid());
}

TEST (ParkingEventTest, IsValidTest)
{
    spark::ParkingEvent event1("ABC123", "2016-11-15 12:00", 30, spark::PaymentToken("verifier", "uid123456"));
    spark::ParkingEvent event2;

    EXPECT_TRUE(event1.isValid());
    EXPECT_FALSE(event2.isValid());
}


TEST (ParkingEventTest, ToStringTest)
{
    // Invalid
    spark::ParkingEvent e1;
    EXPECT_EQ("", e1.toString());

    // Valid
    spark::ParkingEvent e2("ABC123", "2016-11-22 12:30", 90, spark::PaymentToken("ver", "123"));
    EXPECT_EQ("Park;ABC123;2016-11-22 12:30;90;ver;123", e2.toString());
}


TEST (ParkingEventTest, FromStringValidTest)
{
    std::string s1 = "Park;ABC123;2016-11-22 12:30;90;ver;123";
    spark::ParkingEvent e1 = spark::ParkingEvent::fromString(s1);
    EXPECT_EQ("ABC123", e1.registerNumber());
    EXPECT_EQ("2016-11-22 12:30", e1.startingTime());
    EXPECT_EQ(90, e1.duration());
    EXPECT_EQ("ver", e1.token().verifier());
    EXPECT_EQ("123", e1.token().uid());
}


TEST (ParkingEventTest, FromStringMissingRegNum)
{
    std::string s1 = "Park;2016-11-22 12:30;90;ver;123";
    spark::ParkingEvent e1 = spark::ParkingEvent::fromString(s1);
    EXPECT_FALSE(e1.isValid());
}


TEST (ParkingEventTest, FromStringMissingStarter)
{
    std::string s1 = ";ABC123;2016-11-22 12:30;90;ver;123";
    spark::ParkingEvent e1 = spark::ParkingEvent::fromString(s1);
    EXPECT_FALSE(e1.isValid());
}


TEST (ParkingEventTest, FromStringMissingStartTime)
{
    std::string s1 = "Park;ABC123;90;ver;123";
    spark::ParkingEvent e1 = spark::ParkingEvent::fromString(s1);
    EXPECT_FALSE(e1.isValid());
}


TEST (ParkingEventTest, FromStringMissingDuration)
{
    std::string s1 = "Park;ABC123;2016-11-22 12:30;ver;123";
    spark::ParkingEvent e1 = spark::ParkingEvent::fromString(s1);
    EXPECT_FALSE(e1.isValid());
}


TEST (ParkingEventTest, FromStringMissingVerifier)
{
    std::string s1 = "Park;ABC123;2016-11-22 12:30;90;123";
    spark::ParkingEvent e1 = spark::ParkingEvent::fromString(s1);
    EXPECT_FALSE(e1.isValid());
}


TEST (ParkingEventTest, FromStringMissingUid)
{
    std::string s1 = "Park;ABC123;2016-11-22 12:30;90;ver";
    spark::ParkingEvent e1 = spark::ParkingEvent::fromString(s1);
    EXPECT_FALSE(e1.isValid());
}


TEST (ParkingEventTest, FromStringInvalidStartTime)
{
    std::string s1 = "Park;ABC123;2016-11-22 12-30;90;ver;123";
    spark::ParkingEvent e1 = spark::ParkingEvent::fromString(s1);
    EXPECT_FALSE(e1.isValid());

    std::string s2 = "Park;ABC123;2016-11-22 25:30;90;ver;123";
    spark::ParkingEvent e2 = spark::ParkingEvent::fromString(s2);
    EXPECT_FALSE(e2.isValid());

    std::string s3 = "Park;ABC123;2016-11-32 12:30;90;ver;123";
    spark::ParkingEvent e3 = spark::ParkingEvent::fromString(s3);
    EXPECT_FALSE(e3.isValid());

    std::string s4 = "Park;ABC123;2016-13-22 12:30;90;ver;123";
    spark::ParkingEvent e4 = spark::ParkingEvent::fromString(s4);
    EXPECT_FALSE(e4.isValid());
}


TEST (ParkingEventTest, FromStringInvalidDuration)
{
    std::string s1 = "Park;ABC123;2016-11-22 12:30;0;ver;123";
    spark::ParkingEvent e1 = spark::ParkingEvent::fromString(s1);
    EXPECT_FALSE(e1.isValid());
}


TEST (ParkingEventTest, FromStringDurationNotANumber)
{
   std::string str = "Park;ABC123;2016-11-22 12:30;asd;ver;123";
   spark::ParkingEvent e1 = spark::ParkingEvent::fromString(str);
   EXPECT_FALSE(e1.isValid());
}


TEST (ParkingEventTest, ShouldBeEqual)
{
    spark::ParkingEvent e1 = spark::ParkingEvent("ASD123", "2000-01-01 10:10", 90, spark::PaymentToken("ver", "id123"));
    spark::ParkingEvent e2 = spark::ParkingEvent("ASD123", "2000-01-01 10:10", 90, spark::PaymentToken("ver", "id123"));
    EXPECT_TRUE(e1 == e2);
}


TEST (ParkingEventTest, ShouldNotBeEqual)
{
    spark::ParkingEvent e1 = spark::ParkingEvent("ASD123", "2000-01-01 10:10", 90, spark::PaymentToken("ver", "id123"));

    {
        // diff regnum
        spark::ParkingEvent e2 = spark::ParkingEvent("ASD456", "2000-01-01 10:10", 90, spark::PaymentToken("ver", "id123"));
        EXPECT_FALSE(e1 == e2);
    }
    {
        // diff time
        spark::ParkingEvent e2 = spark::ParkingEvent("ASD123", "2000-01-01 10:11", 90, spark::PaymentToken("ver", "id123"));
        EXPECT_FALSE(e1 == e2);
    }
    {
        // diff duration
        spark::ParkingEvent e2 = spark::ParkingEvent("ASD123", "2000-01-01 10:10", 91, spark::PaymentToken("ver", "id123"));
        EXPECT_FALSE(e1 == e2);
    }
    {
        // diff verifier
        spark::ParkingEvent e2 = spark::ParkingEvent("ASD123", "2000-01-01 10:10", 90, spark::PaymentToken("asd", "id123"));
        EXPECT_FALSE(e1 == e2);
    }
    {
        // diff transaction id
        spark::ParkingEvent e2 = spark::ParkingEvent("ASD123", "2000-01-01 10:10", 90, spark::PaymentToken("ver", "id456"));
        EXPECT_FALSE(e1 == e2);
    }
}
