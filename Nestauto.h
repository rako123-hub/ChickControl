
#ifndef NESTAUTO_H
#define NESTAUTO_H

#include <cstdlib>
#include <regex>
#include <thread>

#include "TimeOpenClose.h"
#include "GlobalDefs.h"

struct NestautoData
{
    std::string time_open;
    std::string time_close;
    uint8_t     cylinders;
    uint8_t     state;
};


class Nestauto
{
    public:
    Nestauto();
    ~Nestauto();

    private :
    bool checkTimeString();
    void readNestautoConfiguration();
    TimeOpenClose *_timeopenclose;
    void doOpen();
    void doClose();
    NestautoData _nestData;

    public:
    void doWork();
    
};

#endif