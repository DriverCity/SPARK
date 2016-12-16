/**
 * @file
 * @brief Define abstract interface for PriceProvider.
 */

#ifndef PRICEPROVIDER_H
#define PRICEPROVIDER_H
#include "IPriceProvider.h"

#include "CloudService/ICloudService.h"
#include "PriceInfo.h"

namespace spark
{

/**
 * @brief Abstract interface for getting price information.
 */
class PriceProvider : public IPriceProvider
{
public:
    PriceProvider();

    virtual ~PriceProvider();
    
    virtual void init(ICloudService* cloudService);
    
    virtual PriceInfo getPriceInfo();

    ICloudService * getCloudService() const;
    
private:
    ICloudService* m_cloudService;
};

}

#endif
