#include <gtest/gtest.h>
#include "ConfigurationReader.h"
#include "Logger/Logger.h"
#include <sstream>

namespace
{

// Check that 'input' starts with 'start' and ends with 'end'
bool containsMsg(const std::string& input,
                 const std::string start,
                 const std::string& end)
{
    if (input.length() < start.length() + end.length()){
        std::cout << "Actual message: " << input << std::endl;
        return false;
    }

    std::istringstream iss(input);
    std::string tmp;
    while(std::getline(iss, tmp)){
        if (tmp.substr(0, start.length()) == start){
            if (tmp.substr(tmp.length()-end.length()) == end){
                return true;
            }
        }
    }
    std::cout << "Actual message: " << input << std::endl;
    return false;
}

}


class ConfigurationReaderTest : public ::testing::Test
{
protected:

    std::ostringstream m_oss;
    spark::ConfigurationReader m_reader;

    void SetUp()
    {
        spark::Logger::init(spark::Logger::NO_LOG_FILE, m_oss);
    }


    void TearDown()
    {
        spark::Logger::close();
        m_oss.str("");
        m_oss.clear();
    }
};



TEST_F (ConfigurationReaderTest, ReadValidRow)
{
    spark::ConfigurationReader::ConfigMap conf;
    m_reader.readRow(conf, "paramFoo:Foo\n");

    EXPECT_EQ(spark::ConfigurationReader::ConfigMap::size_type(1), conf.size());
    ASSERT_TRUE(conf.find("paramFoo") != conf.end());
    EXPECT_EQ("Foo", conf["paramFoo"]);
}


TEST_F (ConfigurationReaderTest, TooManyFieldsOnRow)
{
    spark::ConfigurationReader::ConfigMap conf;
    m_reader.readRow(conf, "paramFoo:Foo:other");

    EXPECT_EQ(spark::ConfigurationReader::ConfigMap::size_type(0), conf.size());
    // Check error message
    spark::Logger::close();
    EXPECT_TRUE( containsMsg(m_oss.str(), "Error: [GlobalConfigurationTest] ConfigurationReader.cpp ",
                             ": Invalid configuration: incorrect number of fields on a row.") );
}


TEST_F (ConfigurationReaderTest, TooFewFieldsOnRow)
{
    spark::ConfigurationReader::ConfigMap conf;
    m_reader.readRow(conf, "paramFoo");

    EXPECT_EQ(spark::ConfigurationReader::ConfigMap::size_type(0), conf.size());
    // Check error message
    spark::Logger::close();
    EXPECT_TRUE( containsMsg(m_oss.str(), "Error: [GlobalConfigurationTest] ConfigurationReader.cpp ",
                             ": Invalid configuration: incorrect number of fields on a row.") );
}


TEST_F (ConfigurationReaderTest, ReadValidStream)
{
    std::string inputString = "\n"
                              "# Comment.\n"
                              "paramFoo:Foo\n"
                              "paramBar:Bar\n";
    std::istringstream inputStream(inputString);

    spark::ConfigurationReader::ConfigMap conf = m_reader.readConf(inputStream);

    EXPECT_TRUE(inputStream.peek() == EOF);
    EXPECT_EQ(spark::ConfigurationReader::ConfigMap::size_type(2), conf.size());
    ASSERT_TRUE(conf.find("paramFoo") != conf.end());
    ASSERT_TRUE(conf.find("paramBar") != conf.end());
    EXPECT_EQ("Foo", conf["paramFoo"]);
    EXPECT_EQ("Bar", conf["paramBar"]);
}


TEST_F (ConfigurationReaderTest, ReadInvalidStream)
{
    std::string inputString = "\n"
                              "# Comment.\n"
                              "paramFoo:Foo:afddsf\n"
                              "paramBar:Bar\n";

    std::istringstream inputStream(inputString);
    spark::ConfigurationReader::ConfigMap conf = m_reader.readConf(inputStream);

    EXPECT_FALSE(inputStream.peek() == EOF);
    EXPECT_EQ(spark::ConfigurationReader::ConfigMap::size_type(0), conf.size());

    // Check error message
    spark::Logger::close();
    EXPECT_TRUE( containsMsg(m_oss.str(), "Error: [GlobalConfigurationTest] ConfigurationReader.cpp ",
                             ": Invalid configuration: incorrect number of fields on a row.") );
}


TEST_F (ConfigurationReaderTest, EmpryStream)
{
    std::istringstream inputStream;
    spark::ConfigurationReader::ConfigMap conf = m_reader.readConf(inputStream);

    EXPECT_TRUE(inputStream.peek() == EOF);
    EXPECT_EQ(spark::ConfigurationReader::ConfigMap::size_type(0), conf.size());

    // Check no error messages
    spark::Logger::close();
    EXPECT_EQ("", m_oss.str());
}


TEST_F (ConfigurationReaderTest, ReadValidFile)
{
    spark::ConfigurationReader::ConfigMap conf = m_reader.readFile("data/validConfig.txt");

    EXPECT_EQ(spark::ConfigurationReader::ConfigMap::size_type(3), conf.size());

    ASSERT_TRUE(conf.find("paramFoo") != conf.end());
    EXPECT_EQ("foo", conf["paramFoo"]);

    ASSERT_TRUE(conf.find("paramBar") != conf.end());
    EXPECT_EQ("bar", conf["paramBar"]);

    ASSERT_TRUE(conf.find("paramBaz") != conf.end());
    EXPECT_EQ("baz", conf["paramBaz"]);
}


TEST_F (ConfigurationReaderTest, FileDoesNotOpen)
{
    spark::ConfigurationReader::ConfigMap conf = m_reader.readFile("DoesNotExist.txt");
    EXPECT_TRUE(conf.empty());

    // Check error message
    spark::Logger::close();
    EXPECT_TRUE( containsMsg(m_oss.str(), "Error: [GlobalConfigurationTest] ConfigurationReader.cpp ",
                             ": Could not open configuration file: DoesNotExist.txt") );
}


TEST_F (ConfigurationReaderTest, ReadInvalidFile)
{
    spark::ConfigurationReader::ConfigMap conf = m_reader.readFile("data/invalidConfig.txt");

    EXPECT_EQ(spark::ConfigurationReader::ConfigMap::size_type(0), conf.size());
    // Check error message
    spark::Logger::close();
    EXPECT_TRUE( containsMsg(m_oss.str(), "Error: [GlobalConfigurationTest] ConfigurationReader.cpp ",
                             ": Invalid configuration: conflicting configuration parameters: paramFoo"));

}
