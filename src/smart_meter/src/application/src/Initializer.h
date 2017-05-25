/**
 * @file
 * @brief Defines the Initializer class.
 */

#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <ostream>
#include <string>
#include "Application.h"
#include "GlobalConfiguration/GlobalConfiguration.h"

namespace spark
{

/**
 * @brief The Initializer class is responsible for instantiating and initializing
 *  all components.
 */
class Initializer
{
public:

    /**
     * @brief Initialize Logger component.
     * @param conf Application configuration.
     * @param printOut Print output stream.
     */
    void InitializeLogger(const GlobalConfiguration* conf, std::ostream& printOut) const;

    /**
     * @brief Initialize application configuration.
     * @param appPath Full application path.
     */
    void InitializeConfiguration(std::string appPath) const;

    /**
     * @brief Creates and initializes all Application's setup parameters.
     * @param conf Application's configuration.
     * @return Setup object with initialized members.
     * @exception std::exception, if creating or initializing any parameter fails.
     */
    Application::Setup createSetup(const GlobalConfiguration* conf) const;


private:

    void createCloudService(const GlobalConfiguration* conf, Application::Setup& setup) const;
    void createPriceProvider(const GlobalConfiguration* conf, Application::Setup& setup) const;
    void createParkingDatabase(const GlobalConfiguration* conf, Application::Setup& setup) const;
    void createVerifyParking(Application::Setup& setup) const;
    void createBLEService(const GlobalConfiguration* conf, Application::Setup& setup) const;
};

}

#endif
