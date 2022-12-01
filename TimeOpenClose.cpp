#include <cstdlib>
#include <iostream> 


#include "TimeOpenClose.h"

TimeOpenClose::TimeOpenClose()
{}

TimeOpenClose::TimeOpenClose(std::string open, std::string close)
:_open(open), _close(close)
{}

TimeOpenClose::TimeOpenClose(std::string open, std::string close, std::string dimmOn, std::string dimmOff)
:_open(open), _close(close), _dimm_on(dimmOn), _dimm_off(dimmOff)
{}

bool TimeOpenClose::detectOpenCloseTime()
{
    bool open = false;
    int seconds_now, seconds_open, seconds_close = 0;
    
    seconds_now   = getSeconds();
    seconds_open  = getSeconds(_open);
    seconds_close = getSeconds(_close);

    printf("SecondsNow              seconds: %d\n", seconds_now);
    printf("Openstring:  %s , seconds: %d\n", _open.c_str(), seconds_open);
    printf("Closestring: %s , seconds: %d\n", _close.c_str(), seconds_close);

    if ((seconds_now - seconds_open) > 0) open = true;
    if ((seconds_now - seconds_close) > 0) open = false;
     
    printf("\n");
    if (open) printf("*****OPEN*****\n");       
    else      printf("*****CLOSE****\n");
    printf("\n");
    
    return open;
}

int TimeOpenClose::getSeconds(std::string strTime)
{
    int hh, mm, ss, seconds = 0;
    sscanf(strTime.c_str(), "%2d:%2d:%2d", &hh, &mm, &ss);

  //  printf("hour: %d, min: %d, sec: %d\n", hh, mm, ss);

    seconds = hh * 3600 + mm * 60 + ss;

    return seconds;
}

int TimeOpenClose::getSeconds()
{
    struct tm *tm_now = {};
    int seconds_now   = 0;

    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now); 
    tm_now = localtime ( &time_now );
    seconds_now = tm_now->tm_hour * 3600 + tm_now->tm_min * 60 + tm_now->tm_sec;

    printf ( "Current date and time are: %s \n", asctime (tm_now) );

    return seconds_now;
}

    

