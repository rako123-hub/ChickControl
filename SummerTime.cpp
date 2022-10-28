#include <cstdlib>
#include <iostream> 

#include "SummerTime.h"

SummerTime::SummerTime()
{

}

SummerTime::~SummerTime()
{
    if (_thread != nullptr)
    {
        if (_thread->joinable())
        {
            _thread->join();
        }
    }
}

void SummerTime::createTimeThread()
{
   if(_thread == nullptr)
   {
      _thread = new std::thread(&SummerTime::checkTimeChange, this);
   }
    
}


void SummerTime::checkTimeChange()
{
    while(true)
    {
        std::printf("Thread  running\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

