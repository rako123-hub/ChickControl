#ifndef LIGHTCTRL_H
#define LIGHTCTRL_H

#include <string>
#include <vector>
#include "GlobalDefs.h"
#include "TimeOpenClose.h"
#include "MCP23017.h"


struct LightData
{
    std::string time_on;
    std::string time_off;
    std::string time_dimm_on;
    std::string time_dimm_off;
};


class LightCtrl
{
    public:
    LightCtrl();
    LightCtrl(MCP23017 *mcp);
    ~LightCtrl();

    private:
    byte _intervall_count;
    byte _lightState    = LightState::OFF;
    byte _oldLightState = LightState::UNDEF;

    void readLightConfiguration();
    void getLightState();
    bool getTimeOpenInterval();

    std::string _light_Clock_FF;
    std::string _light_Clear_FF;
    std::string _light_Data_FF;
    int _maxDimmSteps;
    int _actDimmStep = 0;
    std::vector<LightData> _lightDataVec;  
    void doClock(std::string strPin);
    void lightOff();
    void lightOn();
    void dimmOff();
    void dimmOn();
    void printLightState();
    TimeOpenClose *timeOpenClose = nullptr;
    MCP23017      *mcp23017      = nullptr;

    public:
    void doWork();

};

#endif