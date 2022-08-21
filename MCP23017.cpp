#include <iostream> 

#include "MCP23017.h"
#include "ChickenConfiguration.h"



const std::string config_section_MCP23017  = "[MCP23017]";

MCP23017::MCP23017()
{
    readMCP23017_Configuration();
}

MCP23017::~MCP23017()
{}

void MCP23017::readMCP23017_Configuration()
{
    ChickenConfiguration chickConfig(config_section_MCP23017);
    std::string key = "DevicesCount";
    int iValue = 0;


    if(chickConfig.getValue(key, iValue)) devCount = iValue;

// TODO überarbeiten
    /*
    for(int i = 1; i <= devCount; i++)
    {
        std::string strVal;
        key = "DevAdress"; 
        key.append(std::to_string(i));
        if(!chickConfig.getValue(key, strVal)) continue;
        devAdr.push_back(strVal);
    }
    */
}
