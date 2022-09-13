
#include <regex>
#include "MCP23017.h"


const std::string config_section_MCP23017  = "[MCP23017]";



MCP23017::MCP23017()
{
    std::printf("+++ MCP23017 Ctor\n");
    readMCP23017_Configuration();
    init_MCP23017_Devices();
}

MCP23017::MCP23017(Serial_I2C_Interface *serialInterface)
:interface(serialInterface)
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
    set_MCP230127_Dir_and_PullUp_Pins();
}

void MCP23017::set_MCP230127_Dir_and_PullUp_Pins()
{
    for(std::string strAdr : _connectedDevsVec)
    {
        auto item = _gpio_Adr_Dir_Map.find(strAdr);       
        if(item != _gpio_Adr_Dir_Map.end())
        {
           byte gpioPortOffset = 0x00;
           if(_gpio_Adr_Dir_Map[strAdr].capacity() == GPIO_COUNT)
           {
               byte temp = 0x00;
               for(int port = 0; port < GPIO_PORTS; port++ )
               {
                  byte byteVal  = 0x01;
                  byte gpioPort = 0x00;
                  for(int gpio = gpioPortOffset; gpio < gpioPortOffset + 8; gpio++)
                  {
                       byteVal = 0x01;
                       byteVal <<= (gpio - gpioPortOffset);
                       if(_gpio_Adr_Dir_Map[strAdr].at(gpio) == "Input")    //Input --> gpioDir = 1
                       {
                           gpioPort = gpioPort | byteVal;
                       }
                       temp = gpio;                  
                  }
                  gpioPortOffset = temp + 1;
                  std::string strCommand = "S " + strAdr + " P";
                  interface->write_Serial(strCommand);
                  std::string IODIR = hexTable[IODIRA];
                  std::string GPPU  = hexTable[GPPUA];
                  if(port == 1) 
                  {
                      IODIR = hexTable[IODIRB];
                      GPPU  = hexTable[GPPUB]; 
                  }
                  strCommand = "S " + IODIR  + " " + hexTable[gpioPort] + " P";    //Dir PortA, PortB
                  interface->write_Serial(strCommand);
                  strCommand = "S " + GPPU + " " + hexTable[gpioPort] + " P";       // set pullups for INPUT
                  interface->write_Serial(strCommand);  
               }
           }
           else std::printf("ERROR *** MCP23017:: Map capacity != GPIOCOUNT\n");
        }
    } 
}

/* try to connect the configured devices and store them into vector after this delte the key of not connected devs*/
void MCP23017::checkConnectedDevices() 
{
    char recvBuf[256];
    std::string strCommand;
    std::string strAnswer;
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
            _connectedDevsVec.emplace_back(devAdr);
        }
    }
    strCommand = "y30";
    interface->write_Serial(strCommand); 
}

void MCP23017::get_Adr_byteVal_Port(std::string strGpioDev, std::string &strAddr, byte &byteVal, bool &portB)
{
    //for e.g. strGPIOPin = LightIO Pin ===> GPIO_4:1  ===> Pin4 MCP23017 Device 1
    int pos1 = strGpioDev.find("_");
    int pos2 = strGpioDev.find(":");
    std::string strGPIO = strGpioDev.substr(pos1 + 1 , pos2 - (pos1 + 1));
    strAddr = strGpioDev.substr(pos2 + 1);
    byte portOffset = 8;
    byte gpioPin = atoi(strGPIO.c_str());
    if(gpioPin > 7) //detect Port B
    {
        gpioPin -= portOffset;
        portB = true;
    }
    byteVal= 0x01;
    byteVal <<= gpioPin;
}

void MCP23017::setOutputPin(std::string strGPIOPin_Dev, byte val)
{
    //for e.g. strGPIOPin = LightIO Pin ===> GPIO_4:1  ===> Pin4 MCP23017 Device 1
    std::string strAddr = "";
    byte byteVal        = 0x00;
    bool portB          = false;
    get_Adr_byteVal_Port(strGPIOPin_Dev, strAddr, byteVal, portB);
    if(val == 0x01)
    {
        if(portB) _gpioPortB = _gpioPortB | byteVal;
        else      _gpioPortA = _gpioPortA | byteVal;
    }
    else
    { 
        if(portB) _gpioPortB = _gpioPortB &~ byteVal;
        else      _gpioPortA = _gpioPortA &~ byteVal;
    }
    int addrIndex = atoi(strAddr.c_str());
    std::string strCommand = "S " + _devAdrVec[addrIndex - 1] + " P";
    interface->write_Serial(strCommand);
    std::string strPort  = hexTable[GPIOA];
    byte gpioPort        = _gpioPortA;
    if(portB)
    {
        strPort = hexTable[GPIOB];
        gpioPort = _gpioPortB;
    }
    strCommand = "S " + strPort + " " + hexTable[gpioPort] + " P";
    interface->write_Serial(strCommand);
}

byte MCP23017::getPin(std::string strGPIOPin_Dev)
{
    byte pinState = 0xff;
    std::string strAddr = "";
    byte byteVal        = 0x00;
    bool portB          = false;
    get_Adr_byteVal_Port(strGPIOPin_Dev, strAddr, byteVal, portB);

    if(portB)
    {

    }
    else
    {
        
    }

    //todo
    //check reading port via serialInterface

    return pinState;   
}

/*

/*
mymap.insert(pair<int,vector<int> >(10, vector<int>()));

You could then add whatever elements you want with something like:

mymap[10].push_back(1);
mymap[10].push_back(2);

*/