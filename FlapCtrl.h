#ifndef FLAPCTRL_H
#define FLAPCTRL_H

#include <cstdlib>
#include <thread>
#include "TimeOpenClose.h"


//enum FlapState {OPEN, CLOSE, UNDEFIENED};


struct FlapData
{
    std::string time_open;
    std::string time_close;
    uint8_t     state;
};

class FlapCtrl
{
    public:
    FlapCtrl();
    ~FlapCtrl();

    private:
    void readFlapCtrlConfiguration();
    void initThread();
    void doRun();
    TimeOpenClose *_timeopenclose;
    
    FlapData _flapData;

    public:
    void doWork();

};

#endif