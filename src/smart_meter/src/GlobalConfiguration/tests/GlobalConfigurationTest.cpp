
#include <gtest/gtest.h>
#include "GlobalConfiguration.h"
#include "Logger/Logger.h"
#include <sstream>

using namespace spark;

class GlobalConfgurationTest : public ::testing::Test
{
protected:

    std::ostringstream m_oss;

    void SetUp()
    {
        Logger::init();
    }


    void TearDown()
    {
        Logger::close();
    }
};



TEST_F(GlobalConfgurationTest, UninitializedTest)
{
    EXPECT_TRUE(GlobalConfiguration::instance() == nullptr);
}


TEST_F(GlobalConfgurationTest, InitValid)
{
    std::string testFile = std::string(TEST_DATA_DIR) + "/validConfig.txt";
    GlobalConfiguration::init(testFile);
    GlobalConfiguration* inst = GlobalConfiguration::instance();
    ASSERT_TRUE(inst != nullptr);
}


TEST_F(GlobalConfgurationTest, InitInvalid)
{
    std::string testFile = std::string(TEST_DATA_DIR) + "/invalidConfig.txt";
    GlobalConfiguration::init(testFile);
    EXPECT_TRUE(GlobalConfiguration::instance() == nullptr);
}


TEST_F(GlobalConfgurationTest, HasKeyTest)
{
    std::string testFile = std::string(TEST_DATA_DIR) + "/validConfig.txt";
    GlobalConfiguration::init(testFile);
    GlobalConfiguration* inst = GlobalConfiguration::instance();

    EXPECT_TRUE(inst->hasKey("paramFoo"));
    EXPECT_TRUE(inst->hasKey("paramBar"));
    EXPECT_TRUE(inst->hasKey("paramBaz"));
    EXPECT_FALSE(inst->hasKey("not_a_key"));
}


TEST_F(GlobalConfgurationTest, GetValueTest)
{
    std::string testFile = std::string(TEST_DATA_DIR) + "/validConfig.txt";
    GlobalConfiguration::init(testFile);
    GlobalConfiguration* inst = GlobalConfiguration::instance();

    bool ok = true;
    EXPECT_EQ("foo", inst->getValue("paramFoo", &ok));
    EXPECT_TRUE(ok);
    EXPECT_EQ("foo", inst->getValue("paramFoo"));

    EXPECT_EQ("bar", inst->getValue("paramBar", &ok));
    EXPECT_TRUE(ok);
    EXPECT_EQ("foo", inst->getValue("paramFoo"));

    EXPECT_EQ("baz", inst->getValue("paramBaz", &ok));
    EXPECT_TRUE(ok);
    EXPECT_EQ("foo", inst->getValue("paramFoo"));

    EXPECT_EQ("", inst->getValue("not_a_key", &ok));
    EXPECT_FALSE(ok);
    EXPECT_EQ("", inst->getValue("not_a_key"));
}


TEST_F(GlobalConfgurationTest, InsertTest)
{
    std::string testFile = std::string(TEST_DATA_DIR) + "/validConfig.txt";
    GlobalConfiguration::init(testFile);
    GlobalConfiguration* inst = GlobalConfiguration::instance();

    EXPECT_FALSE(inst->hasKey("newKey"));
    inst->insert("newKey", "newValue");
    EXPECT_TRUE(inst->hasKey("newKey"));
    EXPECT_EQ("newValue", inst->getValue("newKey"));

    inst->insert("newKey", "otherValue");
    EXPECT_EQ("otherValue", inst->getValue("newKey"));
}


