#include "HMRlib.h"

#define _H_TERM "H"
#define _M_TERM "M"

HMR3300::HMR3300()
: _pitch(HMR_INVALID_TILT)
, _roll(HMR_INVALID_TILT)
, _head(HMR_INVALID_HEAD)
, _last_data_time(HMR_INVALID_DATA_TIME)
, _sentence_type(_HMR_SENTENCE_OTHER)
, _term_number(0)
, _term_offset(0)
, _end_of_sentence(false)


//HMR3300::init() {
//}
{
    _term[0] = '\0';
}

//public methods
bool HMR3300::encode(char c)
{
    bool valid_sentence = false;

    switch(c)
    {
        case ',': // term terminators
        case '\r':
        case '\n':
            if (_term_offset < sizeof(_term))
            {
                _term[_term_offset] = 0;
                valid_sentence = term_complete();
            }
            _end_of_sentence = true;
            ++_term_number;
            _term_offset = 0;
            return valid_sentence;
            _end_of_sentence = false; //added as no start of sentence char
        case '#': //sentence begin
        case '*':
            _term_number = _term_offset = 0;
            _sentence_type = _HMR_SENTENCE_OTHER;
            _hmr_data_good = false;
            return valid_sentence;
    }
    //ordinary characters
    if (_term_offset < sizeof(_term) - 1)
        _term[_term_offset++] = c;
    return valid_sentence;
}

//internal utilities
int HMR3300::from_hex(char a)
{
    if (a >= 'A' && a <= 'F')
        return a - 'A' + 10;
    else if (a >= 'a' && a <= 'f')
        return a - 'a' + 10;
    else
        return a - '0';
}

unsigned long HMR3300::parse_decimal()
{
    char *p = _term;
    bool isneg = *p == '-';
    if (isneg) ++p;
    unsigned long ret = 100UL * hmratol(p);
    while (hmrisdigit(*p)) ++p;
    if (*p == '.')
    {
        if (hmrisdigit(p[1]))
        {
            ret += 10 * (p[1] - '0');
            if (hmrisdigit(p[2]))
                ret += p[2] - '0';
        }
    }
    return isneg ? -ret : ret;
}

long HMR3300::hmratol(const char *str)
{
    long ret = 0;
    while (hmrisdigit(*str))
        ret = 10 * ret + *str++ - '0';
    return ret;
}

void HMR3300::get_data(long *pitch, long *roll,  long *head, unsigned long *data_age)
{
    if (pitch) *pitch = _pitch;
    if (roll) *roll = _roll;
    if (head) *head = _head;
    if (data_age) *data_age = _last_data_time == HMR_INVALID_DATA_TIME ?
        HMR_INVALID_AGE : millis() - _last_data_time;
}

#define COMBINE(sentence_type, term_number) (((unsigned)(sentence_type) << 5) | term_number)

bool HMR3300::term_complete()
{
            if (_hmr_data_good) {
                _last_data_time = _new_data_time;

                switch(_sentence_type) {
                    case _HMR_SENTENCE_OTHER:
                        _pitch = _new_pitch;
                        _roll = _new_roll;
                        _head = _new_head;
                        break;
                }
                return true;
            }
    if (_end_of_sentence) {
        }
        return false;
    }
    if (_term_number == 0) {
        if (!hmrstrcmp(_term, _H_TERM))
            _sentence_type = _HMR_SENTENCE_H;
        else if (!hmrstrcmp(_term, _M_TERM))
            _sentence_type = _HMR_SENTENCE_M;
        else
            _sentence_type = _HMR_SENTENCE_OTHER;
        return false;
    }
    if (_sentence_type != _HMR_SENTENCE_OTHER && _term[0])
        switch(COMBINE(_sentence_type, _term_number)) {
            case COMBINE(_HMR_SENTENCE_H, 1):
                _new_head = parse_decimal();
                _new_data_time = millis();
                break;
            case COMBINE(_HMR_SENTENCE_H, 2):
                _new_pitch = parse_decimal();
                break;
            case COMBINE(_HMR_SENTENCE_H, 3):
                _new_roll = parse_decimal();
                break;
            //add cases for other sentence types
        }
        return false;
}

int HMR3300::hmrstrcmp(const char *str1, const char *str2) {
    while (*str1 && *str1 == *str2)
        ++str1, ++str2;
    return *str1;
}
