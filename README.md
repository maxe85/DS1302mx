# RTCmx
slimline Arduino library for interfacing ds1302 RTC chip

My intension was to create a lightweight library,
that does not support things I do not desire.

So what this library does not support:
    *single register read
    *setting the halt flag
    *setting the write protect
    *writing 12h mode
    *printing weekdays in text
    
What does this library support:
    *a local objec, that works whith a copy of the time gathered from the device
    *synchronizing object time to device time, by halting the Arduino until
     the seconds on the device flip, this can last one second 
    *automaticly updating the time on first access after midnight with the synchronizing side-effect
    *reading 12h mode but converts it into 24h mode
    
How does it work?
    By initialisation the RTC object it downloads the time from DS1302 by the public methode getTime()
    which stores it in a public member array of 7 bytes called  b[second, minute, hour, date, month, weekday, year];
    it can easily be read and written by other functions. There is no protection against nonsense in these variables.
    The clockwork method clw() updates these by using a private variable to store the last accesstime,
    when it would flip the day it calles setTime again
    
SETUP_MODE
    If it is necessary, the SETUP_MODE can be activated by defining it as true in the header,
    this will also set the TRICKLE_CHARGE_REGISTER defined in same place.
    The TRICKLE_CHARGE_REGISTER implementation is not hardware tested yet.
    
