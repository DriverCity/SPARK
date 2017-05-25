/**
 * @file
 * @brief Defines class for reading global configuraton from file.
 */

#ifndef CONFIGURATIONREADER_H
#define CONFIGURATIONREADER_H

#include <string>
#include <map>

namespace spark
{

/**
 * @brief Reads configuration from file.s
 */
class ConfigurationReader
{
public:

    /**
     * @brief Return value from reading.
     */
    typedef std::map<std::string,std::string> ConfigMap;

    /**
     * @brief Constructor.
     */
    ConfigurationReader();

    /**
     * @brief Destructor.
     */
    ~ConfigurationReader();

    /**
     * @brief Read configuration file.
     * @param file Configuration file path.
     * @return Configuration read from file. Empty configuration if reading fails.
     * @pre None.
     */
    ConfigMap readFile(const std::string& file) const;

    /**
     * @brief Read configuration from a generic input stream.
     * @param input Input stream e.g. opened file.
     * @return Configuration read from @p input.
     * @pre @p input is in a valid state.
     * @post @p input is read to the end or untill error occurs.
     */
    ConfigMap readConf(std::istream& input) const;

    /**
     * @brief Read configuration parameter from a row.
     * @param conf Target configuration.
     * @param row Input row.
     * @pre @p row is not empty or a comment (starts with '#').
     * @post Parameter read from @p row is appended to @p conf.
     *  If @p row is an invalid parameter, @p conf is cleared.
     */
    void readRow(ConfigMap& conf, const std::string& row) const;
};

} // spark

#endif // CONFIGURATIONREADER_H
