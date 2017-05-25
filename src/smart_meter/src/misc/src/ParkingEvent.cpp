
#include "ParkingEvent.h"
#include "Logger/Logger.h"
#include "sstream"
#include <vector>
#include <stdexcept>
#include <ctime>

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


ParkingEvent::ParkingEvent():
    m_regNum(""),
    m_startingTime(""),
    m_duration(0),
    m_token(PaymentToken("", ""))
{
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


bool ParkingEvent::isValid() const
{
    return !m_regNum.empty();
}


std::string ParkingEvent::toString() const
{
    std::string rval;
    if (isValid()){
        std::ostringstream oss;
        oss << "Park;" << m_regNum << ";" << m_startingTime << ";"
            << m_duration << ";" << m_token.verifier() << ";" << m_token.uid();
        rval = oss.str();
    }
    return rval;
}


bool ParkingEvent::operator==(const ParkingEvent &rhs) const
{
    return duration() == rhs.duration() &&
            registerNumber() == rhs.registerNumber() &&
            startingTime() == rhs.startingTime() &&
            token().uid() == rhs.token().uid() &&
            token().verifier() == rhs.token().verifier();
}


ParkingEvent ParkingEvent::fromString(const std::string &str)
{
    std::vector<std::string> parts;
    std::istringstream iss(str);

    std::string tmp;
    while (std::getline(iss, tmp, ';')){
        parts.push_back(tmp);
    }

    if (parts.size() != 6 || parts.at(0) != "Park"){
        LOG_ERROR("Incorrect format.");
        return ParkingEvent();
    }

    int parkingTime(0);
    try{
        parkingTime = std::stoi(parts.at(3));
    }
    catch (std::invalid_argument&){
        LOG_ERROR("Duration is not a number.");
        return ParkingEvent();
    }

    if (parkingTime <= 0){
        LOG_ERROR("Invalid duration");
        return ParkingEvent();
    }

    if (!checkDateTime(parts.at(2))) {
        LOG_ERROR("Invalid datetime");
        return ParkingEvent();
    }

    PaymentToken token(parts.at(4), parts.at(5));
    return ParkingEvent(parts.at(1), parts.at(2), parkingTime, token);
}


bool ParkingEvent::checkDateTime(const std::string &str)
{
    tm time;
    return strptime(str.c_str(), "%Y-%m-%d %R", &time) != NULL;
}


} // spark
