
#include <iostream> 

#include "FlapCtrl.h"
#include "GlobalDefs.h"
#include "ChickenConfiguration.h"

const std::string default_time_open  = "08:00:00";
const std::string default_time_close = "17:00:00";
const std::string config_section     = "[Flap]";

FlapCtrl::FlapCtrl()
{
    _flapData.time_open  = default_time_open;
    _flapData.time_close = default_time_close;
    readFlapCtrlConfiguration();
}

FlapCtrl::~FlapCtrl()
{
    if (_timeopenclose != nullptr)
    {
        delete _timeopenclose;
    }
}

void FlapCtrl::readFlapCtrlConfiguration()
{
    ChickenConfiguration chickConfig(config_section);
    std::string key = "flap_open";
    _flapData.time_open  = chickConfig.getValue(key);
    key = "flap_close";
    _flapData.time_close = chickConfig.getValue(key);
    _timeopenclose = new TimeOpenClose (_flapData.time_open, _flapData.time_close);
} 

void FlapCtrl::doWork()
{
   _flapData.state = State::UNDEFIENED;
   bool open = _timeopenclose->detectOpenCloseTime();

   if(open) _flapData.state = State::OPEN;
   else     _flapData.state = State::CLOSE;
}
