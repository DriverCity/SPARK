/**
 * @file
 * @brief Define data types for representing parking event.
 */

#ifndef PARKINGEVENT_H
#define PARKINGEVENT_H

#include <string>

namespace spark
{


/**
 * @brief Represents payment token
 */
class PaymentToken
{
public:

    /**
     * @brief Constructor.
     * @param verifier Name of the payment verifier.
     * @param uid Payment's unique id.
     */
    PaymentToken(const std::string& verifier,
                 const std::string& uid);

    /**
     * @brief Get verifier name.
     */
    std::string verifier() const;

    /**
     * @brief Get payment unique id.
     */
    std::string uid() const;

private:

    std::string m_verifier;
    std::string m_uid;
};


/**
 * @brief Represents accepted parking event.
 */
class ParkingEvent
{
public:

    /**
     * @brief Default constructor. Required for storing events to stl-containers.
     */
    ParkingEvent();

    /**
     * @brief Constructor.
     * @param registerNumber Car's register number.
     * @param startingTime Timestamp for parking start time. Timestamp format should be 'TODO'.
     * @param duration parking duration in minutes.
     * @param token Payment token.
     */
    ParkingEvent(const std::string& registerNumber,
                 const std::string& startingTime,
                 int duration, const PaymentToken& token);


    /**
     * @brief Get car's register number.
     */
    std::string registerNumber() const;

    /**
     * @brief Get parking event starting time.
     * @return Timestamp in format 'TODO'.
     */
    std::string startingTime() const;

    /**
     * @brief Get parking duration in minutes.
     */
    int duration() const;

    /**
     * @brief Get payment token.
     */
    PaymentToken token() const;

private:

    std::string m_regNum;
    std::string m_startingTime;
    int m_duration;
    PaymentToken m_token;
};

} // spark

# endif // PARKINGEVENT_H
