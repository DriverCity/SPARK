#include "CloudService.h"
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <curl/curl.h>
#include "json/json.h"
#include "Logger/Logger.h"

#define ACCEPT_HDR "Accept: application/json"
#define CONTENT_TYPE_HDR "Content-Type: application/json"

namespace
{

size_t CurlWrite_Callback_ToString(void *contents, size_t size, size_t nmemb, std::string *s){

    size_t newLength = size*nmemb;
    size_t oldLength = s->size();
    s->resize(oldLength + newLength);
    std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);
    return size*nmemb;
}


void writeAreaInfo(int areaNo, const std::string& url, std::string& response){

    CURL *curl;
    curl_global_init(CURL_GLOBAL_DEFAULT);

    std::string areaNoStr = std::to_string(areaNo);
    std::string fullUrl = url + areaNoStr;
    const char * newUrl = fullUrl.c_str();

    curl = curl_easy_init();
    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, newUrl);

        //Set callback for received data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_Callback_ToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

        //Perform
        CURLcode code = curl_easy_perform(curl);
        if (code != CURLE_OK){
            LOG_ERROR("Error while performing http request: " << code);
        }
        curl_easy_cleanup(curl);
    }
    else {
        LOG_ERROR("Could not initialize curl!");
    }
}


}


namespace spark
{


CloudService::CloudService() :
    ICloudService(), m_areaId(0), m_priceAPI(), m_eventAPI()
{
}


CloudService::~CloudService()
{
}


void CloudService::init(int areaId,
                        const std::string& priceAPI,
                        const std::string& eventAPI)
{
    m_areaId = areaId;
    m_priceAPI = priceAPI;
    m_eventAPI = eventAPI;
}


bool CloudService::checkConnection()
{
    std::string cmd = std::string("ping -c 1 8.8.8.8");
    int result = system( cmd.data() );
    return result == 0;
}

bool CloudService::getPriceInformation(double &pricePerHour, int &timeLimit)
{
    std::string response;
    writeAreaInfo(m_areaId, m_priceAPI, response);

    LOG_DEBUG("Got response: " << response);

    bool result = false;
    try{
        extractPriceInfo(response, pricePerHour, timeLimit);
        result = true;
    }
    catch (std::exception&){
        LOG_ERROR("Extracting price information failed!");
    }
    return result;
}


ICloudService::Result CloudService::verifyParkingEvent(const ParkingEvent& event)
{
    std::string json = createParkingEventJson(event);
    CURL* curl = curl_easy_init();

    curl_slist* headers = NULL;
    headers = curl_slist_append(headers, CONTENT_TYPE_HDR);
    headers = curl_slist_append(headers, ACCEPT_HDR);

    curl_easy_setopt(curl, CURLOPT_URL, m_eventAPI.data());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.data());
    curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return curlCodeToResult(result);
}


std::string CloudService::createParkingEventJson(const ParkingEvent& e) const
{
    (void)e;

    Json::Value event;
    event["parkingAreaId"] = m_areaId;
    event["parkingContextType"] = "PAID";
    event["parkingDurationInMinutes"] = e.duration();
    event["paymentMethodInformation"]["paymentMethodType"] = e.token().verifier();
    event["paymentMethodInformation"]["paymentReceipt"] = e.token().uid();
    event["registerNumber"] = e.registerNumber();

    return Json::FastWriter().write(event);
}


ICloudService::Result CloudService::curlCodeToResult(int code)
{
    LOG_DEBUG("CURLcode: " << code);

    switch (code)
    {
    case CURLE_OK:
        return OK;

    case CURLE_HTTP_RETURNED_ERROR:
        return INVALID_TOKEN;

    case CURLE_NO_CONNECTION_AVAILABLE:
    case CURLE_COULDNT_RESOLVE_HOST:
    case CURLE_COULDNT_CONNECT:
    case CURLE_OPERATION_TIMEDOUT:
        return TIMEOUT;

    default:
        break;
    }

    return OTHER;
}


void CloudService::extractPriceInfo(const std::string& json, double& price, int& limit)
{
    std::istringstream iss(json);
    Json::Reader reader;
    Json::Value jsonValue;

    reader.parse(iss, jsonValue, false);
    Json::Value areaValue = jsonValue
            .get(std::to_string(m_areaId), Json::Value::null)
            .get("properties", Json::Value::null);

    Json::Value priceValue = areaValue.get("PRICE", Json::Value::null);
    Json::Value limitValue = areaValue.get("MAX_TIME", Json::Value::null);

    if (priceValue.isNull() || limitValue.isNull()){
        throw std::exception();
    }

    price = priceValue.asDouble();
    limit = int(limitValue.asDouble() * 60);
}


} // spark
