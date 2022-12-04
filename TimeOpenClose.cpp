#include <cstdlib>
#include <iostream> 


#include "TimeOpenClose.h"

TimeOpenClose::TimeOpenClose()
{}

TimeOpenClose::TimeOpenClose(std::string open, std::string close)
:_open(open), _close(close)
{}

TimeOpenClose::TimeOpenClose(std::string open, std::string close, std::string dimm_on, std::string _dimm_off, int maxDimmSteps)
:_open(open), _close(close), _dimm_on(dimm_on), _dimm_off(_dimm_off), _maxDimmSteps(maxDimmSteps)
{}

//time per day
//                 1. Intervall                                        2. Intervall
//   CLOSE           O P E N                        CLOSE               O P E N                    CLOSE
//   CLOSE ( _open _dimm_on    _close _dimm_off )   CLOSE   (_open  _dimm_on    _close _dimm_off)  CLOSE

bool TimeOpenClose::detectOpenCloseTime()
{
    bool open = false;
    int seconds_now, seconds_open, seconds_close, seconds_dimmoff = 0;
    
    seconds_now     = getSeconds();
    seconds_open    = getSeconds(_open);
    seconds_close   = getSeconds(_close);
    seconds_dimmoff = getSeconds(_dimm_off);

    printf("SecondsNow              seconds: %d\n", seconds_now);
    printf("Openstring:  %s , seconds: %d\n", _open.c_str(), seconds_open);
    printf("Closestring: %s , seconds: %d\n", _close.c_str(), seconds_close);

    if((seconds_now > seconds_open) && (seconds_now < (seconds_close + seconds_dimmoff)))
    {
        open = true;
    }
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

bool TimeOpenClose::detectDimmTimeOFF()
{
    bool detectDimmOFF = false;
    int seconds_now, seconds_close, seconds_dimmOFF = 0;

    seconds_close   = getSeconds(_close);
    seconds_dimmOFF = getSeconds(_dimm_off);
    seconds_now     = getSeconds();

    if((seconds_now > seconds_close) && (seconds_now < (seconds_close + seconds_dimmOFF)))
    {
        detectDimmOFF = true;
    }
    return detectDimmOFF;    
}

bool TimeOpenClose::detectDimmTimeON()
{
    bool detectDimmON = false;
    int seconds_now, seconds_open, seconds_dimmON = 0;

    seconds_open   = getSeconds(_open);
    seconds_dimmON = getSeconds(_dimm_on);
    seconds_now    = getSeconds();

    if((seconds_now > seconds_open) && (seconds_now < (seconds_open + seconds_dimmON)))
    {
        detectDimmON = true;
    }
    return detectDimmON;
}

int TimeOpenClose::getDimmOFFStep()
{
    int seconds_now, seconds_close, seconds_dimmOFF, seconds_step_interval = 0;
    if(_maxDimmSteps > 0)
    {
       seconds_close = getSeconds(_close);
       seconds_dimmOFF = getSeconds(_dimm_off);
       seconds_step_interval = seconds_dimmOFF / _maxDimmSteps;
       seconds_now = getSeconds();

       printf("secondsDIMMOFF %d\n", seconds_dimmOFF);
       printf("seconds_step_interval %d\n", seconds_step_interval);

       if(seconds_now > (seconds_close + seconds_dimmOFF))
       {
           _actDimmStep = -1;
       }
       else if( seconds_now > (seconds_close + seconds_step_interval * _actDimmStep))
       {
            _actDimmStep++;
            if(_actDimmStep > _maxDimmSteps) _actDimmStep = -1;
       }
    }
    return _actDimmStep;
}

void TimeOpenClose::resetDimmSteps()
{
    _actDimmStep = 0;
}

    

