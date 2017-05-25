/**
 * @file
 * @brief Defines mock-implementation for IBLEService.
 */

#ifndef BLESERVICEMOCK_H
#define BLESERVICEMOCK_H

#include "IBLEService.h"
#include <gmock/gmock.h>

namespace sparktest
{

class BLEServiceMock : public spark::IBLEService
{
public:

    MOCK_METHOD2(init, void(spark::IPriceProvider* priceProvider, spark::IVerifyParking* verifier));
    MOCK_METHOD0(start, void());
    MOCK_METHOD0(stop, void());
};

}

#endif // BLESERVICEMOCK_H
