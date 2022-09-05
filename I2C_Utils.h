#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include<iostream> 
#include<cstring>

#include "Serial_USB_I2C_Interface.h"

class I2CUtils
{
    public:
    I2CUtils() {};
    I2CUtils(Serial_I2C_Interface interface);
    ~I2CUtils();

    private:
    void scanDevices();


     Serial_I2C_Interface *interface  = nullptr;

};

#endif