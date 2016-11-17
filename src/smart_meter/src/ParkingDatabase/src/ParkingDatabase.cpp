#include "ParkingDatabase.h"
#include "Logger/Logger.h"
#include <sstream>
#include <cassert>
#include <sqlite3.h>

namespace
{

const char* CREATE_TABLE_QUERY =
        "CREATE TABLE IF NOT EXISTS EVENTS( "
        "REGISTER_NUMBER    TEXT    NOT NULL, "
        "STARTING_TIME      TEXT    NOT NULL, "
        "DURATION           INT     NOT_NULL, "
        "TOKEN_VERIFIER     TEXT    NOT_NULL, "
        "TOKEN_UID          TEXT    NOT_NULL, "
        "UNIQUE (REGISTER_NUMBER, STARTING_TIME, DURATION, TOKEN_VERIFIER, TOKEN_UID) );";

const char* GET_EVENTS_QUERY = "SELECT * FROM EVENTS;";

const char* CLEAR_QUERY = "DELETE FROM EVENTS;";

// Events fetched from db will be stored here temporarily.
std::vector<spark::ParkingEvent> fetchedEvents;

// Callback function for get events query.
int getEventsCallback(void*, int argc, char** argv, char**)
{
    assert(argc == 5);

    std::string regNum = argv[0];
    std::string startingTime = argv[1];
    int duration = std::stoi(argv[2]);
    std::string token_verifier = argv[3];
    std::string token_uid = argv[4];

    spark::ParkingEvent event(regNum, startingTime, duration, spark::PaymentToken(token_verifier, token_uid));
    fetchedEvents.push_back(event);
    return 0;
}


}

namespace spark
{


ParkingDatabase::ParkingDatabase(const std::string &dbName) :
    m_db(nullptr)
{
    int result = sqlite3_open(dbName.c_str(), &m_db);
    if (result != 0){
        if (m_db == nullptr){
            LOG_ERROR("Opening database '" << dbName << "' failed: could not allocate memory.");
        }
        else {
            LOG_ERROR("Opening database '" << dbName << "' failed.");
            releaseDb();
        }
    }
    else {
        LOG_DEBUG("Database '" << dbName << "' opened successfully.");
        createEventTableIfNotExist();
    }
}


ParkingDatabase::~ParkingDatabase()
{
    releaseDb();
}


bool ParkingDatabase::addEvent(const ParkingEvent &event)
{
    if (!isValid()){
        LOG_ERROR("Cannot add events: db is in invalid state.");
        return false;
    }

    std::ostringstream oss;
    oss << "INSERT INTO EVENTS VALUES("
        << "'" << event.registerNumber() << "', "
        << "'" << event.startingTime() << "', "
        << event.duration() << ", "
        << "'" << event.token().verifier() << "', "
        << "'" << event.token().uid() << "'"
        << ");";

    char* errorStr = nullptr;
    int result = sqlite3_exec(m_db, oss.str().c_str(), NULL, NULL, &errorStr);

    if (result != 0){
        LOG_ERROR("Adding event failed: " << errorStr);
    }
    else {
        LOG_DEBUG("Event added successfully.");
    }
    return result == 0;
}


std::vector<ParkingEvent> ParkingDatabase::getEvents()
{
    std::vector<ParkingEvent> rval;

    if (!isValid()){
        LOG_ERROR("Cannot get events: db is in an invalid state.");
        return rval;
    }

    fetchedEvents.clear();
    char* errorMsg = nullptr;
    int result = sqlite3_exec(m_db, GET_EVENTS_QUERY, getEventsCallback, NULL, &errorMsg);

    if (result != 0){
        LOG_ERROR("Failed to get events: " << errorMsg);
    }
    else {
        LOG_DEBUG("Successfully fetched " << fetchedEvents.size() << " events.");
        rval = fetchedEvents;
    }

    fetchedEvents.clear();
    return rval;
}


bool ParkingDatabase::clear()
{
    if (!isValid()){
        LOG_ERROR("Cannot clear events: db is in an invalid state.");
        return false;
    }

    char* errorMsg = nullptr;
    int result = sqlite3_exec(m_db, CLEAR_QUERY, NULL, NULL, &errorMsg);

    if (result != 0){
        LOG_ERROR("Could not clear events: " << errorMsg);
    }
    else {
        LOG_DEBUG("All events cleared successfully.");
    }
    return result == 0;
}


bool ParkingDatabase::isValid() const
{
    return m_db != nullptr;
}


void ParkingDatabase::createEventTableIfNotExist()
{
    char* errorMsg = nullptr;
    int result = sqlite3_exec(m_db, CREATE_TABLE_QUERY, NULL, NULL, &errorMsg);
    if (result != 0){
        LOG_ERROR("Creating events table failed: " << errorMsg);
        releaseDb();
    }
    else {
        LOG_DEBUG("Events table created successfully.");
    }
}


void ParkingDatabase::releaseDb()
{
    if (m_db != nullptr){
        sqlite3_close(m_db);
        m_db = nullptr;
    }
}


} // spark
