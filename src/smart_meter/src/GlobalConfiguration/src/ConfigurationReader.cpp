#include "ConfigurationReader.h"
#include "Logger/Logger.h"
#include <fstream>
#include <algorithm>
#include <cassert>

namespace spark
{

ConfigurationReader::ConfigurationReader()
{
}


ConfigurationReader::~ConfigurationReader()
{
}


ConfigurationReader::ConfigMap ConfigurationReader::readFile(const std::string& file) const
{
    std::ifstream ifs(file);
    if (!ifs){
        LOG_ERROR("Could not open configuration file: " << file);
        return ConfigMap();
    }

    ConfigMap conf = readConf(ifs);
    ifs.close();
    return conf;
}


ConfigurationReader::ConfigMap ConfigurationReader::readConf(std::istream& input) const
{
    assert(input && "Input stream is not in valid state.");

    ConfigMap conf;
    std::string row;
    while (std::getline(input, row)){
        // Skip empty and comments.
        if (row.empty() || row.at(0)=='#'){
            continue;
        }

        readRow(conf, row);
        if (conf.empty()){
            break;
        }
    }

    return conf;
}


void ConfigurationReader::readRow(ConfigurationReader::ConfigMap &conf,
                             const std::string &row) const
{
    assert((!row.empty() && row.at(0) != '#') &&
           "Remove comments and empty rows before reading actual rows." );

    if (std::count(row.begin(), row.end(), ';') != 1){
        LOG_ERROR("Invalid configuration: incorrect number of fields on a row.");
        conf = ConfigMap();
        return;
    }

    std::istringstream rowStream(row);
    std::string key;
    std::string value;
    std::getline(rowStream, key, ';');
    std::getline(rowStream, value);

    if (conf.find(key) != conf.end()){
        LOG_ERROR("Invalid configuration: conflicting configuration parameters: " << key);
        conf = ConfigMap();
    }
    else {
        conf[key] = value;
    }
}


} // spark
