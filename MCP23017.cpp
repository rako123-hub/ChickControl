

#include "MCP23017.h"
#include "ChickenConfiguration.h"


const std::string config_section_MCP23017  = "[MCP23017]";

MCP23017::MCP23017()
{
    std::printf("+++ MCP23017 Ctor\n");
    readMCP23017_Configuration();
    initDevices();
}


MCP23017::MCP23017(Serial_I2C_Interface serialInterface)
:interface(&serialInterface)
{
    std::printf("+++ MCP23017 Ctor\n");
    readMCP23017_Configuration();
    initDevices();   
}

MCP23017::~MCP23017()
{}

void MCP23017::readMCP23017_Configuration()
{
    std::printf("+++ readMCP23017Config\n");
    ChickenConfiguration chickConfig(config_section_MCP23017);
    std::string key = "DevicesCount";
    int iValue = 0;
    if(chickConfig.getValue(key, iValue)) _devCount = iValue;

    for(int i = 1; i <= _devCount; i++)
    {   
        std::string newSection = config_section_MCP23017;                          //[MCP23017]
        newSection.erase(newSection.size()-1);
        newSection = newSection + "_" + std::to_string(i) + "]";      //[MCP23017_i]
        chickConfig.setSection(newSection);
        std::string strVal;
        key = "DevAdress"; 
        if(!chickConfig.getValue(key, strVal)) continue;
        _devAdr.push_back(strVal);
    }

    for (int x = 0; x < _devAdr.size(); x++)
    {
        printf("AdressMCP23017: %s\n", _devAdr[x].c_str());
    }
    
    std::printf("--- readMCP23017Config\n");
}

void MCP23017::initDevices()
{
    

}

std::byte MCP23017::GetHWAdress()
{

    std::byte adr;

    return adr;
    
}
