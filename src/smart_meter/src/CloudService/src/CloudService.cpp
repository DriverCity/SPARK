#include "CloudService.h"
#include <cassert>
#include <sstream>
#include <cstdlib>


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


ICloudService::Result CloudService::verifyParkingEvent(const ParkingEvent &event)
{
    return OK;
}


std::string CloudService::createJson(const ParkingEvent &e)
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


} // spark
