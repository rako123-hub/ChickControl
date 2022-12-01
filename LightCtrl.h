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
    byte _state    = LightState::UNDEF;
    byte _oldState = LightState::UNDEF;

    void readLightConfiguration();

    std::vector<LightData> _lightDataVec;
    std::string _light_Clock_FF;
    std::string _light_Clear_FF;
    LightData _lightData;
    std::vector<TimeOpenClose*> _ptrTimeOpenCloseVec;
    void initTime();
    MCP23017 *mcp23017 = nullptr;
    void doClockFF(std::string strPin);

    public:
    void doWork();

};

#endif