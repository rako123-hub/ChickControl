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
#include "SummerTime.h"
#include "DS3231.h"

std::string version("2.0.2");

MCP23017   *mcp    = nullptr;
LightCtrl  *light  = nullptr;
SummerTime *summer = nullptr;
DS3231     *rtc    = nullptr;

void deleteControlObjects()
{
  
   if(light != nullptr)
   {
      delete light;
      light = nullptr;
   }

   if(mcp != nullptr)
   {
      delete mcp;
      mcp = nullptr;
   }
}

void restartControlObjects()
{
   mcp = new MCP23017();
   light = new LightCtrl(mcp);
}

int main(int argc, char *argv[])
{
   printf("ChickenControl %s is starting \n" ,version.c_str());
/*
   rtc    = new DS3231();
   rtc->getDS3231_setLocalTime();
   if(rtc != nullptr)
   {
      delete rtc;
      rtc = nullptr;
   }
*/
   mcp    = new MCP23017();
   light  = new LightCtrl(mcp);
   summer = new SummerTime();
  
   while(true)
   {
      printf("main:: while loop\n");
      std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  //      nest.doWork();
   //     flap.doWork();
      if (light != nullptr) light->doWork();
      if(summer->getSummerTimeChange())
      {
         printf("SummerTime change detect!\n");
         deleteControlObjects();
         summer->storeNewRTCTime();
         std::this_thread::sleep_for(std::chrono::milliseconds(1000));
         restartControlObjects();
      }
//   break; 
   }

   deleteControlObjects();
   if(summer != nullptr)
   {
      delete summer;
      summer = nullptr;
   }
  
   printf("Exit Chicken App\n");
   
   return EXIT_SUCCESS; // return value
}