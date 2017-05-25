/**
 * @file
 * @brief Defined mock implementation for the cloud service.
 */

#ifndef CLOUDSERVICEMOCK_H
#define CLOUDSERVICEMOCK_H

#include "ICloudService.h"
#include <gmock/gmock.h>

namespace sparktest
{

class CloudServiceMock : public spark::ICloudService
{
public:

    MOCK_METHOD3(init, void(int areaId, const std::string& priceAPI, const std::string& eventAPI));
    MOCK_METHOD0(checkConnection, bool());
    MOCK_METHOD2(getPriceInformation, bool(double& price, int& timeLimit));
    MOCK_METHOD1(verifyParkingEvent, spark::ICloudService::Result(const spark::ParkingEvent& e));
};


} // sparktest

#endif // CLOUDSERVICEMOCK_H
