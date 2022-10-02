#include <cstdlib>
#include <iostream> 
#include <ostream>
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

std::string version("0.0.2");

int main(int argc, char *argv[])
{
   printf("ChickenControl %s is starting \n" ,version.c_str());
 //  MCP23017_Interface_ser *mcp = nullptr;
   MCP23017 *mcp = new MCP23017();

   LightCtrl *light = nullptr;
 //  Serial_I2C_Interface *interface = new Serial_I2C_Interface();
      while(true)
      {
         printf("main:: while loop\n");
         std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  //       nest.doWork();
   //      flap.doWork();
   //      light->doWork();
         break;
      }
   if(mcp != nullptr)
   {
      delete mcp;
   }
   printf("Exit Chicken App\n");
   
   return EXIT_SUCCESS; // return value
}