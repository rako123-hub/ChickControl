
#include <cstdlib>
#include <thread>
#include <chrono>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function

#include <unistd.h> // write(), read(), close()

#include "GlobalDefs.h"
#include "ChickenConfiguration.h"
#include "Serial_USB_I2C_Interface.h"

const std::string config_section_Interface = "[Serial_USB_I2C_Interface]";

int counter = 0; 

Serial_I2C_Interface::Serial_I2C_Interface()
{
    printf("+++ ctor Serial_I2C_Interface\n");
    readSerialDevConfiguration();
    if(initSerialDevice())
    {
       if (checkSerialDevInfo()) std::printf("%s is running \n", INTERFACEINITSTRING.c_str());
       else
       {
           std::printf(" FAILURE %s is not running \n", INTERFACEINITSTRING.c_str());
       }
       setI2C_Clock();
       setInitOK(true);
    }
    printf("--- ctor Serial_I2C_Interface\n");
}

Serial_I2C_Interface::~Serial_I2C_Interface()
{
    if(_serialUSB >= 0)
    {
        printf("Close serial device: %s \n", _serialConfig.devName.c_str());
        close(_serialUSB);
    }
}

void Serial_I2C_Interface::readSerialDevConfiguration()
{
    ChickenConfiguration chickConfig(config_section_Interface);
    std::string key = "Device";
    std::string strValue;
    if(chickConfig.getValue(key, strValue)) _serialConfig.devName = strValue;
    key = "Baud";
    int iValue = 0;
    if(chickConfig.getValue(key, iValue))  _serialConfig.baudrate = iValue;
    key = "Busclock";
    if(chickConfig.getValue(key, iValue))
    {
        _serialConfig.busClock = iValue;
        std::printf("****busclock:: %d\n", _serialConfig.busClock);
    }
    
}

bool Serial_I2C_Interface::initSerialDevice()
{
    bool result = false;

    _serialUSB = open(_serialConfig.devName.c_str(), O_RDWR | O_NONBLOCK);
    if(_serialUSB < 0)
    {
        std::printf("Error %i from open: %s\n", errno, strerror(errno));
        std::printf("Can't open serial device : %s --> close application \n", _serialConfig.devName.c_str());
        return result;
    } 

    struct termios tty;
    if(tcgetattr(_serialUSB, &tty) != 0) 
    {
      std::printf("Error %i from tcgetattr: %s \n", errno, strerror(errno));
      return result ;
    }
    // set 8-n-0
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

   // tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
   // tty.c_cc[VMIN] = 0;

    setBaudrate(&tty);

    // Save tty settings, also checking for error();
    if (tcsetattr(_serialUSB, TCSANOW, &tty) != 0) 
    {
        std::printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return result;
    }
    std::printf("Open serial device : %s successfull! \n", _serialConfig.devName.c_str());
    return true;
}

void Serial_I2C_Interface::setBaudrate(termios *tty)
{
    switch(_serialConfig.baudrate)
    {
        case 9600:
        {
            cfsetispeed(tty, B9600);
            cfsetospeed(tty, B9600);
            break;
        }
        case 19200:
        {
            cfsetispeed(tty, B19200);
            cfsetospeed(tty, B19200);
            break;
        }
        case 115200:
        {
            cfsetispeed(tty, B115200);
            cfsetospeed(tty, B115200);
            break;
        }
        default:
        {
            std::printf("Couln'f find baud rate set default to 9600 Baud\n");
            cfsetispeed(tty, B9600);
            cfsetospeed(tty, B9600);
            break;
        }
    }
}


bool Serial_I2C_Interface::checkSerialDevInfo()
{
    printf("+++ Serial_I2C_Interface::checkSerialDevInfo() counter:%d\n", counter);
    bool result = false;
    memset(&_readBuf[0], '\0', sizeof(_readBuf));
    strcpy(_writeBuf, DEVINFO.c_str());
    write_Serial(_writeBuf);
    read_Serial(_readBuf);
    printf("%s\n", _readBuf);
    std::string strRead(_readBuf);
    if (!strRead.find(INTERFACEINITSTRING) == 0);
    {
        result = true;
    }
    printf("--- Serial_I2C_Interface::checkSerialDevInfo()\n");

    counter++;
    return result;
}

void Serial_I2C_Interface::setI2C_Clock()
{
    std::string strClock("T");
    strClock += std::to_string(_serialConfig.busClock);

    printf("Command:%s\n", strClock.c_str());
    strcpy(_writeBuf, strClock.c_str());
    write_Serial(_writeBuf);
}

void Serial_I2C_Interface::write_Serial(char *buf)
{
    printf("WriteSerBuffer: %s\n", buf);
    int success = write(_serialUSB, buf, sizeof(buf));
    if(success < 0) std::printf("Can't write to serial interface \n");
}

void Serial_I2C_Interface::read_Serial(char *buf)
{
    char readBuf[256];
    int numBytes = 0, offset = 0, tries = 0, bytes_to_read = 100;
    memset(&readBuf, '\0', sizeof(readBuf));

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    while(true)
    {
       numBytes = read(_serialUSB, &readBuf[0] + offset, bytes_to_read);
       if( numBytes < 0)
       {  
 //         std::printf("Error reading: %s\n", strerror(errno));
           std::this_thread::sleep_for(std::chrono::milliseconds(50));
           tries++;
           if (tries > 3) break;
       }
       else
       {
           offset += numBytes;
           if ( offset > (sizeof(readBuf) - bytes_to_read)) break;
           if (numBytes == 0) break;
       }
   }
   strcpy(buf, readBuf);
}

void Serial_I2C_Interface::setInitOK( bool init)
{
    _init = init;
}

bool Serial_I2C_Interface::getInitOK()
{
    return _init;
}
    
    










/*
    unsigned char msg[] = { '?', '\r' };
    write(_serialUSB, msg, sizeof(msg));

  // Allocate memory for read buffer, set size according to your needs
  char read_buf [256];

  // Normally you wouldn't do this memset() call, but since we will just receive
  // ASCII data for this example, we'll set everything to 0 so we can
  // call printf() easily.
  memset(&read_buf, '\0', sizeof(read_buf));

  // Read bytes. The behaviour of read() (e.g. does it block?,
  // how long does it block for?) depends on the configuration
  // settings above, specifically VMIN and VTIME
 // int num_bytes = read(_serialUSB, &read_buf, sizeof(read_buf));

  // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
 // if (num_bytes < 0) {
  //    printf("Error reading: %s", strerror(errno));
 //     return ;
 // }

  // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
  // print it to the screen like this!)
  //printf("Read %i bytes. Received message: %s", num_bytes, read_buf);

   memset(&read_buf, '\0', sizeof(read_buf));

  // Read bytes. The behaviour of read() (e.g. does it block?,
  // how long does it block for?) depends on the configuration
  // settings above, specifically VMIN and VTIME
  int num_Bytes = 1;
  while(num_Bytes > 0)
  {
     num_Bytes = read(_serialUSB, &read_buf, sizeof(read_buf));

     // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
     if (num_Bytes < 0) ´´´
     {
      printf("Error reading: %s", strerror(errno));
      return ;
     }
     else printf("Read %i bytes. Received mess;age: %s", num_Bytes, read_buf);

  }
*/
  // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
  // print it to the screen like this!)
