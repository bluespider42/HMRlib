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
    int _pitch, _new_pitch;
    int _roll, _new_roll;
    int _mag, _new_mag;
    int _head, _new_head;

    unsigned long _last_data_time, _new_data_time;

    //parsing state variables
    byte _parity;
    bool _is_checksum_term;
    char _term[15];
    byte _sentence_type;
    byte _term_number;
    byte _term_offset;
    bool _hmr_data_good;

    //internal utilities
    int from_hex(char a);
    unsigned long parse_decimal();
    bool term_complete();
    bool hmrisdigit(char c) { return c>= '0' && c <= '9'; }
    long hmratol(const char *str);
    int hmrstrcmp(const char *str1, const char *str2);
};
#endif
