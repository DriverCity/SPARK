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
        OK = 0,             //! Success
        TIMEOUT,            //! Connection timed out.
        INVALID_TOKEN,      //! Payment token is invalid.
        OTHER               //! Other error.
    };

    virtual ~ICloudService() {}

    /**
     * @brief Check if connection to cloud is ok.
     * @return True, if connection is ok.
     */
    virtual bool checkConnection() = 0;

    /**
     * @brief Get price per hour for this meter.
     * @return Price per hour in EUR. Negative number if query fails.
     */
    virtual double getPricePerHour() = 0;

    /**
     * @brief Get parking time resolution for this meter.
     * @return Smalles quantity of parking time in minutes. Negative number if query fails.
     */
    virtual int getParkingTimeResolution() = 0;

    /**
     * @brief Get maximum alloted parking time for this meter.
     * @return Maximum parking time in minutes.
     *  0 if there is no time limit for this meter.
     *  Negative number if query fails.
     */
    virtual int getTimeLimit() = 0;

    /**
     * @brief Register verified parking event to the cloud.
     * @param event Parking event data.
     * @return Response from the cloud.
     */
    virtual Result verifyParkingEvent(const ParkingEvent& event) = 0;

};

} // spark

#endif // ICLOUDSERVICE_H
