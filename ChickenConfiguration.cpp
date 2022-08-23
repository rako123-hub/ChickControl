#include <cstdlib>
#include <iostream> 

#include "GlobalDefs.h"
#include "ChickenConfiguration.h"


const std::string ConfigPath = "./ChickenCtrl.conf";
const std::regex section("\\[(.*?)\\]"); 
const std::regex value("(\\w+)=([^\\+]+(?!\\+{3}))");     

//constructor

ChickenConfiguration::ChickenConfiguration(std::string section)
{
    _path    = ConfigPath;
    _section_Selected = section;
    OpenConfigFile();
}



ChickenConfiguration::~ChickenConfiguration()
{

   std::cout << "destructor chickencontrl" << std::endl;
   if(_file.is_open())
   {
       _file.close();
   }
}

void ChickenConfiguration::OpenConfigFile()
{
    std::string line;
    _file.open(_path);

    if(!_file.is_open())
    {
        std::cout << "Can't open file: " << _path << std::endl; 
        std::cout << "Use default parameters" << std::endl;
    }
    else
    {
        std::cout << "open config file: " << _path << std::endl; 
    }
}


void ChickenConfiguration::CloseConfigFile()
{}

bool ChickenConfiguration::getValue(std::string key, std::string &strvalue)
{
    bool result = false;
    bool sectionfindfirst = false;
    std::string line;
    if (_file.is_open())
    {
       _file.seekg(std::ios::beg);  // Set ptr to the file beginning

       while(getline(_file, line))
       {
           line = trim(line);
           // print the line on the standard output
           std::smatch match;
           if (std::regex_search(line, match, section))
           {
              if (match[0] == _section_Selected)
              {
                 std::cout << "Section: " << match[0] << std::endl;
                 sectionfindfirst = true;
                 continue;
              }
           }
           if(sectionfindfirst)
           {
              if (std::regex_search(line, match, value))
              {
                 if (key == match[1])
                 {
                    std::cout << "Key: " << match[1] << std::endl;
                    std::cout << "Value: " << match[2] << std::endl;
                    strvalue  = match[2];
                    result = true;
                    break;
                 }
              }
           }
         }
     }
     else   std::cout << "Can't open file";
     return result;
}

bool ChickenConfiguration::getValue(std::string key, int &ivalue)
{
    bool result = false;
    bool sectionfindfirst = false;
    std::regex digits("[[:digit:]]+");
    std::string line;
    if (_file.is_open())
    {
       _file.seekg(std::ios::beg);  // Set ptr to the file beginning

       while(getline(_file, line))
       {
           line = trim(line);
           // print the line on the standard output
           std::smatch match;
           if (std::regex_search(line, match, section))
           {
              if (match[0] == _section_Selected)
              {
                 std::cout << "Section: " << match[0] << std::endl;
                 sectionfindfirst = true;
                 continue;
              }
           }
           if(sectionfindfirst)
           {
              if (std::regex_search(line, match, value))
              {
                 if (key == match[1])
                 {
                    std::cout << "Key: " << match[1] << std::endl;
                    std::cout << "Value: " << match[2] << std::endl;
                    std::string strmatch = match[2];

                    if(std::regex_search(strmatch, match, digits))
                    {
                       std::string striValue = match[0];
                       int test = std::atoi(striValue.c_str());
                       ivalue = test;
                       result = true;
                       break;
                    }  
                 }
              }
           }
         }
     }
     else   std::cout << "Can't open file";
     return result;
}

void ChickenConfiguration::setValue(std::string strKey, std::string strVal)
{}

void ChickenConfiguration::setSection(std::string section)
{
    _section_Selected = section;
}

std::string ChickenConfiguration::trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool ChickenConfiguration::checkTimeString(std::string timeString)
{
    bool result = false;
     // Regex to check valid time in 24-hour format
    const std::regex time_pattern("([01]?[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]");

    if ( regex_match(timeString, time_pattern)) result = true;
    else
    {
        std::printf("ERROR Wrong time configuration!!  Time : %s \n", timeString.c_str());
    }
    return result;
}

