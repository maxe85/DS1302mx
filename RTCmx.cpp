#include "RTCmx.h"

class SPISession {
  public:
    SPISession(const int ce_pin, const int io_pin, const int sclk_pin)
      : ce_pin_(ce_pin), io_pin_(io_pin), sclk_pin_(sclk_pin) {
      digitalWrite(sclk_pin_, LOW );
      digitalWrite(ce_pin_,   HIGH);
      delayMicroseconds(4);  // tCC
    }
    ~SPISession() {
      digitalWrite(ce_pin_,   LOW);
      delayMicroseconds(4);  // tCWH
    }
  private:
    const int ce_pin_;
    const int io_pin_;
    const int sclk_pin_;
};

void RTC::serial() {                             // prints directly to Serial
#if MULTI_TEMPLATE
  printTime(1);
  for (uint8_t i = 0;; i++) {
    if (Buffer[i]) Serial.write(Buffer[i]);
    else break;
  }
#else
  printTime();
  Serial.write(Buffer, 17);//Serial.write(10);
#endif
}

#if MULTI_TEMPLATE
void RTC::printTime(uint8_t tp) {
  clw();
/* template HOWTO:
	use indexes of the RTC::t[] +128 for ones and +136 for tens,
	use characters in between, as well as { 50,48,142,134, ...} for four digit year
*/
  const uint8_t stringTemplate[][20] = {
    {142, 134, '-', 140, 132, '-', 139, 131, ' ', 138, 130, ':', 137, 129, ':', 136, 128, '\n', 0},
    {138, 130, ' ', 'U', 'h', 'r', ' ', 137, 129, ' ', 136, 128, 's', '\n', 0}
  };
  for (uint8_t i = 0;    ; i++) {
    uint8_t c = stringTemplate[tp][i];
#else
void RTC::printTime() {
  clw();
	
  const uint8_t stringTemplate[18] = { 142, 134, '-', 140, 132, '-', 139, 131, ' ', 138, 130, ':', 137, 129, ':', 136, 128, 0}; 
  for (uint8_t i = 0; i < 18 ; i++) {
    uint8_t c = stringTemplate[i];
#endif
    /*  handling the 'numbers' of template array
          (bit7) ? this is a number
          (bit3) ? this is a 10¹                 */
    if (c & 0x80) {                     // if (bit7)
      if (c & 0x08) {       // if (bit3)
        c = t[c & 0x07] / 10;
        c += 48;
      }
      else {                // if !(bit3)
        c = t[c & 0x07] % 10;
        c += 48;
      }
    }
    Buffer[i] = c;	// write directly to Serial here (or what you want) for bypassing the Buffer 
#if MULTI_TEMPLATE
    if (!c) break;
#endif
  }
}


RTC::RTC(const uint8_t CEpin, const uint8_t IOpin, const uint8_t CLpin) :
  _CEpin(CEpin),
  _IOpin(IOpin),
  _CLpin(CLpin)
{
  pinMode(_CEpin, OUTPUT);
  pinMode(_CLpin, OUTPUT);
  getTime();
#if SETUP_MODE
	uint8_t seconds;
  {
	const SPISession s (_CEpin, _IOpin, _CLpin);
	_send(0x81);
	seconds = 0x7f & _read();
  }
  {
	const SPISession s (_CEpin, _IOpin, _CLpin);
	_send(0x80);
	_send(seconds);
  }
  {
  const SPISession s (_CEpin, _IOpin, _CLpin);
	
  // disabling write-protect
	_send(0x8E);
	_send(0x00); 
  setTime();
}
#endif
}


void RTC::_send (const uint8_t writeByte)  {
  pinMode(_IOpin, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    digitalWrite(_IOpin, !!(writeByte & (1 << i)) );
    digitalWrite(_CLpin, HIGH);
    digitalWrite(_CLpin, LOW);
  }
}

uint8_t RTC::_read() {
  uint8_t value = 0;
  uint8_t readBit = 0;
  pinMode(_IOpin, INPUT);
  for (uint8_t i = 0; i < 8; ++i) {
    readBit = digitalRead(_IOpin);
    value |= (readBit << i);
    digitalWrite(_CLpin, HIGH);
    digitalWrite(_CLpin, LOW);
  }
  return value;
}

void RTC::_getTime() {  // gets the DS1302 time into the RTC object
  const SPISession s (_CEpin, _IOpin, _CLpin);
  _send(0xBF);    // send burstReadCommand
  for (uint8_t i = 0; i < 7; i++) {
    t[i] = _bcdToDec(_read());
  }
  _lastClw = millis();
#if SETUP_MODE
  // in case the halt flag was set (bit 7 of second register) it has passed bcdToDec(), so it needs to be revised
  if (t[0] >= 80) t[0] -= 80;
  // in case of 12h mode the hour value has passed bcdToDec() as well, so it needs to be revised
  if (t[2] >= 100) t[2] -= 88; // (AM mode (bit 7) and PM (bit 5)) *10
  if (t[2] >= 80 ) t[2] -= 80; // (AM mode (bit 7) *10
#endif
}


void RTC::getTime() { // updates the time frm DS1302 until the seconds flip
  _getTime();
  uint8_t localSecond = t[0];
  while (localSecond == t[0]) _getTime();
}

void RTC::setTime() {       // uploading the time from RTC object to DS1302 this will always unset the HALT flag (bit7 of seconds) an always set to 24h mode
  clw();
  const SPISession s (_CEpin, _IOpin, _CLpin);
  _send(0xBE);    // sends burstWriteCommand
  for (uint8_t i = 0; i < 7; i++) {
    _send(_decToBcd(t[i]));
  }
  _send(0);
  _lastClw = millis();
}


void RTC::getRam() {
  const SPISession s (_CEpin, _IOpin, _CLpin);
  _send(0xFF);
  for (uint8_t i = 0; i < 31; i++) Buffer[i] = _read();
}

void RTC::setRam() {
  const SPISession s (_CEpin, _IOpin, _CLpin);
  _send(0xFE);
  for (uint8_t i = 0; i < 31; i++) _send(Buffer[i]);
}

void RTC::clw() {
  uint32_t _ramClw = _lastClw;
  _lastClw  = millis();  // saved new last ClockWork
  _ramClw   = (_lastClw - _ramClw);
  _lastClw -= (_ramClw  % 1000 ); //adjusting milliseconds

  t[0] += (_ramClw / 1000) % 60;
  t[1] += (_ramClw / 60000) % 60;
  t[2] += (_ramClw / 3600000) % 24;

  if (t[0] > 59) {
    t[0] -= 60;
    t[1]++;
  }
  if (t[1] > 59) {
    t[1] -= 60;
    t[2]++;
  }
  if (t[2] > 23) {
    getTime();
  }
}

uint8_t RTC::_bcdToDec(const uint8_t bcd) {
  return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
}
uint8_t RTC::_decToBcd(const uint8_t dec) {
  return (dec / 10) << 4 | dec % 10;
}
