
#include "GlobalConfiguration.h"
#include "ConfigurationReader.h"
#include "Logger/Logger.h"


namespace spark
{

std::unique_ptr<GlobalConfiguration> GlobalConfiguration::instance_(nullptr);


GlobalConfiguration::~GlobalConfiguration()
{
}


void GlobalConfiguration::init(const std::string& confFile)
{
    ConfigurationReader::ConfigMap conf = ConfigurationReader().readFile(confFile);
    if (conf.empty()){
        LOG_ERROR("Invalid configuration.");
        instance_.reset();
        return;
    }

    instance_.reset(new GlobalConfiguration(conf));
}


GlobalConfiguration *GlobalConfiguration::instance()
{
    return instance_.get();
}


bool GlobalConfiguration::hasKey(const std::string& key) const
{
    return m_conf.find(key) != m_conf.end();
}


std::string GlobalConfiguration::getValue(const std::string& key, bool* ok) const
{
    bool found = hasKey(key);
    if (ok != nullptr){
        *ok = found;
    }

    if (found){
        return m_conf.at(key);
    }
    return "";
}


void GlobalConfiguration::insert(const std::string& key, const std::string& value)
{
    m_conf[key] = value;
}


GlobalConfiguration::GlobalConfiguration(const std::map<std::string, std::string> &conf):
    m_conf(conf)
{
}

} // spark
