#include "CloudService.h"
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <curl/curl.h>
#include "Logger/Logger.h"

#define ACCEPT_HDR "Accept: application/json"
#define CONTENT_TYPE_HDR "Content-Type: application/json"


namespace spark
{


double extractField(std::string fieldType, std::string &str){

    //return value
    double value = 0.0 ;

    //initialize extracted string of field
    std::string extractedStr ;

    //Remove whitespaces from data
    std::string::iterator lastPos = std::remove(str.begin(),str.end(),' ');
    str.erase(lastPos,str.end());

    //Define the field we want
    fieldType = "\"" + fieldType + "\":" ;


    //Get position of field type in string
    std::size_t typePos = str.find(fieldType) + fieldType.length();
    if (typePos == std::string::npos ){
        //LOG NO FIELD ERROR
        return 0.0;

    }

    //Get position for the end of field type
    std::size_t commaPos = str.find(",",typePos);
    if (commaPos == std::string::npos){
        //LOG STRUCTURED DATA ERROR OR SMTH
        return 0.0;
    }


    //Extract value for the field
    try{
    extractedStr = str.substr(typePos, commaPos-typePos );
    }catch(std::out_of_range &e){
        //LOG STR ERROR
        return 0.0;
    }

    //Convert extracted value to double
    value = std::stod(extractedStr);


    return value;

}


size_t CurlWrite_Callback_ToString(void *contents, size_t size, size_t nmemb, std::string *s){

    size_t newLength = size*nmemb;
    size_t oldLength = s->size();

    try
    {
        s->resize(oldLength + newLength);
    }
    catch(std::bad_alloc &e)
    {
        //LOG ERROR
        return 0;
    }

    std::copy((char*)contents,(char*)contents+newLength,s->begin()+oldLength);

    return size*nmemb;

}

void writeAreaInfo(int areaNo, const std::string& url, std::string& response){

    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);

    std::string areaNoStr = std::to_string(areaNo);


    //std::string url = "https://spark2-150308.firebaseio.com/parkingArea//properties.json?print=pretty";
    //std::size_t firstPos = url.find("/parkingArea/") ;
    //std::size_t lastPos = url.find("/properties");

    std::string fullUrl = url + areaNoStr;
    const char * newUrl = fullUrl.c_str();

    curl = curl_easy_init();
    //std::string s;
    if(curl)
    {

        curl_easy_setopt(curl, CURLOPT_URL, newUrl);

        //Set callback for received data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWrite_Callback_ToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);


        //Perform
        res = curl_easy_perform(curl);


        //Check errors
        if(res != CURLE_OK)
        {
            //LOG_ERROR
        }

        curl_easy_cleanup(curl);
    }

}

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


double CloudService::getPricePerHour()
{
    double pricePerHour;

    std::string s;

    writeAreaInfo(m_areaId, m_priceAPI, s);

    pricePerHour = extractField("PRICE",s);

    return pricePerHour;
}


int CloudService::getParkingTimeResolution()
{
    return 1;
}


int CloudService::getTimeLimit()
{
    int maxTime;

    std::string s;

    writeAreaInfo(m_areaId, m_priceAPI, s);

    maxTime = 60 * extractField("MAX_TIME",s);

    return int(maxTime);
}


ICloudService::Result CloudService::verifyParkingEvent(const ParkingEvent& event)
{
    LOG_DEBUG("RegisterNumber: " << event.registerNumber());
    LOG_DEBUG("Duration: " << event.duration());
    LOG_DEBUG("Verifier: " << event.token().verifier());
    LOG_DEBUG("id: " << event.token().uid());
    LOG_DEBUG("addr: " << m_eventAPI);
    LOG_DEBUG("area: " << m_areaId);

    std::string json = createParkingEventJson(event);
    CURL* curl = curl_easy_init();

    curl_slist* headers = NULL;
    headers = curl_slist_append(headers, CONTENT_TYPE_HDR);
    headers = curl_slist_append(headers, ACCEPT_HDR);

    curl_easy_setopt(curl, CURLOPT_URL, m_eventAPI.data());
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.data());
    LOG_DEBUG("JSON: " << json.data());
    curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

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
        << "\"paymentReceipt\":" << "\"" << e.token().uid() << "\"},"
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
