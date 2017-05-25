/**
 * @file
 * @brief Define interface for temporary parking event database.
 */

#ifndef IPARKINGDATABASE_H
#define IPARKINGDATABASE_H

#include "misc/ParkingEvent.h"
#include <vector>

namespace spark
{

/**
 * @brief Abstract interface for storing events to database and retriving them.
 */
class IParkingDatabase
{
public:
    virtual ~IParkingDatabase() {}

    /**
     * @brief Add event to the database.
     * @param event Event to be stored.
     * @return True, if event was stored successfully.
     */
    virtual bool addEvent(const ParkingEvent& event) = 0;

    /**
     * @brief Get all stored events.
     * @return Vector containing all currently stored events.
     *  Return an empty vector if an error occurs.
     */
    virtual std::vector<ParkingEvent> getEvents() = 0;

    /**
     * @brief Clear database.
     * @return True, if database was cleared successfully.
     */
    virtual bool clear() = 0;


    /**
     * @brief isValid Checks if database is in valid state.
     * @return true if database in valid state, false if not.
     */
    virtual bool isValid() const = 0;
};

} // spark

#endif // IPARKINGDATABASE_H
