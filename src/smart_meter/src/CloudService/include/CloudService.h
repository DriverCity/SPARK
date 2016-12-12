#ifndef CLOUDSERVICE_H
#define CLOUDSERVICE_H
#include "ICloudService.h"

namespace spark {



class CloudService : public ICloudService
{
public:

    CloudService();

    virtual ~CloudService();

    // ICloudService
    virtual void init(int areaId, const std::string& priceAPI, const std::string& eventAPI);
    virtual bool checkConnection();
    virtual double getPricePerHour();
    virtual int getParkingTimeResolution();
    virtual int getTimeLimit();
    virtual Result verifyParkingEvent(const ParkingEvent &event);

    std::string createJson(const ParkingEvent& e);

private:
    int m_areaId;
    std::string m_priceAPI;
    std::string m_eventAPI;
};

}
#endif

