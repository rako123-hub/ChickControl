#ifndef FLAPCTRL_H
#define FLAPCTRL_H

#include <cstdlib>
#include <thread>
#include "TimeOpenClose.h"
#include "GlobalDefs.h"


struct FlapData
{
    std::string time_open;
    std::string time_close;
};

class FlapCtrl
{
    public:
    FlapCtrl();
    ~FlapCtrl();

    private:
    void readFlapCtrlConfiguration();
 //   void initThread();
    void doRun();
    void initTime();
    TimeOpenClose *_timeopenclose;
    
    FlapData _flapData;
    byte _state    = State::UNDEFIENED;
    byte _oldState = State::UNDEFIENED;

    public:
    void doWork();

};

#endif