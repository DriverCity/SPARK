
#include <csignal>
#include <iostream>
#include <atomic>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <Logger/Logger.h>
#include <GlobalConfiguration/GlobalConfiguration.h>

#define PROMPT ">> "

std::atomic_bool stop(false);


void signalHandler(int)
{
    LOG_DEBUG("Quiting...");
    stop.store(true);
}

void instructions()
{
    std::cout << "Type message to be sent as a fake BLE message." << std::endl;
    std::cout << std::endl;
}

std::string getBinaryPath(const std::string& argv0)
{
    std::string path = argv0.substr(0, argv0.find_last_of("/"));
    return path;
}

std::string getConfigFile(const std::string& argv0)
{
    return getBinaryPath(argv0) + "/../config/global.txt";
}


int main(int argc, char* argv[])
{
    signal(SIGABRT, signalHandler);
    signal(SIGINT, signalHandler);
    spark::GlobalConfiguration::init(getConfigFile(argv[0]));
    spark::GlobalConfiguration* config = spark::GlobalConfiguration::instance();
    std::string binaryDir = getBinaryPath(argv[0]);
    std::string output = binaryDir + "/" + config->getValue("BLEInputFifo");
    std::string response = binaryDir + "/" + config->getValue("BLEResponseFifo");

    instructions();
    LOG_DEBUG("Output fifo: " << output);
    LOG_DEBUG("Response fifo " << response);

    while (stop.load() == false)
    {
        // Wait for message.
        std::cout << PROMPT;
        std::string msg;
        std::getline(std::cin, msg);
        if (std::cin.peek() == EOF){
            LOG_DEBUG("Quting!");
            break;
        }

        //Send message
        FILE* outFd = fopen(output.c_str(), "w");
        if (outFd == NULL){
            LOG_ERROR("Fifo '" << output << "' does not exist.");
            return 1;
        }
        msg += "\n";
        fputs(msg.c_str(), outFd);
        fclose(outFd);

        // Read response.
        int respFd = open(response.c_str(), O_RDONLY);
        char buffer[100];
        read(respFd, buffer, 100);
        std::cout << std::string(buffer) << std::endl;
    }

    return 0;
}
