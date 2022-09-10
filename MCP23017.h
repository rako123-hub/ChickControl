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


enum Gpio_Direction {OUTPUT, INPUT, TRISTATE};               // outpout = 0, input = 1

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
    void set_MCD230127_DirectionPins();    
    void checkConnectedDevices();                                           // only Input/Output dir saved
    std::map<std::string, std::vector<std::string>> _gpio_Adr_Dir_Map;     // key Dev Adress with vector Input/Output saved
    std::vector<std::string> _devAdrVec;                                   //asociate DevNumber with its DevAddress
    Serial_I2C_Interface *interface  = nullptr;

    public:
    void setOutputPin(std::string gpioPin, byte value);
    byte getPin(std::string gpioPin);
};



#endif