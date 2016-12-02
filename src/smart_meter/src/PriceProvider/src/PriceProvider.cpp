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

    double price = m_cloudService->getPricePerHour();

    int res  = m_cloudService->getParkingTimeResolution();

    int limit = m_cloudService->getTimeLimit();


    return PriceInfo(price,res, limit);

}

ICloudService *PriceProvider::getCloudService() const
{
  return m_cloudService;
}

}
