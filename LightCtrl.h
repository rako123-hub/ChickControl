#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include <string>
#include <vector>
#include "GlobalDefs.h"
#include "TimeOpenClose.h"


struct LightData
{
    std::string time_on;
    std::string time_off;
};


class LightCtrl
{
    public:
    LightCtrl();
    ~LightCtrl();

    private:
    byte _intervall_count;
    byte _state    = LightState::UNDEF;
    byte _oldState = LightState::UNDEF;

    void readLightConfiguration();

    std::vector<LightData> _lightDataVec;
    std::string _light_IO;
    LightData _lightData;
    std::vector<TimeOpenClose*> _ptrTimeOpenCloseVec;
    void initTime();

    public:
    void doWork();

};

#endif