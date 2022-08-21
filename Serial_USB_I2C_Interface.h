#ifndef SERIAL_USB_I2C_INTERFACE_H
#define SERIAL_USB_I2C_INTERFACE_H

#include <cstdlib>
#include <string>
#include <termios.h> // Contains POSIX terminal control definitions

const std::string DEVINFO             = "?";
const std::string INTERFACEINITSTRING = "ELV USB-I2C-Interface";

struct Serial_I2C_Interface_Config
{
    std::string devName;
    int baudrate = 0;
    int busClock = 0;
};

class Serial_I2C_Interface
{
    public:
    Serial_I2C_Interface();
    ~Serial_I2C_Interface();

    private:
    int _serialUSB;

    private:
    void InitInterface();
    void readSerialDevConfiguration();
    void readMCP23017Configuration();
    void initSerialDevice();
    bool checkSerialDevInfo();
    void setBaudrate(termios *tty);
    void setI2C_Clock();

    Serial_I2C_Interface_Config _serialConfig;
    char _writeBuf[256];
    char _readBuf[256];
    
    public:
    void write_Serial(char *buf);
    void read_Serial(char *buf);
};
 

#endif