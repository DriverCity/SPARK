/**
 * @file
 * @brief Implementation for the IBLEService interface.
 */

#ifndef BLESERVICE_H
#define BLESERVICE_H

#include "IBLEService.h"
#include <string>
#include <thread>
#include <atomic>

namespace spark
{

/**
 * @brief Implementation for the IBLEService interface.
 */
class BLEService : public IBLEService
{
public:

    /**
     * @brief Constructor. Call init before staring.
     */
    BLEService(const std::string& inputFifo, const std::string& responseFifo);

    virtual ~BLEService();

    // Implements IBLEService
    virtual void init(IPriceProvider *priceProvider, IVerifyParking *verifier);
    virtual void start();
    virtual void stop();

    /**
     * @brief Get price provider.
     * @return Current price provider.
     */
    IPriceProvider* priceProvider() const;

    /**
     * @brief Get verifier.
     * @return Current verifier.
     */
    IVerifyParking* verifier() const;

private:

    IPriceProvider* m_priceProvider;
    IVerifyParking* m_verifier;
    std::string m_inputFifo;
    std::string m_responseFifo;

    std::thread m_serviceThread;
    std::atomic_bool m_stopService;

    void startServiceThread();
    void handleMessage(const std::string& msg);
    void sendResponse(std::string msg);

};


} // spark

#endif // BLESERVICE_H
