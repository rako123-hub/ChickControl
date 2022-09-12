#ifndef MCP23017_H
#define MCP23017_H

#include<iostream> 
#include<vector>
#include<map>
#include <utility>

#include "GlobalDefs.h"
#include "ChickenConfiguration.h"
#include "Serial_USB_I2C_Interface.h"

#define GPIO_COUNT     16
#define GPIO_PORTS      2      // Port A and Port B

#define GPIO0      0x01;    /* 0000 0001 */ // PORT_A 
#define GPIO1      0x02;    /* 0000 0010 */ // PORT_A 
#define GPIO2      0x04;    /* 0000 0100 */ // PORT_A 
#define GPIO3      0x08;    /* 0000 1000 */ // PORT_A 
#define GPIO4      0x10;    /* 0001 0000 */ // PORT_A 
#define GPIO5      0x20;    /* 0010 0000 */ // PORT_A 
#define GPIO6      0x40;    /* 0100 0000 */ // PORT_A 
#define GPIO7      0x80     /* 1000 0000 */ // PORT_A 

#define GPIO8      0x01;    /* 0000 0001 */ // PORT_B 
#define GPIO9      0x02;    /* 0000 0010 */ // PORT_B 
#define GPIO10     0x04;    /* 0000 0100 */ // PORT_B 
#define GPIO11     0x08;    /* 0000 1000 */ // PORT_B 
#define GPIO12     0x10;    /* 0001 0000 */ // PORT_B 
#define GPIO13     0x20;    /* 0010 0000 */ // PORT_B 
#define GPIO14     0x40;    /* 0100 0000 */ // PORT_B 
#define GPIO15     0x80;    /* 1000 0000 */ // PORT_B 

//GPIO CONFIG DEFAULT BANK = 0
#define IODIRA     0x00   /* Data register Port A */
#define IODIRB     0x01   /* Data register Port B */
#define IPOLA      0x02   /* Register Logic change polarity Active High, Active Low Port A */
#define IPOLB      0x03   /* Register Logic change polarity Active High, Active Low Port B */
#define GPINTENA   0x04 
#define GPINTENB   0x05
#define DEFVALA    0x06  
#define DEFVALB    0x07
#define INTCONA    0x08   
#define INTCONB    0x09  
#define IOCONA     0x0A   
#define IOCONB     0x0B  
#define GPPUA      0x0C   /* Internal Pull-Up-Resistors Port A */  
#define GPPUB      0x0D   /* Internal Pull-Up-Resistors Port B */  
#define INTFA      0x0E    
#define INTFB      0x0F
#define INTCAPA    0x10  
#define INTCAPB    0x11  
#define GPIOA      0x12    
#define GPIOB      0x13
#define INTPOL     1    
#define INTODR     2
#define MIRROR     6    

//#define SPI_READ    0x01

//static const char* hexTable[256] = 
static std::string hexTable[256] =
{
    "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0a", "0b", "0c", "0d", "0e", "0f", "10", "11",
    "12", "13", "14", "15", "16", "17", "18", "19", "1a", "1b", "1c", "1d", "1e", "1f", "20", "21", "22", "23",
    "24", "25", "26", "27", "28", "29", "2a", "2b", "2c", "2d", "2e", "2f", "30", "31", "32", "33", "34", "35",
    "36", "37", "38", "39", "3a", "3b", "3c", "3d", "3e", "3f", "40", "41", "42", "43", "44", "45", "46", "47",
    "48", "49", "4a", "4b", "4c", "4d", "4e", "4f", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59",
    "5a", "5b", "5c", "5d", "5e", "5f", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6a", "6b",
    "6c", "6d", "6e", "6f", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7a", "7b", "7c", "7d",
    "7e", "7f", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8a", "8b", "8c", "8d", "8e", "8f",
    "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9a", "9b", "9c", "9d", "9e", "9f", "a0", "a1",
    "a2", "a3", "a4", "a5", "a6", "a7", "a8", "a9", "aa", "ab", "ac", "ad", "ae", "af", "b0", "b1", "b2", "b3",
    "b4", "b5", "b6", "b7", "b8", "b9", "ba", "bb", "bc", "bd", "be", "bf", "c0", "c1", "c2", "c3", "c4", "c5",
    "c6", "c7", "c8", "c9", "ca", "cb", "cc", "cd", "ce", "cf", "d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
    "d8", "d9", "da", "db", "dc", "dd", "de", "df", "e0", "e1", "e2", "e3", "e4", "e5", "e6", "e7", "e8", "e9",
    "ea", "eb", "ec", "ed", "ee", "ef", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "fa", "fb",
    "fc", "fd", "fe", "ff"
};



enum GPIO_DIR {OUTPUT, INPUT, TRISTATE};               // outpout = 0, input = 1

class MCP23017
{
    public:
    MCP23017();
    MCP23017(Serial_I2C_Interface interface);
    ~MCP23017();

    private:
    void readMCP23017_Configuration();
    void readMCP23017_Dir_Config(ChickenConfiguration *chickConfig, std::string strVal, int devNum);
    void init_MCP23017_Devices();
    void set_MCP230127_DirectionPins();    
    void checkConnectedDevices();  
    void deleteNotConnectedDevices(std::vector<std::string>);                                         
    std::map<std::string, std::vector<std::string>> _gpio_Adr_Dir_Map;     // key Dev Adress with vector Input/Output saved
    std::vector<std::string> _devAdrVec;                                   //asociate DevNumber with its DevAddress
    std::vector<std::string> _connectedDevsVec;                               
    Serial_I2C_Interface *interface  = nullptr;

    public:
    void setOutputPin(std::string gpioPin, byte value);
    byte getPin(std::string gpioPin);
};



#endif