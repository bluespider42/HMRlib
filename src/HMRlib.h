/*
HMRlib - a small library for Arduino providing basic HMR serial parsing
For use with Honeywell Magnetic Sensors: HMR3300.

Serial Parsing based on work by Mikal Hart
https://github.com/mikalhart/TinyGPS

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef HMRlib_h
#define HMRlib_h

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
  #include "WConstants.h"
#endif

#include <stdlib.h>

#define _HMR_VERSION 001 //library version

// Your class header here...
class HMR3300 {
public:
    enum {
        HMR_INVALID_AGE = 0xFFFFFFFF,
        HMR_INVALID_DATA_TIME = 0xFFFFFFFF,
        HMR_INVALID_MAG = 9999,
        HMR_INVALID_TILT = 9999,
        HMR_INVALID_HEAD = 9999,
    };

    //static const int HMR_INVALID_TILT, HMR_INVALID_MAG, HMR_INVALID_HEAD;

    HMR3300();
    bool encode(char c); //one character from HMR3300
    HMR3300 &operator << (char c) {encode(c); return *this;}

    void get_data(long *pitch, long *roll, long *head, unsigned long *data_age);

    static int library_version() { return _HMR_VERSION; }

private:
    enum {_HMR_SENTENCE_OTHER, _HMR_SENTENCE_H, _HMR_SENTENCE_M};

    //properties
    long _pitch, _new_pitch;
    long _roll, _new_roll;
    long _head, _new_head;
    long _magX, _new_magX;
    long _magY, _new_magY;
    long _magZ, _new_magZ;

    unsigned long _last_data_time, _new_data_time;

    //parsing state variables
    bool _is_checksum_term;
    char _term[15];
    byte _sentence_type;
    byte _term_number;
    byte _term_offset;
    bool _end_of_sentence;

    //internal utilities
    int from_hex(char a); // not needed
    unsigned long parse_decimal();
    bool term_complete();
    bool hmrisdigit(char c) { return c >= '0' && c <= '9'; }
    long hmratol(const char *str);
    int hmrstrcmp(const char *str1, const char *str2);
};
#endif
