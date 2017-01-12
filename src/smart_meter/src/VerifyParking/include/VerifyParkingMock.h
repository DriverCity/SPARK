/**
 * @file
 * @brief Defines mock implementation for IVerifyParking.
 */

#ifndef VERIFYPARKINGMOCK_H
#define VERIFYPARKINGMOCK_H

#include "IVerifyParking.h"

namespace sparktest
{

/**
 * @brief Mock implementation for IVerifyParking.
 *  Can be used in unit tests for other components.
 */
class VerifyParkingMock : public spark::IVerifyParking
{
public:

    // Use these to control mock behaviour
    spark::ICloudService* m_cloudService;
    //spark::IParkingDatabase* m_db;
    spark::ParkingEvent m_lastEvent;
    spark::IVerifyParking::Result m_result;

    VerifyParkingMock():
        m_cloudService(nullptr),
        m_lastEvent(),
        m_result(spark::IVerifyParking::OK)
    {
    }

    virtual ~VerifyParkingMock() {}

    virtual void init(spark::ICloudService *cloudService)
    {
        m_cloudService = cloudService;
        //m_db = db;
    }

    virtual Result verify(const spark::ParkingEvent& event)
    {
        m_lastEvent = event;
        return m_result;
    }


    /**
     * @brief Reset mock back to its initial state.
     */
    void reset()
    {
        m_cloudService = nullptr;
        //m_db = nullptr;
        m_lastEvent = spark::ParkingEvent();
        m_result = spark::IVerifyParking::OK;
    }
};

} // sparktest

#endif // VERIFYPARKINGMOCK_H
