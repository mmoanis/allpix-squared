/**
 *  @author Koen Wolters <koen.wolters@cern.ch>
 */

#ifndef ALLPIX_SIMPLE_CONFIG_MANAGER_H
#define ALLPIX_SIMPLE_CONFIG_MANAGER_H

#include <vector>
#include <list>
#include <string>
#include <utility>
#include <fstream>
#include <map>

#include "Configuration.hpp"
#include "ConfigManager.hpp"

#include "ConfigReader.hpp"

namespace allpix {

    class SimpleConfigManager : public ConfigManager{
    public:
        // Constructor and destructors
        SimpleConfigManager();
        explicit SimpleConfigManager(std::string file_name);
        ~SimpleConfigManager();
        
        // Add file
        void addFile(std::string file_name);
        void removeFiles();
        
        // Reload all configs (clears and rereads)
        void reload();
        
        // Clear all configuration
        void clear();
        
        // Check if configuration section exist
        virtual bool hasConfiguration(std::string name) const;
        virtual int countConfigurations(std::string name) const;
        
        // Return configuration sections by name
        virtual std::vector<Configuration> getConfigurations(std::string name) const;
        
        // Return all configurations with their name
        virtual std::vector<Configuration> getConfigurations() const;
        
    private:
        ConfigReader reader_;
        
        std::vector<std::string> file_names_;
    };
}

#endif /* ALLPIX_CONFIG_MANAGER_H */
