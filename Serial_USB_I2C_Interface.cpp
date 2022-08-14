
#include <cstdlib>
#include <thread>
#include <chrono>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function

#include <unistd.h> // write(), read(), close()

#include "ChickenConfiguration.h"
#include "Serial_USB_I2C_Interface.h"

const std::string config_section = "[Serial_USB_I2C_Interface]";


Serial_I2C_Interface::Serial_I2C_Interface()
{
    readSerialDevConfiguration();
    initSerialDevice();
    if (checkSerialDevInfo()) std::printf("%s is running \n", INTERFACEINITSTRING.c_str());
    else
    {
        std::printf(" FAILURE %s is not running \n", INTERFACEINITSTRING.c_str());
    }
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
    ChickenConfiguration chickConfig(config_section);
    std::string key = "Device";
    _serialConfig.devName = chickConfig.getValue(key);
    key = "Baud";
    _serialConfig.baudrate =  std::atoi(chickConfig.getValue(key).c_str());
}

void Serial_I2C_Interface::initSerialDevice()
{
    _serialUSB = open(_serialConfig.devName.c_str(), O_RDWR | O_NONBLOCK);
    if(_serialUSB < 0)
    {
        std::printf("Error %i from open: %s\n", errno, strerror(errno));
        std::printf("Can't open serial device : %s --> close application \n", _serialConfig.devName.c_str());
        return;
    } 

    struct termios tty;
    if(tcgetattr(_serialUSB, &tty) != 0) 
    {
      std::printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return ;
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
        return;
    }
    std::printf("Open serial device : %s successfull! \n", _serialConfig.devName.c_str());
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
    char buf[256];
    strcpy(buf, DEVINFO.c_str());
    write_Serial(buf);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    read_Serial(buf);
    printf("Read:: %s\n", buf);
    std::string strRead(buf);
    if (strRead.find(INTERFACEINITSTRING) == 0) return false;
    else return true;
}

void Serial_I2C_Interface::write_Serial(char *buf)
{

    int success = write(_serialUSB, buf, sizeof(buf));
    if(success < 0) std::printf("Can't write to serial interface \n");
}

void Serial_I2C_Interface::read_Serial(char *buf)
{
    char readBuf[256];
    int numBytes = 0;
    memset(&readBuf, '\0', sizeof(readBuf));
    numBytes = read(_serialUSB, &readBuf, sizeof(readBuf));
    if( numBytes < 0)  std::printf("Error reading: %s", strerror(errno));

    //std::printf("Bytes read %d \n", numBytes);
    strcpy(buf, readBuf);
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
