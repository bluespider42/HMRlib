#include <HMRlib.h>


#if defined(__AVR_ATmega32U4__) {
    #include "SoftwareSerial.h"
    SoftwareSerial Serial1(0, 1);
}

HMR3300 hmr;

long tiltx, tilty, head;
unsigned long data_age;

void setup() {
    Serial.begin(115200);
    Serial1.begin(19200);
}

void loop() {
    while (Serial1.available()) {
        int c = Serial1.read();
        if (hmr.encode(c)) {
            hmr.get_data(&tiltx, &tilty, &head, &data_age);
            Serial.print("X Angle: ");
            Serial.print(tiltx);
            Serial.print(", Y Angle: ");
            Serial.print(tilty);
            Serial.print(", Bearing: ");
            Serial.println(head);
        }
    }
}
