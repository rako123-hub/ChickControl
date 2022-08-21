#ifndef MCP23017_H
#define MCP23017_H

#include<vector>

#define GPIO_COUNT     16


class MCP23017
{
    public:
    MCP23017();
    ~MCP23017();

    private:
    int devCount = 0; 
    std::vector<std::string> devAdr;

    void readMCP23017_Configuration();
};



#endif