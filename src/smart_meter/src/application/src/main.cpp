#include <iostream>
#include "GlobalConfiguration/GlobalConfiguration.h"
#include "Logger/Logger.h"
#include <unistd.h>


std::string getConfigFilePath(const std::string& programName)
{
    std::string path = programName.substr(0, programName.find_last_of("/"));
    return path + "/../config/global.txt";
}


int main(int argc, char** argv)
{
    spark::Logger::init();
    std::string configDir = getConfigFilePath(argv[0]);
    spark::GlobalConfiguration::init(configDir);
    LOG_DEBUG(spark::GlobalConfiguration::instance()->getValue("greetings"));
    return 0;
}
