/**
 * @file
 * @brief Define interface for verifying parking event.
 */

#ifndef IVERIFYPARKING_H
#define IVERIFYPARKING_H

#include "misc/ParkingEvent.h"
#include "CloudService/ICloudService.h"
#include "ParkingDatabase/IParkingDatabase.h"

namespace spark
{

/**
 * @brief Interface for registering parking events.
 */
class IVerifyParking
{
public:

    /**
     * @brief Possible responses to verification request.
     */
    enum Result
    {
        OK = 0,             //! Success
        TIMEOUT,            //! Cloud connection timed out.
        INVALID_TOKEN,      //! Payment token is invalid.
        OTHER               //! Other error.
    };

    virtual ~IVerifyParking() {}

    /**
     * @brief Initialize VerifyParking.
     * @param cloudService Cloud service.
     * @param db Temporary event database.
     * @pre @p cloudService != nullptr, @p db != nullptr.
     * @post VerifyParking is ready to use.
     */
    virtual void init(ICloudService* cloudService) = 0;

    /**
     * @brief Register parking event.
     * @param event Parking event.
     * @return OK, if event was stored to cloud. Else an appropriate error result.
     * @pre init() has been called.
     * @post Event is sent to cloud. If cloud is not available. Event is stored to
     *  temporary parking event database.
     */
    virtual Result verify(const ParkingEvent& event) = 0;
};

} // spark

#endif // IVERIFYPARKING_H
