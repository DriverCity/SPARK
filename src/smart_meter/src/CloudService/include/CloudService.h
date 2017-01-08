/**
 * @file
 * @brief Defines the CloudService class.
 */

#ifndef CLOUDSERVICE_H
#define CLOUDSERVICE_H
#include "ICloudService.h"


namespace spark {

/**
 * @brief Implementation for the ICloudService interface.
 */
class CloudService : public ICloudService
{
public:

    /**
     * @brief Constructor.
     */
    CloudService();

    /**
     * @brief Destructor.
     */
    virtual ~CloudService();

    // ICloudService
    virtual void init(int areaId, const std::string& priceAPI, const std::string& eventAPI);
    virtual bool checkConnection();
    virtual double getPricePerHour();
    virtual int getParkingTimeResolution();
    virtual int getTimeLimit();
    virtual Result verifyParkingEvent(const ParkingEvent &event);

    /**
     * @brief Convert ParkingEvent to JSON object.
     * @param e Parking event
     * @return  JSON representation of @p e
     */
    std::string createParkingEventJson(const ParkingEvent& e) const;

    /**
     * @brief Create JSON representation for price request.
     * @param parkingAreaId Parking area id.
     * @return JSON representation for price request.
     */
    std::string createPriceRequestJSON(int parkingAreaId) const;

    /**
     * @brief Convert curl return code to ICloudService::Result.
     * @param code Curl return code.
     * @return Respective ICloudService::Result.
     */
    Result curlCodeToResult(int code);


private:
    int m_areaId;
    std::string m_priceAPI;
    std::string m_eventAPI;
};

}
#endif

