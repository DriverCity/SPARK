/**
 * @file
 * @brief Defines mock implementation for IPriceProvider
 */

#ifndef PRICEPROVIDERMOCK_H
#define PRICEPROVIDERMOCK_H

#include "IPriceProvider.h"
#include <gmock/gmock.h>

namespace sparktest
{

class PriceProviderMock : public spark::IPriceProvider
{
public:

    MOCK_METHOD1(init, void(spark::ICloudService* cloud));
    MOCK_METHOD0(getPriceInfo, spark::PriceInfo());
};

} // sparktest

#endif // PRICEPROVIDERMOCK_H
