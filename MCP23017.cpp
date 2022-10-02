#include <regex>
#include "MCP23017.h"


const std::string config_section_MCP23017  = "[MCP23017]";
const std::string strdevice                = "/dev/i2c-2";

MCP23017::MCP23017()
{
    std::printf("+++ MCP23017 Ctor\n");
    
    readMCP23017_Configuration();
    if(openMCP23017Device())
    {
       init_MCP23017_Devices();
    }
}

MCP23017::~MCP23017()
{}

bool MCP23017::openMCP23017Device()
{
    printf("+++Open I2C device\n");
    _open = false;
	_devI2C = open(strdevice.c_str(), O_RDWR);
	if (_devI2C < 0 ) 
    {
		if (errno == ENOENT) 
        {
			fprintf(stderr, "Error: Could not open file: %s  : %s\n", strdevice.c_str(), strerror(ENOENT));
		} 
        else 
        {
			fprintf(stderr, "Error: Could not open file "
				"`%s': %s\n", strdevice.c_str(), strerror(errno));
			if (errno == EACCES)
				fprintf(stderr, "Run as root?\n");
		}
	}
    else
    {
        _open = true;
    }
    printf("---Open I2C device\n");
    return _open;
}

void MCP23017::readMCP23017_Configuration()
{
    std::printf("+++ readMCP23017Config\n");
    ChickenConfiguration *chickConfig = new ChickenConfiguration(config_section_MCP23017);
    std::string key = "DevicesCount";
    int iValue, devCount = 0;
    if(chickConfig->getValue(key, iValue)) devCount = iValue;

    for(int i = 1; i <= devCount; i++)
    {   
        std::string newSection = config_section_MCP23017;             //[MCP23017]
        newSection.erase(newSection.size()-1);
        newSection = newSection + "_" + std::to_string(i) + "]";      //[MCP23017_i]
        chickConfig->setSection(newSection);
        std::string strVal;
        key = "DevAdress"; 
        if(!chickConfig->getValue(key, strVal)) continue;
        std::regex pattern("0x|0X");
        strVal = std::regex_replace(strVal, pattern, "");
        _devAdrVec.emplace_back(strVal);
        _gpio_Adr_Dir_Map.insert(std::pair<std::string, std::vector<std::string>>(strVal, std::vector<std::string>()));  //add empty vector to key (strVal)
        readMCP23017_Dir_Config(chickConfig, strVal, i);
    }
    if(chickConfig != nullptr) delete chickConfig;    
    printf("--- readMCP23017Config\n");
}

void MCP23017::readMCP23017_Dir_Config(ChickenConfiguration *chickConfig, std::string strAdr, int devNum)
{
    /* GPIO_0_1 */
    std::string key;
    for (int i = 0; i < GPIO_COUNT; i++)
    {
       key = "GPIO_" + std::to_string(i) + "_" + std::to_string(devNum);
       std::string strVal;
       if(chickConfig->getValue(key, strVal))
       {
           _gpio_Adr_Dir_Map[strAdr].emplace_back(strVal);
       }  
    }
}

void MCP23017::init_MCP23017_Devices()
{
    checkConnectedDevices();
//    set_MCP230127_Dir_and_PullUp_Pins();
}

/* try to connect the configured devices and store them into vector after this delte the key of not connected devs*/
void MCP23017::checkConnectedDevices() 
{
    for (const auto& elements : _gpio_Adr_Dir_Map)  //key --> elements.first, value --> elements.second
    {
        std::string devAdr = elements.first;
        int addr = std::stoi(devAdr);
        byte address = 0x68;
        bool force = false;
        if(_devI2C >= 0)
        {
           if (ioctl(_devI2C, force ? I2C_SLAVE_FORCE : I2C_SLAVE, address) < 0) 
	       {
		       printf("Failed to acquire bus access and/or talk to slave addr %d\n", address);
		       continue;
	       }
           else
          {
               printf("Device %x connected to I2C Bus\n", address);
              _connectedDevsVec.emplace_back(devAdr);
           }
        }
        else printf("Filhnd _devI2C < o\n");
    }
}

bool MCP23017::getOpen()
{
    return _open;
}
/*
void MCP23017::set_MCP230127_Dir_and_PullUp_Pins()
{
    for(std::string strAdr : _connectedDevsVec)
    {
        auto item = _gpio_Adr_Dir_Map.find(strAdr);       
        if(item != _gpio_Adr_Dir_Map.end())
        {
           byte gpioPortOffset = 0x00;
           if(_gpio_Adr_Dir_Map[strAdr].capacity() == GPIO_COUNT)
           {
               byte temp = 0x00;
               for(int port = 0; port < GPIO_PORTS; port++ )
               {
                  byte byteVal  = 0x01;
                  byte gpioPort = 0x00;
                  for(int gpio = gpioPortOffset; gpio < gpioPortOffset + 8; gpio++)
                  {
                       byteVal = 0x01;
                       byteVal <<= (gpio - gpioPortOffset);
                       if(_gpio_Adr_Dir_Map[strAdr].at(gpio) == "Input")    //Input --> gpioDir = 1
                       {
                           gpioPort = gpioPort | byteVal;
                       }
                       temp = gpio;                  
                  }
                  gpioPortOffset = temp + 1;
                  std::string IODIR = hexTable[IODIRA];
                  std::string GPPU  = hexTable[GPPUA];
                  if(port == 1) 
                  {
                      IODIR = hexTable[IODIRB];
                      GPPU  = hexTable[GPPUB]; 
                  }
                  std::string strCommand = "S " + strAdr + " " + IODIR  + " " + hexTable[gpioPort] + " P";    //Dir PortA, PortB
//                  interface->write_Serial(strCommand);
//                  interface->read_Serial(strCommand);
                  std::printf("%s\n", strCommand.c_str());
                  strCommand = "S " + strAdr + " " + GPPU + " " + hexTable[gpioPort] + " P";       // set pullups for INPUT
 //                 interface->write_Serial(strCommand);  
 //                 interface->read_Serial(strCommand);
                  std::printf("%s\n", strCommand.c_str());
               }
           }
           else std::printf("ERROR *** MCP23017:: Map capacity != GPIOCOUNT\n");
        }
    } 
}
*/



/*

#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port


	int file_i2c;
	int length;
	unsigned char buffer[60] = {0};

	
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
	
	int addr = 0x5a;          //<<<<<The I2C address of the slave
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}
	
	
	//----- READ BYTES -----
	length = 4;			//<<< Number of bytes to read
	if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}

	
	//----- WRITE BYTES -----
	buffer[0] = 0x01;
	buffer[1] = 0x02;
	length = 2;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		 ERROR HANDLING: i2c transaction failed 
		printf("Failed to write to the i2c bus.\n");
	}
*/

