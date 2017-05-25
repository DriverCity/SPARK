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
    virtual bool getPriceInformation(double& pricePerHour, int& timeLimit);
    virtual Result verifyParkingEvent(const ParkingEvent &event);

    /**
     * @brief Convert ParkingEvent to JSON object.
     * @param e Parking event
     * @return  JSON representation of @p e
     */
    std::string createParkingEventJson(const ParkingEvent& e) const;

    /**
     * @brief Convert curl return code to ICloudService::Result.
     * @param code Curl return code.
     * @return Respective ICloudService::Result.
     */
    Result curlCodeToResult(int code);

    /**
     * @brief Get price information from json string.
     * @param[in] json Json string.
     * @param[out] price Extracted price.
     * @param[out] limit Extracted time limit.
     * @throws std::exception, if values could not be fetched.
     */
    void extractPriceInfo(const std::string& json, double& price, int& limit);


private:
    int m_areaId;
    std::string m_priceAPI;
    std::string m_eventAPI;
};

}
#endif

