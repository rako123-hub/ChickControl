
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

struct CylinderDevice
{
    /*
    std::byte dir;            // direction bit [output]
    std::byte enable;         // enaböle bit   [output]
    std::byte open_detect;    // switch open   [input]
    std::byte close_detect;   // switch close  [input]
    */
    std::string dir;            // direction bit [output]
    std::string enable;         // enable bit   [output]
    std::string open_detect;    // switch open   [input]
    std::string close_detect;   // switch close  [input]
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
    std::vector<CylinderDevice> _cylinderDevVec;

    public:
    void doWork();
    
};

#endif