#include <RTCmx.h>

RTC rtc(5, 6, 7);

void setup() {
  Serial.begin(9600); // the rtc.serial() depends on this

  /* The RTC object contains another byte array of 31 length called Buffer
      which is used to create readable time strings (and read and write the ram)
  */

  Serial.println("Displaying Buffer content");
  for (byte i = 0; i < 31; i++) {
    if (rtc.Buffer[i]) {
      Serial.write(rtc.Buffer[i]);
    }
    else {
      Serial.write(10);
      break;
    }
  }
  delay(1000);

  Serial.println("writing time string: rtc.printTime()");
  delay(300);
  rtc.printTime();
  delay(300);
  Serial.println("Displaying Buffer again");
  for (byte i = 0; i < 31; i++) {
    if (rtc.Buffer[i]) {
      Serial.write(rtc.Buffer[i]);
    }
    else {
      Serial.write(10);
      break;
    }
  }
  delay(1000);
  Serial.println("Of course it runs the clockwork automaticly ...");
  for (byte i = 0; i < 7; i++) {
    delay(1000);
    rtc.printTime();
    for (byte i = 0; i < 31; i++) {
      if (rtc.Buffer[i]) {
        Serial.write(rtc.Buffer[i]);
      }
      else {
        Serial.write(10);
        break;
      }
    }
  }
  delay(2000);
  Serial.println("\n");
  Serial.println("And finally, RTC can access the Serial itself.");
  Serial.println("The rtc.serial() calles printTime() and writes the to Serial.");

}

void loop() {
  rtc.serial();
  
  Serial.println();
  
  delay(random(1000,10000));


}
