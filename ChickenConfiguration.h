#ifndef CHICKENCONFIGURATION_H
#define CHICKENCONFIGURATION_H

#include <cstdlib>
#include <string>
#include <regex>
#include <fstream>



class ChickenConfiguration
{
    public:
    ChickenConfiguration(std::string section);
    ~ChickenConfiguration();
    std::string getValue(std::string key);
    void setValue(std::string key, std::string value);

    private:
    void OpenConfigFile();
    void CloseConfigFile();
    std::string trim(const std::string& str);

    std::string   _path;
    std::string   _section;
    std::ifstream _file;

};



#endif