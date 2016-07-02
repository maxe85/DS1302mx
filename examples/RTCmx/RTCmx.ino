#include <RTCmx.h>

/*  Create the RTC object, this will download the time from the DS1302.
 *  This can last uptil one second, because it syncs the time in the object with the time on the chip.
 *  The Arduino function millis() does not count this time, exept RTC is created in setup() or loop().
 *  If this does not work, you may have set the halt flag, run in SETUP_MODE to fix this.
 *  Expects the CE IO and CL pins as arguments (aka RST DAT and CLK)
 */
RTC rtc(5,6,7);

void setup() {
  Serial.begin(9600);  
  Serial.println();
  Serial.println("The RTC object contains a byte array t[6] which is the local copy of the DS1302 time.");
  Serial.print("Seconds: rtc.t[0] = ");  Serial.println(rtc.t[0]);
  delay(1000);
  Serial.println("As you can see, it does not change itself ...");
  delay(1000);
  Serial.print("Seconds: rtc.t[0] = ");  Serial.println(rtc.t[0]);
  delay(1000);
  Serial.print("Seconds: rtc.t[0] = ");  Serial.println(rtc.t[0]);
  delay(1000);
  Serial.println("... unless it gets updated by the clockwork: rtc.clw()");
  delay(1000);
  rtc.clw();
  Serial.print("Seconds: rtc.t[0] = ");  Serial.println(rtc.t[0]);
  delay(1000);
  rtc.clw();
  Serial.print("Seconds: rtc.t[0] = ");  Serial.println(rtc.t[0]);
  delay(1000);
  rtc.clw();
  Serial.print("Seconds: rtc.t[0] = ");  Serial.println(rtc.t[0]);
  delay(1000);
  Serial.println("clw() does not care how often it is called,");
  delay(1000);
  Serial.println("it stores it last accesstime and computes.");
  delay(4000);
  rtc.clw();
  Serial.print("Seconds: rtc.t[0] = ");  Serial.println(rtc.t[0]);
}

void loop() {
}
