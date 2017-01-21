/**
 * @file
 * @brief Defines abstract interface for cloud service.
 */

#ifndef ICLOUDSERVICE_H
#define ICLOUDSERVICE_H

#include <string>
#include "misc/ParkingEvent.h"

namespace spark
{

/**
 * @brief Abstract interface for accessing the cloud.
 */
class ICloudService
{
public:

    /**
     * @brief Responses to parking registration requests.
     */
    enum Result
    {
        //! Success
        OK = 0,
        //! Connection timed out.
        TIMEOUT,
        //! Payment token is invalid.
        INVALID_TOKEN,
        //! Other error.
        OTHER
    };

    /**
     * @brief Destructor.
     */
    virtual ~ICloudService() {}

    /**
     * @brief Initialize CloudService.
     * @param areaId Parking are Id.
     * @param priceAPI Address to price REST API.
     * @param eventAPI Address to event REST API.
     */
    virtual void init(int areaId,
                      const std::string& priceAPI,
                      const std::string& eventAPI) = 0;

    /**
     * @brief Check if connection to cloud is ok.
     * @return True, if connection is ok.
     */
    virtual bool checkConnection() = 0;

    /**
     * @brief Get parking price and maximum parking time.
     * @param[out] pricePerHour Price per hour (EUR).
     * @param[out] timeLimit Maximum parking time (min).
     * @return True, if information was extracted successfully.
     */
    virtual bool getPriceInformation(double& pricePerHour, int& timeLimit) = 0;

    /**
     * @brief Register verified parking event to the cloud.
     * @param event Parking event data.
     * @return Response from the cloud.
     */
    virtual Result verifyParkingEvent(const ParkingEvent& event) = 0;

};

} // spark

#endif // ICLOUDSERVICE_H
