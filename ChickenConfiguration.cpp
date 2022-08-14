#include <cstdlib>
#include <iostream> 

#include "ChickenConfiguration.h"

const std::string ConfigPath = "./ChickenCtrl.conf";
const std::regex section("\\[(.*?)\\]"); 
const std::regex value("(\\w+)=([^\\+]+(?!\\+{3}))");     

//constructor

ChickenConfiguration::ChickenConfiguration(std::string section)
{
    _path    = ConfigPath;
    _section = section;
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

std::string ChickenConfiguration::getValue(std::string key)
{
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
             if (match[0] == _section)
             std::cout << "Section: " << match[0] << std::endl;
          }
          else if (std::regex_search(line, match, value))
          {
             if (key == match[1])
             {
                std::cout << "Key: " << match[1] << std::endl;
                std::cout << "Value: " << match[2] << std::endl;
                return match[2];
             }
          }
        }
    }
    else
    {
        std::cout << "Can't open file";
    }

    return "9999";
}

void ChickenConfiguration::setValue(std::string strKey, std::string strVal)
{
    
}

std::string ChickenConfiguration::trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

