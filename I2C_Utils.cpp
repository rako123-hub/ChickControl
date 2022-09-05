

#include "I2C_Utils.h"




I2CUtils::I2CUtils(Serial_I2C_Interface serialInterface)
:interface(&serialInterface)
{
    scanDevices();
}

I2CUtils::~I2CUtils()
{}


void I2CUtils::scanDevices()
{
   char buf[256];
   char recvbuf[256];
   char hexBuf[2];

   //Adresse 0x4E   

   //for (int i = 2; i < 255; i+=2)
   for (int i = 1; i < 255; i++)
   {

     std::sprintf(buf, "%02X", i);

     std::string str = "S ";
     str.append(buf);
     str.append(" 00 P");

     printf("%s\n", str.c_str());

     strcpy(buf, str.c_str());
     interface->write_Serial(buf);
     interface->read_Serial(recvbuf);

     printf("recv:: %s\n", recvbuf);
     std::string rec(recvbuf);

   }
   
}


