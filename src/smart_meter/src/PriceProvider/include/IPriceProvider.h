/**
 * @file
 * @brief Define abstract interface for PriceProvider.
 */

#ifndef IPRICEPROVIDER_H
#define IPRICEPROVIDER_H

#include "CloudService/ICloudService.h"
#include "PriceInfo.h"

namespace spark
{

/**
 * @brief Abstract interface for getting price information.
 */
class IPriceProvider
{
public:

    virtual ~IPriceProvider() {}

    /**
     * @brief Initialize PriceProvider.
     * @param cloudService Cloud service.
     * @pre @cloudService is not nullptr.
     * @post PriceProvider is ready to be used.
     */
    virtual void init(ICloudService* cloudService) = 0;

    /**
     * @brief Get current pricing information for this meter.
     * @return Price information. Return invalid PriceInfo if there is no
     *  working connection to the cloud.
     * @pre PriceProvider has been initialized.
     */
    virtual PriceInfo getPriceInfo() = 0;
};

} // spark

#endif // IPRICEPROVIDER_H
