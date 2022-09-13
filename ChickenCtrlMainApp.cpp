#include <cstdlib>
#include <iostream> 
#include <chrono>
#include <thread>

#include "GlobalDefs.h"
#include "Nestauto.h"
#include "FlapCtrl.h"
#include "MCP23017.h"
#include "LightCtrl.h"
#include "Serial_USB_I2C_Interface.h"
#include "I2C_Utils.h"

#include "ChickenConfiguration.h"

std::string version("0.0.1");

int main(int argc, char *argv[])
{
   printf("ChickenControl %s is starting \n" ,version.c_str());
   
   Serial_I2C_Interface *interface = new Serial_I2C_Interface();
   if(interface->getInitOK())
   {
//      I2CUtils utils(interface);
       MCP23017 *mcp = new MCP23017(interface);
       LightCtrl light(mcp);
 //      Nestauto nest;
 //     FlapCtrl flap;
 //      

      while(true)
      {
         printf("main:: while loop\n");
         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  //       nest.doWork();
   //      flap.doWork();
         light.doWork();
         break;
      }

   }
   std::cout << "Exit Chicken App";
   std::cout << "\n \n";
   
   return EXIT_SUCCESS; // return value
}