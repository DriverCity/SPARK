

#include "PriceInfo.h"
#include <cassert>
#include <sstream>
#include "Logger/Logger.h"

namespace spark
{

PriceInfo::PriceInfo():
    m_hourPrice(0.0),
    m_timeLimit(0),
    m_resolution(0)
{
}


PriceInfo::PriceInfo(float pricePerHour, int timeLimit, int resolution):
    m_hourPrice(pricePerHour),
    m_timeLimit(timeLimit),
    m_resolution(resolution)
{
}


float PriceInfo::pricePerHour() const
{
    return m_hourPrice;
}


int PriceInfo::timeLimit() const
{
    return m_timeLimit;
}

int PriceInfo::timeResolution() const
{
    return m_resolution;
}


bool PriceInfo::isValid() const
{
    return m_hourPrice >= 0 && m_timeLimit >= 0 && m_resolution > 0;
}


std::string PriceInfo::toString() const
{
    std::ostringstream oss;
    oss << m_hourPrice << ";" << m_timeLimit << ";" << m_resolution;
    return oss.str();
}


PriceInfo PriceInfo::fromString(const std::string &str, char separator)
{
    float price = -1;
    int limit = -1;
    int resolution = -1;
    std::istringstream iss(str);

    try
    {
        // price per hour
        {
            std::string tmp;
            std::string::size_type idx = 0;
            std::getline(iss, tmp, separator);
            price = std::stof(tmp, &idx);
            if (idx != tmp.length()) throw std::exception();
        }

        // time limit
        {
            std::string tmp;
            std::string::size_type idx = 0;
            std::getline(iss, tmp, separator);
            limit = std::stoul(tmp, &idx);
            if (idx != tmp.length()) throw std::exception();
        }

        // resolution
        {
            std::string tmp;
            std::string::size_type idx = 0;
            std::getline(iss, tmp, separator);
            resolution = std::stoi(tmp, &idx);
            if (idx != tmp.length()) throw std::exception();
        }

        // Check for extra fields.
        if (iss.peek() != EOF){
            throw std::exception();
        }
    }
    catch (std::exception&){
        LOG_ERROR("Parsing PriceInfo failed. Input: " << str);
        return PriceInfo();
    }

    return PriceInfo(price, limit, resolution);
}


} // spark
