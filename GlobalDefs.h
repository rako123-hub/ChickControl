#ifndef GLOBALDEFS_H
#define GLOBALDEFS_H

#include <string>

enum State {OPEN, CLOSE, UNDEFIENED};

const std::string RETURN_FAILURE_CONFIG = "999999999";

namespace std
{
  // define std::byte
  enum class byte : unsigned char {};

};

#endif