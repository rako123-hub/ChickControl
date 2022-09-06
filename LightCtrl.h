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

    void readLightConfiguration();

    std::vector<LightData> _lightDataVec;
    std::string _light_IO;
    LightData _lightData;
    TimeOpenClose *_timeopenclose = nullptr;
    void initTime();

    public:
    void doWork();

};

#endif