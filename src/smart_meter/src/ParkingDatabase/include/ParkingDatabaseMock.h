/**
 * @file
 * @brief Defines mock implementation for IParkingDatabase.
 */

#ifndef PARKINGDATABASEMOCK_H
#define PARKINGDATABASEMOCK_H

#include "IParkingDatabase.h"

namespace sparktest
{

/**
 * @brief Mock implementation for IParkingDatabase.
 *  This class can be used in unit tests for other components.
 */
class ParkingDatabaseMock : public spark::IParkingDatabase
{
public:

    // Use these to controll mock behaviour and check state.
    std::vector<spark::ParkingEvent> m_storedEvents;
    std::vector<spark::ParkingEvent> m_addedEvents;
    bool m_addEventResult;
    bool m_getEventsResult;
    bool m_clearResult;

    ParkingDatabaseMock():
        m_storedEvents(),
        m_addedEvents(),
        m_addEventResult(true),
        m_getEventsResult(true),
        m_clearResult(true)
    {
    }


    virtual ~ParkingDatabaseMock()
    {
    }


    virtual bool addEvent(const spark::ParkingEvent& event)
    {
        m_addedEvents.push_back(event);
        if (m_addEventResult){
            m_storedEvents.push_back(event);
        }
        return m_addEventResult;
    }


    virtual std::vector<spark::ParkingEvent> getEvents()
    {
        if (m_getEventsResult){
            return m_storedEvents;
        }
        return std::vector<spark::ParkingEvent>();
    }


    virtual bool clear()
    {
        if (m_clearResult){
            m_storedEvents.clear();
        }
        return m_clearResult;
    }


    /**
     * @brief Set mock back to its initial state.
     */
    void reset()
    {
        m_storedEvents.clear();
        m_addedEvents.clear();
        m_addEventResult = true;
        m_getEventsResult = true;
        m_clearResult = true;
    }
};

} // sparktest

#endif PARKINGDATABASEMOCK_H
