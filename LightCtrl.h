#ifndef I2C_UTILS_H
#define I2C_UTILS_H

#include <string>
#include <vector>
#include "GlobalDefs.h"


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
    byte intervall_count;

    void readLightConfiguration();

    std::vector<LightData> lightDataVec;
    std::string light_IO;

};

#endif