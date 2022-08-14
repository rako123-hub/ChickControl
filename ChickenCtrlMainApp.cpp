#include <cstdlib>
#include <iostream> 
#include <chrono>
#include <thread>

#include "Nestauto.h"
#include "FlapCtrl.h"
#include "Serial_USB_I2C_Interface.h"

#include "ChickenConfiguration.h"


int main(int argc, char *argv[])
{
   std::cout << "Huhn Control is starting \n";

   Nestauto nest = Nestauto();
   FlapCtrl flap = FlapCtrl();
   Serial_I2C_Interface interface = Serial_I2C_Interface();

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