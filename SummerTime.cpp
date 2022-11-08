#include <cstdlib>
#include <iostream> 

#include "SummerTime.h"

SummerTime::SummerTime()
{
    createTimeThread();

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
      _thread = new std::thread(&SummerTime::checkTimeChangeThread, this);
   }
    
}

void SummerTime::checkTimeChangeThread()
{
    oldTime = std::chrono::system_clock::now();
    summer_winter_change = false;
    while(true)
    {
        std::printf("SummerTime Thread  running\n");
        std::this_thread::sleep_for(std::chrono::seconds(10));
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
//        std::time_t time_now = std::chrono::system_clock::to_time_t(now); 

        int diff = std::chrono::duration_cast<std::chrono::seconds>(now - oldTime).count();

        printf("diffTime SummerTime check :: %d\n", diff);

        if (diff > 100)
        {
            summer_winter_change = true;

        }
        else if(diff < -100)
        {
            summer_winter_change = true;
        }
        oldTime = std::chrono::system_clock::now();
    }
}

bool SummerTime::getSummerTimeChange()
{
    return summer_winter_change;
}

void SummerTime::storeNewRTCTime()
{
    std::string strCommand = "/usr/local/bin/chickApp/TimeGetSetMainApp getLocal_setRTC";
    const char *command = strCommand.c_str();
    system(command); 
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    summer_winter_change = false;
}


