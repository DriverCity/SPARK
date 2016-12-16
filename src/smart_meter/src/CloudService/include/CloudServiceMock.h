/**
 * @file
 * @brief Defined mock implementation for the cloud service.
 */

#ifndef CLOUDSERVICEMOCK_H
#define CLOUDSERVICEMOCK_H

#include "ICloudService.h"

namespace sparktest
{

/**
 * @brief Mock implementation for ICloudService.
 *  This class can be used for unit testing other components.
 */
class CloudServiceMock : public spark::ICloudService
{
public:

    // Use these to check mock status and control its behaviour.
    bool m_connectionOk;
    double m_pricePerHour;
    int m_parkingTimeResolution;
    int m_timeLimit;
    ICloudService::Result m_verifyResult;
    spark::ParkingEvent m_lastEvent;
    int m_parkingAreaId;
    std::string m_priceApi;
    std::string m_eventApi;


    CloudServiceMock() :
        ICloudService(),
        m_connectionOk(true),
        m_pricePerHour(0),
        m_parkingTimeResolution(0),
        m_timeLimit(0),
        m_verifyResult(ICloudService::OK),
        m_lastEvent("", "", 0, spark::PaymentToken("", "")),
        m_parkingAreaId(0),
        m_priceApi(),
        m_eventApi()
    {
    }


    virtual ~CloudServiceMock()
    {
    }


    virtual void init(int areaId, const std::string &priceAPI, const std::string &eventAPI)
    {
        m_parkingAreaId = areaId;
        m_priceApi = priceAPI;
        m_eventApi = eventAPI;
    }


    virtual bool checkConnection()
    {
        return m_connectionOk;
    }


    virtual double getPricePerHour()
    {
        return m_pricePerHour;
    }


    virtual int getParkingTimeResolution()
    {
        return m_parkingTimeResolution;
    }


    virtual int getTimeLimit()
    {
        return m_timeLimit;
    }


    virtual Result verifyParkingEvent(const spark::ParkingEvent &event)
    {
        m_lastEvent = event;
        return m_verifyResult;
    }


    /**
     * @brief Bring mock back to its initial state.
     */
    void reset()
    {
        m_connectionOk = true;
        m_pricePerHour = 0;
        m_parkingTimeResolution = 0;
        m_timeLimit = 0;
        m_verifyResult = ICloudService::OK;
        m_lastEvent = spark::ParkingEvent("", "", 0, spark::PaymentToken("", ""));
    }
};


} // sparktest

#endif // CLOUDSERVICEMOCK_H
