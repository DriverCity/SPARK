#include <string>
#include <memory>
#include <map>

namespace spark
{

/**
 * @brief Singleton class for accessing global configuration parameters.
 */
class GlobalConfiguration
{
public:

    ~GlobalConfiguration();

    /**
     * @brief Initialize.
     * @param confFile Configuration file.
     * @pre None.
     * @post GlobalConfiguration is instantiated with @p confFile.
     *  Calling init() again resets configuration.
     */
    static void init(const std::string& confFile);

    /**
     * @brief Get pointer to GlobalConfiguration instance.
     * @return pointer to active instance. Returns nullptr, if not initialized.
     */
    static GlobalConfiguration* instance();

    /**
     * @brief Check if configuration contains given parameter.
     * @param key Parameter key.
     * @return True, if key exists.
     */
    bool hasKey(const std::string& key) const;

    /**
     * @brief Get value paired with the key.
     * @param key Parameter key.
     * @param ok If not 0, this is set to false, if key is not found. Else true.
     * @return Value paired to the @p key. Returns empty string if key is not found.
     */
    std::string getValue(const std::string& key, bool* ok=0) const;


private:

    // Prevent construction outside the class.
    GlobalConfiguration(const std::map<std::string,std::string>& conf);
    GlobalConfiguration(const GlobalConfiguration&);
    GlobalConfiguration& operator=(const GlobalConfiguration&);

    static std::unique_ptr<GlobalConfiguration> instance_;
    std::map<std::string,std::string> m_conf;
};

} // spark
