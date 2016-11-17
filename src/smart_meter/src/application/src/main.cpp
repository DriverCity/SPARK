#include <iostream>
#include "GlobalConfiguration/GlobalConfiguration.h"
#include "Logger/Logger.h"
#include "ParkingDatabase/ParkingDatabase.h"


std::string getConfigFilePath(const std::string& programName)
{
    std::string path = programName.substr(0, programName.find_last_of("/"));
    return path + "/../config";
}


int main(int argc, char** argv)
{
    std::string configDir = getConfigFilePath(argv[0]);
    spark::GlobalConfiguration::init(configDir + "/global.txt");
    LOG_DEBUG(spark::GlobalConfiguration::instance()->getValue("greetings"));

    spark::ParkingDatabase db(configDir + "/testdb.db");

    return 0;
}
