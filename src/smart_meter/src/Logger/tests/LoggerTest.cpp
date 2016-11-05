/**
 * @file
 * @brief Unit tests for spark::Logger.
 */

#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "../include/Logger.h"


// Print single line.
TEST (LoggerTest, PrintTest)
{
    std::ostringstream oss;
    spark::Logger::init(spark::Logger::NO_LOG_FILE, oss);
    spark::Logger::write("Hello");
    spark::Logger::close();

    EXPECT_EQ("Hello\n", oss.str());
}


// Print and log multiple lines.
TEST (LoggerTest, WriteLogTest)
{
    std::ostringstream oss;
    const std::string file = "testlog.log";
    spark::Logger::init(file, oss);
    spark::Logger::write("Hello");
    spark::Logger::write("World!");
    spark::Logger::close();

    const std::string expected = "Hello\nWorld!\n";
    EXPECT_EQ(expected, oss.str());

    // Check log file
    std::ifstream ifs(file);
    EXPECT_TRUE(ifs);
    std::string result;
    std::string tmp;
    while(std::getline(ifs, tmp)){
        result += tmp;
        result += "\n"; // std::getline strips separator.
    }

    EXPECT_EQ(expected, result);
}


// Test LOG_DEBUG convenience macro.
TEST (LoggerTest, LogDebugTest)
{
    std::ostringstream oss;
    spark::Logger::init(spark::Logger::NO_LOG_FILE, oss);

    const int REFERENCE = __LINE__;
    LOG_DEBUG("Hello World!");
    spark::Logger::close();

    std::ostringstream expected;
    expected << "Debug: LoggerTest.cpp " << REFERENCE+1 << ": Hello World!\n";
    EXPECT_EQ(expected.str(), oss.str());
}


// Test LOG_ERROR convenience macro.
TEST (LoggerTest, LogErrorTest)
{
    std::ostringstream oss;
    spark::Logger::init(spark::Logger::NO_LOG_FILE, oss);

    const int REFERENCE = __LINE__;
    LOG_ERROR("Hello World!");
    spark::Logger::close();

    std::ostringstream expected;
    expected << "Error: LoggerTest.cpp " << REFERENCE+1 << ": Hello World!\n";
    EXPECT_EQ(expected.str(), oss.str());
}


// Test LOG_DEBUG convenience macro using stream notation.
TEST (LoggerTest, LogDebugStream)
{
    std::ostringstream oss;
    spark::Logger::init(spark::Logger::NO_LOG_FILE, oss);

    const int REFERENCE = __LINE__;
    LOG_DEBUG("Hello" << " " << "World!");
    spark::Logger::close();

    std::ostringstream expected;
    expected << "Debug: LoggerTest.cpp " << REFERENCE+1 << ": Hello World!\n";
    EXPECT_EQ(expected.str(), oss.str());
}
