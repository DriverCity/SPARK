/**
 * @file
 * @brief Defines mock implementation for IParkingDatabase.
 */

#ifndef PARKINGDATABASEMOCK_H
#define PARKINGDATABASEMOCK_H

#include "IParkingDatabase.h"
#include <gmock/gmock.h>

namespace sparktest
{


class ParkingDatabaseMock : public spark::IParkingDatabase
{
public:

    MOCK_METHOD1(addEvent, bool(const spark::ParkingEvent& e));
    MOCK_METHOD0(getEvents, std::vector<spark::ParkingEvent>());
    MOCK_METHOD0(clear, bool());
    MOCK_CONST_METHOD0(isValid, bool());

};

}

#endif //PARKINGDATABASEMOCK_H
