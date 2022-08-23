#ifndef MCP23017_H
#define MCP23017_H

#include<vector>

#define GPIO_COUNT     16

#define GPIO0    0x0001;    /* 0000 0000 | 0000 0001 */
#define GPIO1    0x0002;    /* 0000 0000 | 0000 0010 */
#define GPIO2    0x0004;    /* 0000 0000 | 0000 0100 */
#define GPIO3    0x0008;    /* 0000 0000 | 0000 1000 */
#define GPIO4    0x0010;    /* 0000 0000 | 0001 0000 */
#define GPIO5    0x0020;    /* 0000 0000 | 0010 0000 */
#define GPIO6    0x0040;    /* 0000 0000 | 0100 0000 */
#define GPIO7    0x0080     /* 0000 0000 | 1000 0000 */

#define GPIO8    0x0100;    /* 0000 0001 | 0000 0000 */
#define GPIO9    0x0200;    /* 0000 0010 | 0000 0000 */
#define GPIO10   0x0400;    /* 0000 0100 | 0000 0000 */
#define GPIO11   0x0800;    /* 0000 1000 | 0000 0000 */
#define GPIO12   0x1000;    /* 0001 0000 | 0000 0000 */
#define GPIO13   0x2000;    /* 0010 0000 | 0000 0000 */
#define GPIO14   0x4000;    /* 0100 0000 | 0000 0000 */
#define GPIO15   0x8000;    /* 1000 0000 | 0000 0000 */


class MCP23017
{
    public:
    MCP23017();
    ~MCP23017();

    private:
    int _devCount = 0; 
    std::vector<std::string> _devAdr;

    void readMCP23017_Configuration();
};



#endif