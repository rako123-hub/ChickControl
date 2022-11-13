#ifndef DS3231_H
#define DS3231_H

#include <iostream> 
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <errno.h>              // Error integer and strerror() function
#include <cerrno>
#include <cstring>
#include <clocale>
#include <chrono>
 
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port

typedef uint8_t byte;


#define REG_SEC    0x00
#define REG_MIN    0x01
#define REG_HOUR   0x02
#define REG_DAY    0x04
#define REG_MONTH  0x05
#define REG_YEAR   0x06

#define REG_MASK   0x0f

class DS3231
{
    public:
    DS3231();
    ~DS3231();

    void getlocalTime_setDS3231();
    void getDS3231_setLocalTime();
    void getRTC();

    private:
    bool openDS3231();
    bool setDevAdr(byte addr);
    bool writeData(byte reg, byte value);
    byte readData(byte reg);
    void getlocalTime();

    byte getSecond();
    byte getMinute();
    byte getHour();
    byte getDay();
    byte getMonth();
    byte getYear();

    void setClock(byte reg, byte val);

    int _devI2C;
    bool _open;
    
};


#endif