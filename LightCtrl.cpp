

#include "LightCtrl.h"
#include "ChickenConfiguration.h"

const std::string config_section_Light     = "[Light]";
const short       default_LightIntervall   = 1;

LightCtrl::LightCtrl()
{
    readLightConfiguration();
    timeOpenClose = new TimeOpenClose();
}

LightCtrl::LightCtrl(MCP23017 *mcp23017Device)
:mcp23017(mcp23017Device)
{
    readLightConfiguration();
    timeOpenClose = new TimeOpenClose();
}

LightCtrl::~LightCtrl()
{
    if(timeOpenClose != nullptr)
    {
        delete timeOpenClose;
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
        LightData lightData;
        strValue.empty();
        key = "light_on_" + std::to_string(i);
        if(chickConfig.getValue(key, strValue)) lightData.time_on = strValue;
        key = "light_off_" + std::to_string(i);
        if(chickConfig.getValue(key, strValue)) lightData.time_off = strValue;
        key = "light_dimm_on_" + std::to_string(i);
        if(chickConfig.getValue(key, strValue)) lightData.time_dimm_on = strValue;
        key = "light_dimm_off_" + std::to_string(i);
        if(chickConfig.getValue(key, strValue)) lightData.time_dimm_off = strValue;
        _lightDataVec.emplace_back(lightData);      
    }
    key = "light_Clear_FF";
    strValue.empty();
    if(chickConfig.getValue(key, strValue)) _light_Clear_FF = strValue;
    strValue.empty();
    key = "light_Clock_FF";
    if(chickConfig.getValue(key, strValue)) _light_Clock_FF = strValue;
    key = "dimm_steps";
    iValue = 0;
    if(chickConfig.getValue(key, iValue)) _maxDimmSteps = iValue;
}  

void LightCtrl::doWork()
{
    //set lightState ON, OFF, DIMM_ON, DIMM_OFF
    _lightState = LightState::OFF;
    if (getTimeOpenInterval())
    {
       getLightState();
    }
    if(_oldLightState != _lightState)
    {
       _oldLightState = _lightState;
       switch(_lightState)
       {
           case LightState::ON:
             lightOn();
             break;

           case LightState::OFF:
             lightOff();
             break;
        
           case LightState::DIMM_ON:
              dimmOn();
              break;  

           case LightState::DIMM_OFF:
              dimmOff();
              break;

           default:
              break;
       }
    }
    printLightState();
}

bool LightCtrl::getTimeOpenInterval()
{
    bool interval = false;
    for( LightData data : _lightDataVec)
    {
        timeOpenClose->setOpen(data.time_on);
        timeOpenClose->setClose(data.time_off);
        timeOpenClose->setDimmOn(data.time_dimm_on);
        timeOpenClose->setDimmOff(data.time_dimm_off);   
        if (timeOpenClose->detectOpenCloseTime())
        {
           interval = true;
           timeOpenClose->setMaxDimmSteps(_maxDimmSteps);
           break; 
        }  
    }
    return interval;
}

void LightCtrl::getLightState()
{
    if (timeOpenClose->detectDimmTimeON())
    {
        _lightState = LightState::DIMM_ON;
    } 
    else if(timeOpenClose->detectDimmTimeOFF())
    {
       _lightState = LightState::DIMM_OFF; 
    }   
    else
    {
       _lightState = LightState::ON;
    }  
}

void LightCtrl::doClockFF(std::string strClk)
{
        mcp23017->setOutputPin(strClk, LOW);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        mcp23017->setOutputPin(strClk, HIGH);
}

void LightCtrl::lightOn()
{
    mcp23017->setOutputPin(_light_Clear_FF, HIGH);
    doClockFF(_light_Clock_FF);
    timeOpenClose->resetDimmSteps();

}

void LightCtrl::lightOff()
{
    mcp23017->setOutputPin(_light_Clear_FF, LOW);
    timeOpenClose->resetDimmSteps();
}

void LightCtrl::dimmOff()
{
    int dimmStep = timeOpenClose->getDimmOFFStep();
 
    if(dimmStep != _actDimmStep)
    {
        _actDimmStep = dimmStep;
        doClockFF(_light_Clock_FF);
    }
}

void LightCtrl::dimmOn()
{
    
}

void LightCtrl::printLightState()
{
    switch(_lightState)
    {
        case LightState::ON:
            printf("*******LIGHT ON\n");
            break;

        case LightState::OFF:
            printf("*******LIGHT OFF\n");
            break;
        
        case LightState::DIMM_ON:
            printf("*******LIGHT DIMMON DimmStep:%d\n", _actDimmStep);
            break;  

        case LightState::DIMM_OFF:
            printf("*******LIGHT DIMMOFF DimmStep:%d\n", _actDimmStep);
            break;

        default:
            printf("*******LIGHT default\n");
            break;
    }
}