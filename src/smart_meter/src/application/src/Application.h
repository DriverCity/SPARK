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
        std::shared_ptr<IBLEService> bleService;
        //! Price provider
        std::shared_ptr<IPriceProvider> priceProvider;
        //! VerifyParking
        std::shared_ptr<IVerifyParking> verifier;
        //! Cloud Service
        std::shared_ptr<ICloudService> cloudService;
        //! Parking database
        std::shared_ptr<IParkingDatabase> parkingDb;
    };


    /**
     * @brief Constructor.
     * @param setup Setup parameters.
     * @pre All @p setup members are initialized.
     */
    Application(const Setup& setup);

    /**
     * @brief Destructor.
     */
    ~Application();


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
