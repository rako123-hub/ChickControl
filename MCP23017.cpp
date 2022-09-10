
#include <regex>
#include "MCP23017.h"


const std::string config_section_MCP23017  = "[MCP23017]";



MCP23017::MCP23017()
{
    std::printf("+++ MCP23017 Ctor\n");
    readMCP23017_Configuration();
    init_MCP23017_Devices();
}

MCP23017::MCP23017(Serial_I2C_Interface serialInterface)
:interface(&serialInterface)
{
    std::printf("+++ MCP23017 Ctor\n");
    readMCP23017_Configuration();
    init_MCP23017_Devices();   
}

MCP23017::~MCP23017()
{}

void MCP23017::readMCP23017_Configuration()
{
    std::printf("+++ readMCP23017Config\n");
    ChickenConfiguration *chickConfig = new ChickenConfiguration(config_section_MCP23017);
    std::string key = "DevicesCount";
    int iValue, devCount = 0;
    if(chickConfig->getValue(key, iValue)) devCount = iValue;

    for(int i = 1; i <= devCount; i++)
    {   
        std::string newSection = config_section_MCP23017;             //[MCP23017]
        newSection.erase(newSection.size()-1);
        newSection = newSection + "_" + std::to_string(i) + "]";      //[MCP23017_i]
        chickConfig->setSection(newSection);
        std::string strVal;
        key = "DevAdress"; 
        if(!chickConfig->getValue(key, strVal)) continue;
        std::regex pattern("0x|0X");
        strVal = std::regex_replace(strVal, pattern, "");
        _devAdrVec.emplace_back(strVal);
        _gpio_Adr_Dir_Map.insert(std::pair<std::string, std::vector<std::string>>(strVal, std::vector<std::string>()));  //add empty vector to key (strVal)
        readMCP23017_Dir_Config(chickConfig, strVal, i);
    }

    if(chickConfig != nullptr) delete chickConfig;
    
    std::printf("--- readMCP23017Config\n");
}

void MCP23017::readMCP23017_Dir_Config(ChickenConfiguration *chickConfig, std::string strAdr, int devNum)
{
    /* GPIO_0_1 */
    std::string key;
    for (int i = 0; i < GPIO_COUNT; i++)
    {
       key = "GPIO_" + std::to_string(i) + "_" + std::to_string(devNum);
       std::string strVal;
       if(chickConfig->getValue(key, strVal))
       {
           _gpio_Adr_Dir_Map[strAdr].emplace_back(strVal);
       }  
    }
}

void MCP23017::init_MCP23017_Devices()
{
    checkConnectedDevices();
    set_MCD230127_DirectionPins();

}

void MCP23017::set_MCD230127_DirectionPins()
{
    
}

/* try to connect the configured devices and store them into vector after this delte the key of not connected devs*/
void MCP23017::checkConnectedDevices() 
{
    char recvBuf[256];
    std::string strCommand;
    std::string strAnswer;
    std::vector<std::string> connectedDevsVec;
    strCommand = "y31";
    interface->write_Serial(strCommand);

    for (const auto& elements : _gpio_Adr_Dir_Map)  //key --> elements.first, value --> elements.second
    {
        std::string devAdr = elements.first;
        strCommand = "S " + devAdr + " 00 P";
        interface->write_Serial(strCommand);
        interface->read_Serial(strAnswer);
        std::transform(strAnswer.begin(), strAnswer.end(), strAnswer.begin(), tolower);
        if(strAnswer.find("kk") !=  std::string::npos )
        {
            connectedDevsVec.emplace_back(devAdr);
        }

        printf("Connected DevAdr : 0x%s\n",devAdr.c_str() );
    }
    strCommand = "y30";
    interface->write_Serial(strCommand); 
}

void MCP23017::setOutputPin(std::string gpioPin, byte val)
{
    
}

byte MCP23017::getPin(std::string gpioPin)
{
    byte pinState = 0xff;

    return pinState;
    
}
/*
mymap.insert(pair<int,vector<int> >(10, vector<int>()));

You could then add whatever elements you want with something like:

mymap[10].push_back(1);
mymap[10].push_back(2);

*/