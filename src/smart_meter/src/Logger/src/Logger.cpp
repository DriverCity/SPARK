
#include "Logger.h"
#include <iostream>
#include <fstream>

namespace spark
{

const std::string Logger::DEBUG_MSG = "Debug: ";
const std::string Logger::ERROR_MSG = "Error: ";
const std::string Logger::NO_LOG_FILE = "";

std::unique_ptr<std::ostream> Logger::fileStream_ = std::unique_ptr<std::ostream>(nullptr);
std::ostream* Logger::printStream_ = &std::cout;


void Logger::init(const std::string& logFile, std::ostream& output)
{
    printStream_ = &output;

    if (logFile != NO_LOG_FILE){
        fileStream_.reset(new std::ofstream(logFile));
        if( !(*fileStream_) ){
            *printStream_ << "ERROR: logfile '" << logFile << "' did not open!" << std::endl;
        }
    }
}


void Logger::write(const std::string& msg)
{
    *Logger::printStream_ << msg << std::endl;
    if (fileStream_ != nullptr){
        *fileStream_ << msg << std::endl;
    }
}


void Logger::close()
{
    fileStream_.reset();
    printStream_ = &std::cout;
}

}
