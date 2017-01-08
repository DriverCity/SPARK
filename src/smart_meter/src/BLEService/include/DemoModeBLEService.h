
/**
 * @file
 * @brief Defines BLEService version used for demonstration purposes.
 */

#ifndef DEMOMODEBLESERVICE_H
#define DEMOMODEBLESERVICE_H

#include "BLEService.h"
#include "GlobalConfiguration/ConfigurationReader.h"

namespace spark
{

/**
 * @brief IBLEService implementation used in the demo mode.
 */
class DemoModeBLEService : public BLEService
{
public:

    /**
     * @brief Possible demo responeses listed in demo configuration.
     */
    enum class DemoResult
    {
        //! Replace actual token with valid test token.
        OK = 0,
        //! Demonstrate cloud request timeout.
        TIMEOUT,
        //! Demonstrate invalid payment token.
        INVALID_TOKEN,
        //! Demonstrate other error registering parking event.
        OTHER_ERROR,
        //! Automatically send OK response.
        FORCED_OK
    };

    /**
     * @brief Constructor.
     * @param inputFifo Fifo where to read incoming messages from.
     * @param responseFifo Fifo where to write response messages.
     * @param demoConfFile Path to demo mode configuration file.
     */
    DemoModeBLEService(const std::string& inputFifo,
                       const std::string& responseFifo,
                       const std::string& demoConfFile);

    /**
     * @brief Destructor.
     */
    virtual ~DemoModeBLEService();


protected:

    /**
     * @brief Reimplemented for demonstration purposes.
     * @param msg Received message.
     */
    virtual void handleMessage(const std::string& msg);


private:

    ConfigurationReader::ConfigMap m_demoConf;
    int m_step;
    PriceInfo m_demoPrice;

    void hanleDemoValue(int demoValue, const std::string& msg);
    void handlePrice();
    PriceInfo parsePrice(std::string str) const;
};



} // spark

#endif // DEMOMODEBLESERVICE_H
