/*
  Copyright (c) 2016 Max Schöpflin. All right reserved.

  GNU GENERAL PUBLIC LICENSE  Version 2

	this is a library to interface the DS1302 Timekeeping Chip
	from maxim intergrated -> http://datasheets.maximintegrated.com/en/ds/DS1302.pdf
	read the readme
*/

#ifndef RTCmx_h
#define RTCmx_h
#include "Arduino.h"
// Defining the setupMode for the preprocessor. This can be necessary, if your DS1302 was not configured from before.
#define SETUP_MODE false
// Defining the TRICKLE-CHARGE REGISTER to 2 Diodes at 8kâ„¦. Delete or set to false if you dont want to set. Works only in setupMode.
#define TRICKLE_CHARGE_REGISTER B10101011
// Defining multipl templates for RTC::printTime()
// You can add more template arrays to the stringTemplate[][], redefine them in calling function as RTC::serial() 
#define MULTI_TEMPLATE false

class RTC {
public:
  uint8_t t[7] = {57,59,11,11,4,4,85}; ;  // 0 seconds, 1 minutes, 2 hour, 3 date, 4 month, 5 weekday, 6 year
  uint8_t Buffer[31];
  RTC(const uint8_t CEpin, const uint8_t IOpin, const uint8_t CLpin);
  void getTime();
  void setTime();
  void clw();
  void getRam();
  void setRam();
#if MULTI_TEMPLATE
  void printTime(uint8_t tp);
#else
  void printTime();
#endif
  void serial();
private:
  void    _getTime();
  void    _send (uint8_t writeByte);
  uint8_t _read();
  uint8_t _bcdToDec(const uint8_t bcd);
  uint8_t _decToBcd(const uint8_t bcd);
  
  uint8_t _CEpin;   //Pins
  uint8_t _IOpin;
  uint8_t _CLpin;
  uint32_t _lastClw;// stores time of last ClockWork
  
};

#endif
