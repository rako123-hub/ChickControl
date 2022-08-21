#include <cstdlib>
#include <iostream> 
#include <chrono>
#include <thread>

#include "Nestauto.h"
#include "FlapCtrl.h"
#include "MCP23017.h"
#include "Serial_USB_I2C_Interface.h"

#include "ChickenConfiguration.h"


int main(int argc, char *argv[])
{
   std::cout << "Huhn Control is starting \n";
   
   MCP23017 mcp;
   Nestauto nest;
   FlapCtrl flap;
   Serial_I2C_Interface interface;

   while(true)
   {
      printf("main:: while loop\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      nest.doWork();
      flap.doWork();

      break;
   }

   std::cout << "Exit Huhn App";
   std::cout << "\n \n";
   
   return EXIT_SUCCESS; // return value
}