/**
 * @file
 * @brief Defines mock implementation for IVerifyParking.
 */

#ifndef VERIFYPARKINGMOCK_H
#define VERIFYPARKINGMOCK_H

#include "IVerifyParking.h"
#include <gmock/gmock.h>

namespace sparktest
{

class VerifyParkingMock : public spark::IVerifyParking
{
public:

    MOCK_METHOD1(init, void(spark::ICloudService* cloud));
    MOCK_METHOD1(verify, spark::IVerifyParking::Result(const spark::ParkingEvent& e));
};

} // sparktest

#endif // VERIFYPARKINGMOCK_H
