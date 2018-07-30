[![Build Status](https://travis-ci.org/bluespider42/HMRlib.svg?branch=develop)](https://travis-ci.org/bluespider42/HMRlib)

HMRlib
======
An Arduino library for use with Honeywell HMR Compass Modules.
Currently only the HMR3300 is supported.

Currently work in progress.
Work will be slow as my project now uses the HMC6343 sensor instead.

## Compatibility
**Version 0.0.1**

<!-- Start Table -->
MCU                | Tested Works | Doesn't Work | Not Tested  | Notes
------------------ | :----------: | :----------: | :---------: | -----
ATSAMD21G18A       |      x       |              |             |
<!-- End Table -->

Usage
-----
Open a serial port at a baudrate of 19200 to communicate with the sensor.
Feed `encode(char)` one byte at a time.
It will return true once an entire sentence has been parsed.

Run
 `get_data(long &pitch, long &roll, long &heading, unsigned long &age)`
to update the variables with the new data.

The angles are given in degrees*100 and the age in milliseconds.

```
#include <HMRlib.h>
HMR3300 hmr
void setup() {
    long pitch, roll, heading;
    unsigned long age;
    Serial.begin(19200);
}
void loop() {
    while(Serial.available()) {
        c = Serial.read();
        if (hmr.encode(c))
            hmr.get_data(&pitch, &roll, &heading, &age);
    }
}
```
