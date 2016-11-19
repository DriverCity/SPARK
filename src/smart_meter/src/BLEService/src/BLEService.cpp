
#include "BLEService.h"
#include <cassert>

namespace spark
{


BLEService::BLEService() :
    IBLEService(), m_priceProvider(nullptr), m_verifier(nullptr)
{
}


BLEService::~BLEService()
{
}


void BLEService::init(IPriceProvider *priceProvider, IVerifyParking *verifier)
{
    assert(priceProvider != nullptr);
    assert(verifier != nullptr);
    m_priceProvider = priceProvider;
    m_verifier = verifier;
}


void BLEService::start()
{

}

IPriceProvider* BLEService::priceProvider() const
{
    return m_priceProvider;
}


IVerifyParking* BLEService::verifier() const
{
    return m_verifier;
}

} // spark
