/**
 * @file
 * @brief Defines abstract interface for BLEService..
 */

#ifndef IBLESERVICE_H
#define IBLESERVICE_H

#include "PriceProvider/IPriceProvider.h"
#include "VerifyParking/IVerifyParking.h"

namespace spark
{

/**
 * @brief Abstract interface for BLEService.
 */
class IBLEService
{
public:
    virtual ~IBLEService() {}

    /**
     * @brief Initialize service.
     * @param priceProvider Callback interface for getting price information.
     * @param verifier Callback interface for verifying parking event.
     */
    virtual void init(IPriceProvider* priceProvider,
                      IVerifyParking* verifier) = 0;

    /**
     * @brief Start service.
     */
    virtual void start() = 0;

    /**
     * @brief Stop service.
     */
    virtual void stop() = 0;
};

} // spark

#endif // IBLESERVICE_H
