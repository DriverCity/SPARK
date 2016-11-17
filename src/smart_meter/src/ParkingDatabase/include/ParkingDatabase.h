/**
 * @file
 * @brief Implementation for IParkingDatabase.
 */

#ifndef PARKINGDATABASE_H
#define PARKINGDATABASE_H

#include "IParkingDatabase.h"
#include <string>

class sqlite3;

namespace spark
{

/**
 * @brief Implementation for IParkingDatabase.
 */
class ParkingDatabase : IParkingDatabase
{
public:

    /**
     * @brief Constructor.
     * @param dbName SQLite database name.
     * @post Opens the database. If not successful, object becomes invalid.
     */
    ParkingDatabase(const std::string& dbName);

    virtual ~ParkingDatabase();

    // Implements IParkingDatabase.
    virtual bool addEvent(const ParkingEvent &event);
    virtual std::vector<ParkingEvent> getEvents();
    virtual bool clear();

    /**
     * @brief Check if object is in a valid state.
     * @return True, if database is open and ok.
     */
    bool isValid() const;

private:

    sqlite3* m_db;

    void createEventTableIfNotExist();
    void releaseDb();
};

} // spark

#endif // PARKINGDATABASE_H
