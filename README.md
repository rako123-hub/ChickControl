# ChickControl
control software for chickens in the trolly

the application and its configuration file is located in:
/usr/local/bin/chickApp

services around chickenctrl:
ChickCtrlApp.service
TimeSetter.service

start TimeSetter:
TimeGetSetMainApp getLocal_setRTC   or
TimeGetSetMainApp getRTC_setLocal

set Time via Terminal:
date -s hh:mm

