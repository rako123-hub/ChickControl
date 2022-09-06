

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
    if (_timeopenclose != nullptr)
    {
        delete _timeopenclose;
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
   // _timeopenclose = new TimeOpenClose (_nestData.time_open, _nestData.time_close);
}

void LightCtrl::doWork()
{
    
}