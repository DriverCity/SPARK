/**
 * @file
 * @brief Define the PriceProvider class.
 */

#ifndef PRICEPROVIDER_H
#define PRICEPROVIDER_H
#include "IPriceProvider.h"

#include "CloudService/ICloudService.h"
#include "PriceInfo.h"

namespace spark
{

/**
 * @brief Implementation for the IPriceProvider interface.
 */
class PriceProvider : public IPriceProvider
{
public:
    PriceProvider();

    virtual ~PriceProvider();
    
    virtual void init(ICloudService* cloudService);
    
    virtual PriceInfo getPriceInfo();

    /**
     * @brief get cloud service.
     * @return Pointer to CloudService.
     */
    ICloudService * getCloudService() const;
    
private:
    ICloudService* m_cloudService;
};

}

#endif
