
#include <gtest/gtest.h>
#include "ParkingDatabase.h"
#include <Logger/Logger.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <sqlite3.h>


#define COMPARE_EVENTS(expected, actual) \
    EXPECT_EQ(expected.registerNumber(), actual.registerNumber());\
    EXPECT_EQ(expected.startingTime(), actual.startingTime());\
    EXPECT_EQ(expected.duration(), actual.duration());\
    EXPECT_EQ(expected.token().verifier(), actual.token().verifier());\
    EXPECT_EQ(expected.token().uid(), actual.token().uid());


namespace
{

const std::string TEST_DB_NAME = "testdb.db";

bool fileExists(const std::string& fileName)
{
    struct stat buffer;
    return (stat (fileName.c_str(), &buffer) == 0);
}


int checkTableCallback(void*, int argc, char**, char**)
{
    if (argc != 1){
        return 0;
    }
    return 1;
}


bool checkEventsTableExists(const std::string& dbName)
{
    const char* sql = "select 1 from sqlite_master where type='table' and name='EVENTS';";
    sqlite3* db = 0;
    int result = sqlite3_open(dbName.c_str(), &db);
    if (result != 0){
        sqlite3_close(db);
        return false;
    }

    char* errorMsg = 0;
    result = sqlite3_exec(db, sql, checkTableCallback, 0, &errorMsg);
    sqlite3_close(db);
    return result != 0;
}

}


class ParkingDatabaseTest : public ::testing::Test
{
protected:

    void SetUp()
    {
        spark::Logger::init();
    }


    void TearDown()
    {
        if ( fileExists(TEST_DB_NAME) ){
            std::remove(TEST_DB_NAME.c_str());
        }
        spark::Logger::close();
    }
};



TEST_F(ParkingDatabaseTest, ConstructorTest)
{
    // Successful, db does not exist beforehand.
    {
        EXPECT_FALSE( fileExists(TEST_DB_NAME) );
        spark::ParkingDatabase db(TEST_DB_NAME);
        EXPECT_TRUE( db.isValid() );
        EXPECT_EQ( std::vector<spark::ParkingEvent>::size_type(0), db.getEvents().size());
        EXPECT_TRUE( fileExists(TEST_DB_NAME) );
        EXPECT_TRUE( checkEventsTableExists(TEST_DB_NAME) );
    }

    // Successful, db already exists,
    {
        EXPECT_TRUE( fileExists(TEST_DB_NAME) );
        spark::ParkingDatabase db(TEST_DB_NAME);
        EXPECT_TRUE( db.isValid() );
        EXPECT_TRUE( fileExists(TEST_DB_NAME) );
        EXPECT_TRUE( checkEventsTableExists(TEST_DB_NAME) );
    }

    // Failure
    {
        spark::ParkingDatabase db("dir_not_exist/testdb.db");
        EXPECT_FALSE(db.isValid());
    }
}


TEST_F (ParkingDatabaseTest, InvalidDbTest)
{
    spark::ParkingDatabase db("not_exist/testdb.db");
    EXPECT_FALSE(db.isValid());
    EXPECT_FALSE(db.addEvent(spark::ParkingEvent("ABC123", "2016-11-16 12:00", 30, spark::PaymentToken("verifier", "1234"))));
    EXPECT_EQ(std::vector<spark::ParkingEvent>::size_type(0), db.getEvents().size());
    EXPECT_FALSE(db.clear());
}


TEST_F (ParkingDatabaseTest, addEventSuccessTest)
{
    spark::ParkingDatabase db(TEST_DB_NAME);
    ASSERT_TRUE(db.isValid());

    spark::ParkingEvent e1("ABC123", "2016-11-16 12:00", 10, spark::PaymentToken("verifierX", "id123"));
    spark::ParkingEvent e2("ABC123", "2016-11-16 12:10", 20, spark::PaymentToken("verifierY", "id456"));
    spark::ParkingEvent e3("ABC123", "2016-11-16 12:20", 30, spark::PaymentToken("verifierZ", "id789"));

    EXPECT_TRUE( db.addEvent(e1) );
    EXPECT_TRUE( db.addEvent(e2) );
    EXPECT_TRUE( db.addEvent(e3) );

    // get events from db object
    std::vector<spark::ParkingEvent> events = db.getEvents();
    ASSERT_EQ( std::vector<spark::ParkingEvent>::size_type(3), events.size() );
    COMPARE_EVENTS(e1, events.at(0));
    COMPARE_EVENTS(e2, events.at(1));
    COMPARE_EVENTS(e3, events.at(2));
}


