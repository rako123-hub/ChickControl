
#include "DS3231.h"

const std::string strdevice                = "/dev/i2c-2";


DS3231::DS3231()
{
    if(openDS3231())
    {
        setDevAdr(0x68);
    }
}

DS3231::~DS3231()
{
    if(_devI2C >= 0)
    {
        close(_devI2C);
    }
}

bool DS3231::setDevAdr(byte addr)
{
    bool result = true;
    bool force = false;
    if (ioctl(_devI2C, force ? I2C_SLAVE_FORCE : I2C_SLAVE, addr) < 0) 
    {
        printf("Failed to acquire bus access and/or talk to slave addr %0x\n", addr);
        result = false;
    }
    else
    {
        printf("SetDevAddress:: %0x\n", addr);
    }
    return result;
}

bool DS3231::openDS3231()
{
    printf("+++Open I2C device\n");
    _open = false;
	_devI2C = open(strdevice.c_str(), O_RDWR);
	if (_devI2C < 0 ) 
    {
		if (errno == ENOENT) 
        {
			printf("Error: Could not open file: %s  : %s\n", strdevice.c_str(), strerror(ENOENT));
		} 
        else 
        {
			printf("Error: Could not open file "
				"`%s': %s\n", strdevice.c_str(), strerror(errno));
			if (errno == EACCES)
				printf("Run as root?\n");
		}
	}
    else
    {
        _open = true;
    }
    printf("---Open I2C device\n");
    return _open;
}

bool DS3231::writeData(byte reg, byte value)
{
    bool result = true;

    if(i2c_smbus_write_byte_data(_devI2C, reg, value) < 0) 
    {
        printf("Failed to write data to MCP23017 device\n");
        result = false;
    }
    return result;
}

byte DS3231::readData(byte reg)
{
    byte val = 0x55;
    val = i2c_smbus_read_byte_data(_devI2C, reg);
    return val;
}

void DS3231::getlocalTime_setDS3231()
{
    struct tm *tm_now = {};

    auto now = std::chrono::system_clock::now();
    std::time_t time_now = std::chrono::system_clock::to_time_t(now); 
    tm_now = localtime ( &time_now );
    //seconds_now = tm_now->tm_hour * 3600 + tm_now->tm_min * 60 + tm_now->tm_sec;

    printf ( "Current date and time are: %s \n", asctime (tm_now) );
/*
m_sec	int	seconds after the minute	0-61*
tm_min	int	minutes after the hour	0-59
tm_hour	int	hours since midnight	0-23
tm_mday	int	day of the month	1-31
tm_mon	int	months since January	0-11
tm_year	int	years since 1900	
tm_wday	int	days since Sunday	0-6
tm_yday	int	days since January 1	0-365
tm_isdst	int	Daylight Saving Time flag	
*/
    int year  = tm_now->tm_year + 1900 - 2000;
    int month = tm_now->tm_mon + 1 ;
    int day   = tm_now->tm_mday;
    int hour  = tm_now->tm_hour;
    int min   = tm_now->tm_min;
    int sec   = tm_now->tm_sec;

    printf("setYear0::%d\n", year);
    printf("setMonth0::%d\n", month);
    printf("setday0::%d\n", day);
    printf("sethour0::%d\n", hour);
    printf("setmin0::%d\n", min);
    printf("setsec0::%d\n", sec);
    printf("\n -------------------\n");
    setClock(REG_YEAR, year);
    setClock(REG_MONTH, month);
    setClock(REG_DAY, day);
    setClock(REG_HOUR, hour);
    setClock(REG_MIN, min);
    setClock(REG_SEC, sec);
}

void DS3231::setClock(byte reg, byte val)
{
    byte highVal = val / 10;
    byte lowVal  = val - (highVal * 10);
    byte calcVal = (highVal << 4) | lowVal;
    writeData(reg, calcVal);

}

byte DS3231::getSecond()
{
    byte secReg  = readData(REG_SEC);
    byte lowSec  = secReg & 0x0f;
    byte highSec = (secReg & 0xf0) >> 4;
    byte sec     = 10 * highSec + lowSec;  
    return sec;
}

byte DS3231::getMinute()
{
    byte minReg  = readData(REG_MIN);
    byte lowMin  = minReg & 0x0f;
    byte highMin = (minReg & 0xf0) >> 4;
    byte min     = 10 * highMin + lowMin;  
    return min;
}

byte DS3231::getHour()
{
    byte hourReg  = readData(REG_HOUR);
    byte lowHour  = hourReg & 0x0f;
    byte highHour = (hourReg & 0x30) >> 4;
    byte hour     = 10 * highHour + lowHour;  
    return hour;
}

byte DS3231::getDay()
{
    byte dayReg  = readData(REG_DAY);
    byte lowDay  = dayReg & 0x0f;
    byte highDay = (dayReg & 0x30) >> 4;
    byte day     = 10 * highDay + lowDay;  
    return day;
}

byte DS3231::getMonth()
{
    byte monthReg  = readData(REG_MONTH);
    byte lowMonth  = monthReg & 0x0f;
    byte highMonth = (monthReg & 0x10) >> 4;
    byte month     = 10 * highMonth + lowMonth;  
    return month;
}

byte DS3231::getYear()
{
    byte yearReg  = readData(REG_YEAR);
    byte lowYear  = yearReg & 0x0f;
    byte highYear = (yearReg & 0xf0) >> 4;
    byte year     = 10 * highYear + lowYear;  
    return year;
}



void DS3231::getDS3231_setLocalTime()
{
    byte sec   = getSecond();  
    byte min   = getMinute();
    byte hour  = getHour();
    byte day   = getDay();
    byte month = getMonth();
    byte year  = getYear();

    printf("sec::%d\n", sec);
    printf("min::%d\n", min);
    printf("hour::%d\n", hour);
    printf("day::%d\n", day);
    printf("month::%d\n", month);
    printf("year::%d\n", year);

    //buffer to format command
	unsigned char buff[37]={0};
	//formatting command with the given parameters
	sprintf((char*)buff,(const char *)"sudo date -s \"%02d/%02d/%04d %02d:%02d:%02d\"",month,day,year+2000,hour,min,sec);
	//execute formatted command using system()
	system((const char *)buff);
     
}

void DS3231::getRTC()
{
    byte sec   = getSecond();  
    byte min   = getMinute();
    byte hour  = getHour();
    byte day   = getDay();
    byte month = getMonth();
    byte year  = getYear();

    printf("RTC Date:: %02d/%02d/%04d %02d:%02d:%02d\n",month, day, year+2000, hour, min, sec);
}
