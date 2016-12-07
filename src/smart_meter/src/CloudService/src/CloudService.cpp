#include "CloudService.h"

namespace spark
{

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


}
