
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

class DemoModeBLEService : public BLEService
{
public:

    /**
     * @brief Possible demo responeses listed in demo configuration.
     */
    enum class DemoResult
    {
        OK = 0,
        TIMEOUT,
        INVALID_TOKEN,
        OTHER
    };

    DemoModeBLEService(const std::string& inputFifo,
                       const std::string& responseFifo,
                       const std::string& demoConfFile);

    virtual ~DemoModeBLEService();


protected:

    virtual void handleMessage(const std::string& msg);


private:

    ConfigurationReader::ConfigMap m_demoConf;
    int m_step;

    void hanleDemoValue(int demoValue, const std::string& msg);
};



} // spark

#endif // DEMOMODEBLESERVICE_H
