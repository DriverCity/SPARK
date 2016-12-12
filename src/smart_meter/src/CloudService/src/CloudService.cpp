#include "CloudService.h"
#include <cassert>
#include <sstream>

namespace spark
{

CloudService::CloudService() :
    ICloudService(), m_areaId(0)
{
}


CloudService::~CloudService()
{

}

void CloudService::init(int areaId)
{
    m_areaId = areaId;
}


bool CloudService::checkConnection()
{
    return true;
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
