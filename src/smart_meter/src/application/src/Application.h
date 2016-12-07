#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <memory>
#include <atomic>
#include "BLEService/IBLEService.h"
#include "PriceProvider/IPriceProvider.h"
#include "VerifyParking/IVerifyParking.h"
#include "CloudService/ICloudService.h"

namespace spark
{

class Application
{
public:

    struct Setup
    {
        std::unique_ptr<IBLEService> bleService;
        std::unique_ptr<IPriceProvider> priceProvider;
        std::unique_ptr<IVerifyParking> verifier;
        std::unique_ptr<ICloudService> cloudService;
        std::unique_ptr<IParkingDatabase> parkingDb;
    };


    Application(Setup&& setup);

    ~Application();

    static std::string getBinaryDir(const std::string& argv0);

    static std::string getConfigDir(const std::string& argv0);

    int exec();

    static void stop();

private:

    Setup m_setup;
    static std::atomic_bool m_stop;
};

}

#endif // APPLICATION_H