TEST_F (ParkingDatabaseTest, AddDuplicateEvent)
{
    spark::ParkingDatabase db(TEST_DB_NAME);
    ASSERT_TRUE(db.isValid());

    spark::ParkingEvent e1("ABC123", "2016-11-16 12:00", 10, spark::PaymentToken("verifierX", "id123"));
    EXPECT_TRUE( db.addEvent(e1) );
    EXPECT_EQ(std::vector<spark::ParkingEvent>::size_type(1), db.getEvents().size());
    EXPECT_FALSE( db.addEvent(e1) );
    EXPECT_EQ(std::vector<spark::ParkingEvent>::size_type(1), db.getEvents().size());
    COMPARE_EVENTS( e1, db.getEvents().at(0) );
}


TEST_F (ParkingDatabaseTest, EventsPreserveOverDestructor)
{
    spark::ParkingEvent e1("ABC123", "2016-11-16 12:00", 10, spark::PaymentToken("verifierX", "id123"));
    spark::ParkingEvent e2("ABC123", "2016-11-16 12:10", 20, spark::PaymentToken("verifierY", "id456"));
    spark::ParkingEvent e3("ABC123", "2016-11-16 12:20", 30, spark::PaymentToken("verifierZ", "id789"));

    {
        spark::ParkingDatabase db(TEST_DB_NAME);
        ASSERT_TRUE(db.isValid());
        EXPECT_TRUE( db.addEvent(e1) );
        EXPECT_TRUE( db.addEvent(e2) );
        EXPECT_TRUE( db.addEvent(e3) );
        std::vector<spark::ParkingEvent> events = db.getEvents();
        ASSERT_EQ( std::vector<spark::ParkingEvent>::size_type(3), events.size() );
        COMPARE_EVENTS(e1, events.at(0));
        COMPARE_EVENTS(e2, events.at(1));
        COMPARE_EVENTS(e3, events.at(2));
    } //Destructor

    { //Re-open
        spark::ParkingDatabase db(TEST_DB_NAME);
        ASSERT_TRUE(db.isValid());
        std::vector<spark::ParkingEvent> events = db.getEvents();
        ASSERT_EQ( std::vector<spark::ParkingEvent>::size_type(3), events.size() );
        COMPARE_EVENTS(e1, events.at(0));
        COMPARE_EVENTS(e2, events.at(1));
        COMPARE_EVENTS(e3, events.at(2));
    }
}


TEST_F (ParkingDatabaseTest, ClearTest)
{
    spark::ParkingDatabase db(TEST_DB_NAME);
    ASSERT_TRUE(db.isValid());

    spark::ParkingEvent e1("ABC123", "2016-11-16 12:00", 10, spark::PaymentToken("verifierX", "id123"));
    spark::ParkingEvent e2("ABC123", "2016-11-16 12:10", 20, spark::PaymentToken("verifierY", "id456"));
    spark::ParkingEvent e3("ABC123", "2016-11-16 12:20", 30, spark::PaymentToken("verifierZ", "id789"));

    EXPECT_TRUE( db.addEvent(e1) );
    EXPECT_TRUE( db.addEvent(e2) );
    EXPECT_TRUE( db.addEvent(e3) );

    std::vector<spark::ParkingEvent> events = db.getEvents();
    ASSERT_EQ( std::vector<spark::ParkingEvent>::size_type(3), events.size() );
    COMPARE_EVENTS(e1, events.at(0));
    COMPARE_EVENTS(e2, events.at(1));
    COMPARE_EVENTS(e3, events.at(2));

    db.clear();
    EXPECT_EQ(std::vector<spark::ParkingEvent>::size_type(0), db.getEvents().size());
}
