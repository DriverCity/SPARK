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
