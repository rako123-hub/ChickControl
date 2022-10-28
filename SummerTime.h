#ifndef SUMMERTIME_H
#define SUMMERTIME_H

#include <thread>
#include <cstdlib>


class SummerTime
{
    public:
    SummerTime();
    ~SummerTime();

    private:
    void createTimeThread();

    std::thread *_thread = nullptr;
    void checkTimeChange();

};



#endif