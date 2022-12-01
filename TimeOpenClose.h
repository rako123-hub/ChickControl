#ifndef TIMEOPENCLOSE_H
#define TIMEOPENCLOSE_H

#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>

class TimeOpenClose
{
    public:
    TimeOpenClose();
    TimeOpenClose(std::string open, std::string close);
    TimeOpenClose(std::string open, std::string close, std::string dimmOn, std::string dimmOff);
 //   bool getOpenState();
    bool detectOpenCloseTime();             //return true is open state
    
    private:
    std::string _open;
    std::string _close;   
    std::string _dimm_on;
    std::string _dimm_off;
    
    int  getSeconds(std::string strTime);
    int  getSeconds();
};

#endif