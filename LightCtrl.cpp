

#include "LightCtrl.h"
#include "ChickenConfiguration.h"

const std::string config_section_Light     = "[Light]";
const short       default_LightIntervall   = 1;

LightCtrl::LightCtrl()
{
    readLightConfiguration();
}

LightCtrl::LightCtrl(MCP23017 *mcp23017Device)
:mcp23017(mcp23017Device)
{
    readLightConfiguration();
    initTime();
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
        key = "light_dimm_on_" + std::to_string(i);
        if(chickConfig.getValue(key, strValue)) _lightData.time_dimm_on = strValue;
        key = "light_dimm_off_" + std::to_string(i);
        if(chickConfig.getValue(key, strValue)) _lightData.time_dimm_off = strValue;

        _lightDataVec.emplace_back(_lightData);      
    }
    key = "light_Clear_FF";
    strValue.empty();
    if(chickConfig.getValue(key, strValue)) _light_Clear_FF = strValue;
    strValue.empty();
    key = "light_Clock_FF";
    if(chickConfig.getValue(key, strValue)) _light_Clock_FF = strValue;
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
    for(TimeOpenClose *timeOpenClose : _ptrTimeOpenCloseVec)
    {
        bool lighton = timeOpenClose->detectOpenCloseTime();
        if (lighton)          // if light is on at the first intervall break and swtch on the light otherwise switch on the light at second intervall
        {
            _state = LightState::DIMM_ON;
            break;
        }
        else
        {
            _state = LightState::DIMM_OFF;
        }
    }
    if(_oldState != _state)
    {
        _oldState = _state; 
        if(mcp23017 != nullptr)
        {
            if (_state == LightState::OFF)
            {
                mcp23017->setOutputPin(_light_Clear_FF, LOW);
            }
            else if (_state == LightState::ON)
            {
                mcp23017->setOutputPin(_light_Clear_FF, HIGH);
                doClockFF(_light_Clock_FF);
            }
            //mcp23017->setOutputPin(_light_IO, _state);

            /* implemeted for quick release*/
           // mcp23017->setOutputPin(_state); // this is the setoutputpin fpr all pins -->0xff

        }
    }

    while(true)
    {
        mcp23017->setOutputPin(_light_Clock_FF, LightState::OFF);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        mcp23017->setOutputPin(_light_Clock_FF, LightState::ON);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));

    }
}

void LightCtrl::doClockFF(std::string strClk)
{
        mcp23017->setOutputPin(strClk, LOW);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        mcp23017->setOutputPin(strClk, HIGH);
}