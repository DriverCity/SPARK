/**
 * @file
 * @brief Unit tests for spark::Logger.
 */

#include <gtest/gtest.h>
#include <sstream>
#include <fstream>
#include "Logger.h"


class LoggerTest : public ::testing::Test
{
protected:

    std::ostringstream m_oss;

    void SetUp()
    {

    }

    void TearDown()
    {
        m_oss.clear();
    }
};

// Print single line.
TEST_F (LoggerTest, PrintTest)
{
    spark::Logger::init(spark::Logger::NO_LOG_FILE, m_oss);
    spark::Logger::write("Hello");
    spark::Logger::close();

    EXPECT_EQ("Hello\n", m_oss.str());
}


// Test invalid log file (should crash for assertion.
TEST_F (LoggerTest, LogFileDoesNotOpen)
{
    spark::Logger::init("not_a_dir/log.log", m_oss);
    EXPECT_EQ("ERROR: logfile 'not_a_dir/log.log' did not open!\n", m_oss.str());
}


// Print and log multiple lines.
TEST_F (LoggerTest, WriteLogTest)
{
    const std::string file = "testlog.log";
    spark::Logger::init(file, m_oss);
    spark::Logger::write("Hello");
    spark::Logger::write("World!");
    spark::Logger::close();

    const std::string expected = "Hello\nWorld!\n";
    EXPECT_EQ(expected, m_oss.str());

    // Check log file
    std::ifstream ifs(file);
    EXPECT_TRUE((bool)ifs);
    std::string result;
    std::string tmp;
    while(std::getline(ifs, tmp)){
        result += tmp;
        result += "\n"; // std::getline strips separator.
    }

    EXPECT_EQ(expected, result);
}


// Test LOG_DEBUG convenience macro.
TEST_F (LoggerTest, LogDebugTest)
{
    spark::Logger::init(spark::Logger::NO_LOG_FILE, m_oss);

    const int REFERENCE = __LINE__;
    LOG_DEBUG("Hello World!");
    spark::Logger::close();

    std::ostringstream expected;
    expected << "Debug: [LoggerTest] LoggerTest.cpp " << REFERENCE+1 << ": Hello World!\n";
    EXPECT_EQ(expected.str(), m_oss.str());
}


// Test LOG_ERROR convenience macro.
TEST_F (LoggerTest, LogErrorTest)
{
    spark::Logger::init(spark::Logger::NO_LOG_FILE, m_oss);

    const int REFERENCE = __LINE__;
    LOG_ERROR("Hello World!");
    spark::Logger::close();

    std::ostringstream expected;
    expected << "Error: [LoggerTest] LoggerTest.cpp " << REFERENCE+1 << ": Hello World!\n";
    EXPECT_EQ(expected.str(), m_oss.str());
}


// Test LOG_DEBUG convenience macro using stream notation.
TEST_F (LoggerTest, LogDebugStream)
{
    std::ostringstream m_oss;
    spark::Logger::init(spark::Logger::NO_LOG_FILE, m_oss);

    const int REFERENCE = __LINE__;
    LOG_DEBUG("Hello" << " " << "World!");
    spark::Logger::close();

    std::ostringstream expected;
    expected << "Debug: [LoggerTest] LoggerTest.cpp " << REFERENCE+1 << ": Hello World!\n";
    EXPECT_EQ(expected.str(), m_oss.str());
}
