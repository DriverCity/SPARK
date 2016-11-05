
#include "Logger.h"
#include <cassert>
#include <iostream>
#include <fstream>

namespace spark
{

const std::string Logger::DEBUG_MSG = "Debug: ";
const std::string Logger::ERROR_MSG = "Error: ";
const std::string Logger::NO_LOG_FILE = "";

std::unique_ptr<std::ostream> Logger::fileStream_ = std::unique_ptr<std::ostream>(nullptr);
std::ostream* Logger::printStream_ = nullptr;


void Logger::init(const std::string& logFile, std::ostream& output)
{
    assert(printStream_ == nullptr &&
           "Initialize Logger only once!");

    printStream_ = &output;

    if (logFile != NO_LOG_FILE){
        fileStream_.reset(new std::ofstream(logFile));
        if( !(*fileStream_) ){
            *printStream_ << "FATAL: Opening log file " << logFile << " failed!" << std::endl;
            assert(*fileStream_ && "Opening log file failed!");
        }
    }
}


void Logger::write(const std::string& msg)
{
    assert(printStream_ != nullptr &&
           "Logger must be initialized before writing.");

    *Logger::printStream_ << msg << std::endl;
    if (fileStream_ != nullptr){
        *fileStream_ << msg << std::endl;
    }
}


void Logger::close()
{
    fileStream_.reset();
    printStream_ = nullptr;
}

}
