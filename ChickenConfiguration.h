#ifndef CHICKENCONFIGURATION_H
#define CHICKENCONFIGURATION_H

#include <cstdlib>
#include <string>
#include <regex>
#include <fstream>



class ChickenConfiguration
{
    public:
    ChickenConfiguration(std::string section_Selected);
    ~ChickenConfiguration();
    bool getValue(std::string key, std::string &strValue);
    bool getValue(std::string key, int &iValue);
    
    void setValue(std::string key, std::string value);
    void setSection(std::string section);

    private:
    void OpenConfigFile();
    void CloseConfigFile();
    bool checkTimeString(std::string timeString);
    std::string trim(const std::string& str);

    std::string   _path;
    std::string   _section_Selected;
    std::ifstream _file;

};



#endif