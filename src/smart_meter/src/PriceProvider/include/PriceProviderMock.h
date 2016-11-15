/**
 * @file
 * @brief Defines mock implementation for IPriceProvider
 */

#ifndef PRICEPROVIDERMOCK_H
#define PRICEPROVIDERMOCK_H

#include "IPriceProvider.h"

namespace sparktest
{

/**
 * @brief Mock implementation for IPriceProvider.
 *  This can be used in unit tests for other components.
 */
class PriceProviderMock : public spark::IPriceProvider
{
public:

    // Use these to control mock behaviour.
    spark::ICloudService* m_cloudService;
    spark::PriceInfo m_info;

    PriceProviderMock():
        m_cloudService(nullptr),
        m_info()
    {
    }

    virtual ~PriceProviderMock()
    {
    }


    virtual void init(spark::ICloudService* cloudService)
    {
        m_cloudService = cloudService;
    }

    virtual spark::PriceInfo getPriceInfo()
    {
        return m_info;
    }


    /**
     * @brief Reset mock back to initial state.
     */
    void reset()
    {
        m_cloudService = nullptr;
        m_info = spark::PriceInfo();
    }

};

} // sparktest

#endif // PRICEPROVIDERMOCK_H
