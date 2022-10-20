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

#include "ChickenConfiguration.h"

std::string version("1.0.0");

int main(int argc, char *argv[])
{
   printf("ChickenControl %s is starting \n" ,version.c_str());
   MCP23017 *mcp = new MCP23017();

   LightCtrl *light = new LightCtrl(mcp);
   while(true)
   {
      printf("main:: while loop\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  //      nest.doWork();
   //     flap.doWork();
      light->doWork();
//   break; 
   }
   if(mcp != nullptr)
   {
      delete mcp;
   }
   if(light != nullptr)
   {
      delete light;
   }
   printf("Exit Chicken App\n");
   
   return EXIT_SUCCESS; // return value
}