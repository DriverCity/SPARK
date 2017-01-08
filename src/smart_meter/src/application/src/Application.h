/**
 * @file
 * @brief Defines the Application class.
 */

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

/**
 * @brief The smart meter application.
 */
class Application
{
public:

    /**
     * @brief Setup parameters for Application.
     */
    struct Setup
    {
        //! BLE Service
        std::unique_ptr<IBLEService> bleService;
        //! Price provider
        std::unique_ptr<IPriceProvider> priceProvider;
        //! VerifyParking
        std::unique_ptr<IVerifyParking> verifier;
        //! Cloud Service
        std::unique_ptr<ICloudService> cloudService;
        //! Parking database
        std::unique_ptr<IParkingDatabase> parkingDb;
    };


    /**
     * @brief Constructor.
     * @param setup Setup parameters.
     * @pre All @p setup members are initialized.
     */
    Application(Setup&& setup);

    /**
     * @brief Destructor.
     */
    ~Application();

    /**
     * @brief Extract directory from full application path.
     * @param argv0 Full application path.
     * @return Path's directory part.
     */
    static std::string getBinaryDir(const std::string& argv0);

    /**
     * @brief Extract configuration directory from full application path.
     * @param argv0 Full application path.
     * @return Full path to configuration directory.
     */
    static std::string getConfigDir(const std::string& argv0);

    /**
     * @brief exec Start application.
     * @return 0 on normal exit.
     */
    int exec();

    /**
     * @brief Stop the application.
     */
    static void stop();

private:

    Setup m_setup;
    static std::atomic_bool m_stop;
};

}

#endif // APPLICATION_H
