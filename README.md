# sunset clock

This clock contains 24 LEDs for every hour of the day. The LEDs between sunrise and sunset are on and the current hour flashes.

<img src="/model.gif" alt="Model"/>

## components 
- Arduino nano
- clock module DS3231
- 24 LEDs
- 3 shift register SN74HC595

The calculation of daytime is based on the date and location of the clock.
The clockmodule tracks the current time, if the powersource is disconnected.
The shiftregisters are daisychained to toggle the individual LEDs. The Arduino nano does not have enough pins. 
