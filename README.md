[![Build Status](https://travis-ci.org/bluespider42/HMRlib.svg?branch=develop)](https://travis-ci.org/bluespider42/HMRlib)
[![Build Status](https://gitlab.com/obic/HMRlib/badges/develop/build.svg)](https://gitlab.com/obic/HMRlib/commits/develop)
HMRlib
======
An Arduino library for use with Honeywell HMR Compass Modules.
Currently only the HMR3300 is supported.

Currently work in progress.
Work will be slow as my project now uses the HMC6343 sensor instead.

Usage
-----
Open a serial port at a baudrate of 19200 to communicate with the sensor.
Feed `encode(char)` one byte at a time.
It will return true once an entire sentence has been parsed.

Run
 `get_data(long &pitch, long &roll, long &heading, unsigned long &age)`
to update the variables with the new data.

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
