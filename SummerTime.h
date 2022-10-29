#ifndef SUMMERTIME_H
#define SUMMERTIME_H

#include <thread>
#include <cstdlib>
#include <chrono>


class SummerTime
{
    public:
    SummerTime();
    ~SummerTime();

    public:
    bool getSummerTimeChange();
    void storeNewRTCTime();

    private:
    void createTimeThread();

    std::thread *_thread = nullptr;
    void checkTimeChangeThread();
    std::chrono::time_point<std::chrono::system_clock> oldTime;
    bool summer_winter_change = false;

};



#endif