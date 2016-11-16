/**
 * @file
 * @brief Defines mock-implementation for IBLEService.
 */

#ifndef BLESERVICEMOCK_H
#define BLESERVICEMOCK_H

#include "IBLEService.h"

namespace sparktest
{

/**
 * @brief Mock implementation for IBLEService. Could be used for unit testing other components.
 */
class BLEServiceMock : public spark::IBLEService
{
public:

    // Check mock's state from these variables.
    spark::IPriceProvider* m_priceProvider;
    spark::IVerifyParking* m_verifyParking;
    bool m_started;


    BLEServiceMock() :
        m_priceProvider(nullptr), m_verifyParking(nullptr), m_started(false)
    {
    }


    virtual ~BLEServiceMock()
    {
    }


    virtual void init(spark::IPriceProvider* priceProvider, spark::IVerifyParking* verifier)
    {
        m_priceProvider = priceProvider;
        m_verifyParking = verifier;
    }


    virtual void start()
    {
        m_started = true;
    }


    /**
     * @brief Revert mock to the initial state.
     */
    void reset()
    {
        m_priceProvider = nullptr;
        m_verifyParking = nullptr;
        m_started = false;
    }
};

} // sparktest

#endif // BLESERVICEMOCK_H
