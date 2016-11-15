
#include "ParkingEvent.h"

namespace spark
{

PaymentToken::PaymentToken(const std::string &verifier, const std::string &uid) :
    m_verifier(verifier), m_uid(uid)
{
}


std::string PaymentToken::verifier() const
{
    return m_verifier;
}


std::string PaymentToken::uid() const
{
    return m_uid;
}


ParkingEvent::ParkingEvent(const std::string &registerNumber,
                           const std::string &startingTime,
                           int duration,
                           const PaymentToken &token) :

    m_regNum(registerNumber),
    m_startingTime(startingTime),
    m_duration(duration),
    m_token(token)
{
}


std::string ParkingEvent::registerNumber() const
{
    return m_regNum;
}


std::string ParkingEvent::startingTime() const
{
    return m_startingTime;
}


int ParkingEvent::duration() const
{
    return m_duration;
}


PaymentToken ParkingEvent::token() const
{
    return m_token;
}


} // spark
