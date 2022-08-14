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
    _nestData.time_open  = chickConfig.getValue(key);
    key = "net_close";
    _nestData.time_close = chickConfig.getValue(key);
    key = "Cylinder";
    _nestData.cylinders  = std::atoi(chickConfig.getValue(key).c_str());
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

bool Nestauto::checkTimeString()
{
    //bool v2(const std::string& nom)

    //static const std::regex re{"[a-zA-Z][a-zA-Z0-9_]*"};
    //return std::regex_match(nom, re);

     // Regex to check valid time in 24-hour format
   const std::regex time_pattern("([01]?[0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]");

   /*
       ( represents the start of the group.
                [01]?[0-9] represents the time starts with 0-9, 1-9, 00-09, 10-19.
            | represents or.
               2[0-3] represents the time starts with 20-23.
        ) represents the end of the group. 
     
           : represents the time should be followed by a colon(:).
            [0-5][0-9] represents the time followed by 00 to 59
    */



   if ( !regex_match(_nestData.time_open, time_pattern))
   {
       std::cout << "wrong time open configuration -> use default value \n";
   }

   if ( !regex_match(_nestData.time_close, time_pattern))
   {
       std::cout << "wrong time close configuration -> use default value \n";
   }

   return true;

}