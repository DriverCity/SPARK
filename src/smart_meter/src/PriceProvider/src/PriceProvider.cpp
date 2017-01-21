#include "PriceProvider.h"
#include "cassert"

namespace spark {


PriceProvider::PriceProvider()
{

}

PriceProvider::~PriceProvider()
{

}


void PriceProvider::init(ICloudService *cloudService)
{
    assert (cloudService!=nullptr);
    this->m_cloudService = cloudService;


}

PriceInfo PriceProvider::getPriceInfo()
{
    double price(0);
    int limit(0);
    bool result = m_cloudService->getPriceInformation(price, limit);

    PriceInfo info(price, limit, 5);
    if (!result){
        info = PriceInfo();
    }

    return info;
}

ICloudService *PriceProvider::getCloudService() const
{
  return m_cloudService;
}

}
