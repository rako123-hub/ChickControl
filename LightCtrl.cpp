

#include "LightCtrl.h"
#include "ChickenConfiguration.h"

const std::string config_section_Light     = "[Light]";
const short       default_LightIntervall   = 1;

LightCtrl::LightCtrl()
{
    readLightConfiguration();
}
LightCtrl::~LightCtrl()
{
    for(TimeOpenClose *timeOpenClose : _ptrTimeOpenCloseVec)
    {
        if(timeOpenClose != nullptr)
        {
            delete timeOpenClose;
        }
    }
}

void LightCtrl::readLightConfiguration()
{
    ChickenConfiguration chickConfig(config_section_Light);
    std::string key = "intervall_count";
    std::string strValue;
    int iValue = 0;
    if(chickConfig.getValue(key, iValue)) _intervall_count = iValue;

    for(int i = 1; i <= _intervall_count; i++)
    {
        strValue.empty();
        key = "light_on_" + std::to_string(i);
        if(chickConfig.getValue(key, strValue)) _lightData.time_on = strValue;
        key = "light_off_" + std::to_string(i);
        if(chickConfig.getValue(key, strValue)) _lightData.time_off = strValue;
        _lightDataVec.emplace_back(_lightData);      
    }
    key = "light_IO";
    if(chickConfig.getValue(key, strValue)) _light_IO = strValue;
}  

void LightCtrl::initTime()
{
   
   TimeOpenClose *timeOpenClose = nullptr;
   
   for(LightData data : _lightDataVec)
   {
       timeOpenClose = new TimeOpenClose(data.time_on, data.time_off);
       if(timeOpenClose != nullptr)
       {
           _ptrTimeOpenCloseVec.emplace_back(timeOpenClose);
       }
   }
}

void LightCtrl::doWork()
{
    _state = LightState::OFF;
    for(TimeOpenClose *timeOpenClose : _ptrTimeOpenCloseVec)
    {
        bool lighton = timeOpenClose->detectOpenCloseTime();
        if (lighton)          // if light is on at the first intervall break and swtch on the light otherwise switch on the light at second intervall
        {
            _state = LightState::ON;
            break;
        }
    }
    if(_oldState != _state)
    {
        _oldState = _state; 

        //TODO
        //switch on/off the light --> access to HW MCP1407 IO
    }
}