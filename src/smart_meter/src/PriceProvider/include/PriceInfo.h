/**
 * @file
 * @brief Defines data type for representing pricing information.
 */

#ifndef PRICEINFO_H
#define PRICEINFO_H

#include <string>

namespace spark
{


/**
 * @brief Datatype for pricing information.
 */
class PriceInfo
{
public:

    /**
     * @brief Default constructor. Constructs invalid PriceInfo.
     */
    PriceInfo();

    /**
     * @brief Constructor.
     * @param pricePerHour Price per hour in EUR.
     * @param timeLimit Maximum parking time in minutes.
     * @param resolution Time resolution in minutes.
     * @post Created object is valid, if @p pricePerHour >= 0, @p timeLimit >= 0 and @p resolution > 0.
     */
    PriceInfo(float pricePerHour, int timeLimit, int resolution);

    /**
     * @brief Get parking price per hour.
     * @return Price in EUR.
     */
    float pricePerHour() const;

    /**
     * @brief Maximum alloted parking time.
     * @return Maximum time in minutes. 0 if there is no time limit.
     */
    int timeLimit() const;

    /**
     * @brief Get time resolution
     * @return Smallest quantity of parking time in minutes.
     */
    int timeResolution() const;

    /**
     * @brief Check that PriceInfo is valid.
     * @return True, if information is valid.
     */
    bool isValid() const;

    /**
     * @brief Convert to string.
     * @return String representation.
     */
    std::string toString() const;

    /**
     * @brief Create PriceInfo from string representation.
     * @param str Input string.
     * @param separator Separator character.
     * @return PriceInfo represented by @p str,
     *  or invalid PriceInfo, if @p str does not represent a valid object.
     */
    static PriceInfo fromString(const std::string& str, char separator = ';');


private:
    float m_hourPrice;
    int m_timeLimit;
    int m_resolution;
};


} // spark

#endif // PRICEINFO_H
