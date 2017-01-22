#include "gtest/gtest.h"
#include "CloudService.h"
#include "misc/ParkingEvent.h"
#include <sstream>
#include <cstdlib>
#include <curl/curl.h>
#include "json/json.h"
#include "Logger/Logger.h"


namespace
{

bool isConnected()
{
    return system("ping -c 1 8.8.8.8") == 0;
}

}


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
                               "\"paymentReceipt\":\"id123\""
                               "},"
                               "\"registerNumber\":\"ABC123\""
                               "}\n";

    std::string json = s.createParkingEventJson(e);
    EXPECT_EQ(expectedJson, json);
}



TEST (CloudServiceTest, CheckConnectionTest)
{
    spark::CloudService s;
    s.init(123, "", "");
    int expected = isConnected();
    bool actual = s.checkConnection();

    EXPECT_EQ(expected, actual);
}



TEST (CloudServiceTest, CurlCodeToResultTest)
{
    spark::CloudService s;

    EXPECT_EQ(spark::ICloudService::OK,             s.curlCodeToResult(CURLE_OK));
    EXPECT_EQ(spark::ICloudService::TIMEOUT,        s.curlCodeToResult(CURLE_COULDNT_CONNECT));
    EXPECT_EQ(spark::ICloudService::TIMEOUT,        s.curlCodeToResult(CURLE_COULDNT_RESOLVE_HOST));
    EXPECT_EQ(spark::ICloudService::TIMEOUT,        s.curlCodeToResult(CURLE_OPERATION_TIMEDOUT));
    EXPECT_EQ(spark::ICloudService::TIMEOUT,        s.curlCodeToResult(CURLE_NO_CONNECTION_AVAILABLE));
    EXPECT_EQ(spark::ICloudService::INVALID_TOKEN,  s.curlCodeToResult(CURLE_HTTP_RETURNED_ERROR));
    EXPECT_EQ(spark::ICloudService::OTHER,          s.curlCodeToResult(CURLE_HTTP_POST_ERROR));
}


TEST (CloudServiceTest, extractPriceInfoSuccess)
{
    Json::Value jsonVal;
    jsonVal["1234"]["properties"]["PRICE"] = 1.2;
    jsonVal["1234"]["properties"]["MAX_TIME"] = 2;
    std::string jsonStr = Json::FastWriter().write(jsonVal);

    double price(0);
    int limit(0);
    spark::CloudService s;
    s.init(1234, "", "");

    EXPECT_NO_THROW(s.extractPriceInfo(jsonStr, price, limit));
    EXPECT_DOUBLE_EQ(1.2, price);
    EXPECT_EQ(120, limit);
}


TEST (CloudServiceTest,  extractPriceInfoPriceNotFound)
{
    Json::Value jsonVal;
    jsonVal["1234"]["properties"]["MAX_TIME"] = 2;
    std::string jsonStr = Json::FastWriter().write(jsonVal);

    double price(0);
    int limit(0);
    spark::CloudService s;
    s.init(1234, "", "");

    EXPECT_THROW(s.extractPriceInfo(jsonStr, price, limit), std::exception);
}

TEST (CloudServiceTest,  extractPriceInfoLimitNotFound)
{
    Json::Value jsonVal;
    jsonVal["1234"]["properties"]["PRICE"] = 1.2;
    std::string jsonStr = Json::FastWriter().write(jsonVal);

    double price(0);
    int limit(0);
    spark::CloudService s;
    s.init(1234, "", "");

    EXPECT_THROW(s.extractPriceInfo(jsonStr, price, limit), std::exception);
}

TEST (CloudServiceTest, extractPriceInfoPriceNotANumber)
{
    Json::Value jsonVal;
    jsonVal["1234"]["properties"]["PRICE"] = "asd";
    jsonVal["1234"]["properties"]["MAX_TIME"] = 2;
    std::string jsonStr = Json::FastWriter().write(jsonVal);

    double price(0);
    int limit(0);
    spark::CloudService s;
    s.init(1234, "", "");

    EXPECT_THROW(s.extractPriceInfo(jsonStr, price, limit), std::exception);
}

TEST (CloudServiceTest, extractPriceInfoLimitNotANumber)
{
    Json::Value jsonVal;
    jsonVal["1234"]["properties"]["PRICE"] = "asd";
    jsonVal["1234"]["properties"]["MAX_TIME"] = 2;
    std::string jsonStr = Json::FastWriter().write(jsonVal);

    double price(0);
    int limit(0);
    spark::CloudService s;
    s.init(1234, "", "");

    EXPECT_THROW(s.extractPriceInfo(jsonStr, price, limit), std::exception);
}


TEST (CloudServiceTest, getPriceInfoRealCaseSuccess)
{
    spark::CloudService service;
    service.init(1022,
                 "https://spark2-150308.firebaseio.com/parkingArea.json?orderBy=%22area_number%22&equalTo=",
                 "https://spark2-150308.appspot-preview.com/api/v1.0/parkingEvent/");

    if (isConnected()){
        double price(0);
        int limit(0);

        EXPECT_TRUE(service.getPriceInformation(price, limit));
        EXPECT_DOUBLE_EQ(1.6, price);
        EXPECT_EQ(240, limit);
    }
    else {
        LOG_ERROR("***** SKIPPED, NO INTERNET CONNECTION *****");
    }
}


TEST (CloudServiceTest, getPriceInfoRealCaseFailure)
{
    spark::CloudService service;
    service.init(std::numeric_limits<int>::max(),
                 "https://spark2-150308.firebaseio.com/parkingArea.json?orderBy=%22area_number%22&equalTo=",
                 "https://spark2-150308.appspot-preview.com/api/v1.0/parkingEvent/");

    if (isConnected()){
        double price(0);
        int limit(0);

        EXPECT_FALSE(service.getPriceInformation(price, limit));
    }
    else {
        LOG_ERROR("***** SKIPPED, NO INTERNET CONNECTION *****");
    }
}


TEST (CloudServiceTest, registerParkingRealCaseOk)
{
    spark::CloudService service;
    service.init(0,
                 "https://spark2-150308.firebaseio.com/parkingArea.json?orderBy=%22area_number%22&equalTo=",
                 "https://spark2-150308.appspot-preview.com/api/v1.0/parkingEvent/");

    if (isConnected()){

        spark::ParkingEvent e("ABC123", "2000-01-01 10:00", 90, spark::PaymentToken("SERVICE_1", "valid_test_hash"));
        EXPECT_EQ(spark::ICloudService::OK, service.verifyParkingEvent(e));
    }
    else {
        LOG_ERROR("***** SKIPPED, NO INTERNET CONNECTION *****");
    }
}
