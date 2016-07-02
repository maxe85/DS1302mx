#include <RTCmx.h>
RTC rtc(5, 6, 7);
void setup() {

  Serial.begin(9600);
  
  /*
  This function does not check if write-protect is on, 
  if it is, run in SETUP_MODE.
  read readme for more information
  */
  
  
  // seconds
  rtc.t[0] = 40;
  
  // minutes
  rtc.t[1] = 16;
  
  // hours
  rtc.t[2] = 18;
  
  // date
  rtc.t[3] = 2;

  // month
  rtc.t[4] = 7;

  // weekday
  rtc.t[5] = 6;

  // year
  rtc.t[6] = 16;

  rtc.setTime();

}

void loop() {

  rtc.serial();
  Serial.println();
  delay(1000);
}
