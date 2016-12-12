#include "CloudService.h"
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <curl/curl.h>

#define ACCEPT_HDR "Accept: application/json"
#define CONTENT_TYPE_HDR "Content-Type: application/json"


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
    std::string cmd = std::string("ping -c 1 ") + m_eventAPI;
    int result = system( cmd.data() );
    return result == 0;
}


double CloudService::getPricePerHour()
{
    return 1.2;
}


int CloudService::getParkingTimeResolution()
{
    return 10;
}


int CloudService::getTimeLimit()
{
    return 0;
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

    CURLcode result = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return curlCodeToResult(result);
}


std::string CloudService::createParkingEventJson(const ParkingEvent& e) const
{
    std::ostringstream oss;
    oss << "{"
        << "\"parkingAreaId\":" << m_areaId << ","
        << "\"parkingContextType\":\"PAID\","
        << "\"parkingDurationInMinutes\":" << e.duration() << ","
        << "\"paymentMethodInformation\":{"
        << "\"paymentMethodType\":" << "\"" << e.token().verifier() << "\","
        << "\"paymentMethodReceipt\":" << "\"" << e.token().uid() << "\"},"
        << "\"registerNumber\":" << "\"" << e.registerNumber() << "\""
        << "}";

    return oss.str();
}


std::string CloudService::createPriceRequestJSON(int parkingAreaId) const
{
    std::ostringstream oss;
    oss << "{"
        << "\"parkingAreaId\":" << parkingAreaId
        << "}";

    return oss.str();
}


ICloudService::Result CloudService::curlCodeToResult(int code)
{
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


} // spark
