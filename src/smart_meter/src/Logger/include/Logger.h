/**
 * @file
 * @brief Defines spark logging utilities.
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <memory>
#include <sstream>

#ifndef COMPONENT_NAME
/**
 * @def COMPONENT_NAME
 * @brief If Component name is not defined, use 'unknown' as component name.
 */
#define COMPONENT_NAME "unknown"
#endif


/**
 * @def LOG_IMPLEMENTATION(identifier, msg)
 * @brief Common part of implementation for macros below. For internal use only.
 * @param identifier Message type.
 * @param msg Message to be printed.
 */
#define LOG_IMPLEMENTATION(identifier, msg)\
{\
    std::ostringstream logger_impl_oss; \
    logger_impl_oss << identifier \
        << "[" << COMPONENT_NAME << "] "\
        << std::string(__FILE__).substr(std::string(__FILE__).find_last_of("/")+1) \
        << " " << __LINE__ << ": " << msg; \
    spark::Logger::write(logger_impl_oss.str());\
}


/**
 * @def LOG_DEBUG(msg)
 * @brief Convenience macro. Prints a debug message with component name, file name and line number.
 * @param msg Message to be printed.
 */
#define LOG_DEBUG(msg) \
{\
    LOG_IMPLEMENTATION(spark::Logger::DEBUG_MSG, msg) \
}


/**
 * @def LOG_ERROR(msg)
 * @brief Convenience macro. Prints an error message with component name, file name and line number.
 * @param msg Message to be printed.
 */
#define LOG_ERROR(msg) \
{\
    LOG_IMPLEMENTATION(spark::Logger::ERROR_MSG, msg) \
}



namespace spark
{

/**
 * @brief Utility class for printing and logging.
 */
class Logger
{
public:

    /**
     * @brief Initialize logger.
     * @param logFile Log messages are saved here. If Logger::NO_LOG_FILE is
     *  given, log messages are not saved to any file.
     *
     * @param output Printing stream. Defaults to std::cout. Can be changed
     *  for testing purposes.
     *
     * @pre @p output != nullptr.
     *
     * @post Logger is ready to be written.
     */
    static void init(const std::string& logFile = NO_LOG_FILE,
                     std::ostream& output = std::cout);

    /**
     * @brief Write a log message.
     *
     * @param msg Log message to be written.
     *
     * @post @p msg is printed to @p output and @p logFile given in init.
     */
    static void write(const std::string& msg);

    /**
     * @brief Closes the logger.
     * @pre None.
     * @post Log file is closed. Log file and print output are set back to defualt.
     *  Logger can be re-initialized at later time.
     */
    static void close();

    /**
     * @brief Identifier for a debug log message.
     */
    static const std::string DEBUG_MSG;

    /**
     * @brief Identifier for an error log message.
     */
    static const std::string ERROR_MSG;

    /**
     * @brief Special value to be given as @p logFile to indicate that
     *  log messages will not be written to a file.
     */
    static const std::string NO_LOG_FILE;

private:
    static std::unique_ptr<std::ostream> fileStream_;
    static std::ostream* printStream_;
};

} // spark

#endif // LOGGER_H
