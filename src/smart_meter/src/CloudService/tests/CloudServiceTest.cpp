#include "gtest/gtest.h"
#include "CloudService.h"
#include "misc/ParkingEvent.h"
#include <sstream>
#include <cstdlib>


TEST (CloudServiceTest, CreateJsonTest)
{
    spark::CloudService s;
    s.init(123, "", "");
    spark::PaymentToken token("verifier", "id123");
    spark::ParkingEvent e("ABC123", "2000-01-01 12:00", 90, token);

    std::string expectedJson = "{"
                               "\"parkingAreaId\":123,"
                               "\"parkingContextType\":\"PAID\","
                               "\"parkingDurationInMinutes\":90,"
                               "\"paymentMethodInformation\":{"
                                    "\"paymentMethodType\":\"verifier\","
                                    "\"paymentMethodReceipt\":\"id123\""
                               "},"
                               "\"registerNumber\":\"ABC123\""
                               "}";

    std::string json = s.createJson(e);
    EXPECT_EQ(expectedJson, json);
}



TEST (CloudServiceTest, CheckConnectionSuccessTest)
{
    spark::CloudService s;
    std::string addr = "www.google.com";
    s.init(123, "", addr);

    std::string cmd = std::string("ping -c 1 ") + addr;
    int expected = system(cmd.data()) == 0;
    bool actual = s.checkConnection();

    EXPECT_EQ(expected, actual);
}


TEST (CloudServiceTest, CheckConnectionFailureTest)
{
    spark::CloudService s;
    std::string addr = "192.168.254.253";
    s.init(123, "", addr);

    EXPECT_FALSE(s.checkConnection());
}
