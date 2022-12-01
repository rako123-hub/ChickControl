#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

//#include <cstdint>
#include <string>

enum State {OPEN, CLOSE, UNDEFIENED};
enum LightState {OFF, ON, DIMM_ON, DIMM_OFF, UNDEF};

typedef uint8_t byte;

#define LOW  0x00
#define HIGH 0x01

#endif