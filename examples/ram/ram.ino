#include "RTCmx.h"
RTC rtc(5,6,7);

void setup() {
  Serial.begin(9600);

  char myString[] = "Hello my huge RTC ram!!";  

  Serial.println("Here is my myString: ");
  Serial.println(myString);
  Serial.println();
  Serial.println("Now loading it to rtc.Buffer ...");
  
  for (byte i=0; i<31 ;i++){  
    if (myString[i]) rtc.Buffer[i]=myString[i];
    else {
      rtc.Buffer[i]=0;    // terminates the string in ram
      Serial.print(i);  // prints the written string length
      break;
    }
  }
  Serial.println(" bytes written to rtc.Buffer");
  Serial.println();
  Serial.println("Uploading rtc.buffer to ds1302 ...");
  rtc.setRam();           // uploading string
  Serial.println();

  Serial.println("Now using the buffer for something else ...");
  rtc.serial();
  rtc.Buffer[18]=0; //terminates the string in rtc.Buffer when not in MULTI_TEMPLATE mode
  Serial.println();
  Serial.println("Buffer now was used by RTC::serial() as you can see:");       
  for (byte i=0; i<31 ;i++){
    if (rtc.Buffer[i]) Serial.write(rtc.Buffer[i]);
    else {
      Serial.println();
      break;
    }
  }
  Serial.println();
  
  Serial.println("Downloading ds1302 ram to rtc.buffer");
  rtc.getRam();           // downloading string
  Serial.println();
  
  Serial.println("Here is the rtc.Buffer again: ");
  for (byte i=0; 31 ;i++){
    if (rtc.Buffer[i]) Serial.write(rtc.Buffer[i]);
    else {
      Serial.println();
      break;
    }
  }

}

void loop() {
 
}
