/**
 * @file
 * @brief Implementation for the IBLEService interface.
 */

#ifndef BLESERVICE_H
#define BLESERVICE_H

#include "IBLEService.h"

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
    BLEService();

    virtual ~BLEService();

    // Implements IBLEService
    virtual void init(IPriceProvider *priceProvider, IVerifyParking *verifier);
    virtual void start();

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
};


} // spark

#endif // BLESERVICE_H
