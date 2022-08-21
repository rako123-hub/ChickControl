#include <iostream> 
#include "Nestauto.h"
#include "ChickenConfiguration.h"


const std::string default_time_open  = "08:00:00";
const std::string default_time_close = "17:00:00";
const std::string config_section     = "[Nestauto]";
const short       default_cylinder   = 1;

Nestauto::Nestauto()
{
    _nestData.time_open  = default_time_open;
    _nestData.time_close = default_time_close;
    _nestData.cylinders  = default_cylinder;
    readNestautoConfiguration();
    doWork();
}

Nestauto::~Nestauto()
{
    if (_timeopenclose != nullptr)
    {
        delete _timeopenclose;
    }
}

void Nestauto::readNestautoConfiguration()
{
    ChickenConfiguration chickConfig(config_section);
    std::string key = "net_open";
    std:: string strValue;
    if(chickConfig.getValue(key, strValue)) _nestData.time_open = strValue;
    key = "net_close";
    if(chickConfig.getValue(key, strValue)) _nestData.time_close = strValue;
    key = "Cylinder"; 
    int iValue;
    if(chickConfig.getValue(key, iValue)) _nestData.cylinders = iValue;
    _timeopenclose = new TimeOpenClose (_nestData.time_open, _nestData.time_close);
} 

void Nestauto::doWork()
{
   _nestData.state = State::UNDEFIENED;
   bool open = _timeopenclose->detectOpenCloseTime();

   if(open) _nestData.state = State::OPEN;
   else     _nestData.state = State::CLOSE;
}


void Nestauto::doOpen()
{
    std::cout << "+++DoOpen() \n";
}

void Nestauto::doClose()
{
    
}