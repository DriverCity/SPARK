#ifndef CLOUDSERVICE_H
#define CLOUDSERVICE_H
#include "ICloudService.h"

namespace spark {



class CloudService : public ICloudService
{


    // ICloudService interface
public:
    virtual bool checkConnection();
    virtual double getPricePerHour();
    virtual int getParkingTimeResolution();
    virtual int getTimeLimit();
    virtual Result verifyParkingEvent(const ParkingEvent &event);
};

}
#endif

