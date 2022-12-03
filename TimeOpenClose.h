#ifndef TIMEOPENCLOSE_H
#define TIMEOPENCLOSE_H

#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

class TimeOpenClose
{
    public:
    TimeOpenClose();
    TimeOpenClose(std::string open, std::string close);
    TimeOpenClose(std::string open, std::string close, std::string dimm_on, std::string _dimm_off, int maxDimmSteps);
    bool detectOpenCloseTime();             //return true is open state
    bool detectDimmTimeOFF();                 //return true we are dimming
    bool detectDimmTimeON();                 //return true we are dimming
    int getDimmOFFStep();
    void resetDimmSteps();
    void setOpen(std::string open)       {_open         = open;}
    void setClose(std::string close)     {_close        = close;}
    void setDimmOn(std::string dimmON)   {_dimm_on      = dimmON;}
    void setDimmOff(std::string dimmOFF) {_dimm_off     = dimmOFF;}
    void setMaxDimmSteps(int maxSteps)   {_maxDimmSteps = maxSteps;}
    private:
    std::string _open;
    std::string _close;   
    std::string _dimm_on;
    std::string _dimm_off;
    int _maxDimmSteps;
    int _actDimmStep = 0;
    
    int  getSeconds(std::string strTime);
    int  getSeconds();
};

#endif