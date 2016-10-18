#include <HMRlib.h>

#if defined(__AVR_ATmega328P__)
    #include "SoftwareSerial.h"
    SoftwareSerial Serial1(0, 1);
#endif

HMR3300 hmr;

long pitch, roll, head;
unsigned long data_age;

void setup() {
    Serial.begin(115200);
    while (!Serial){
        ; //wait for serial port
    }
    Serial1.begin(19200);
    delay(1000);
    Serial.println("Starting...");
}

void loop() {
    bool newData = false;
    for (unsigned long start = millis(); millis() - start <1000;) {
        while (Serial1.available()) {
            char c = Serial1.read();
            //Serial.print(c);
            if (hmr.encode(c)) {
                newData = true;
            }
        }
    }
    if (newData) {
        hmr.get_data(&pitch, &roll, &head, &data_age);
        Serial.print("Pitch: ");
        Serial.print(float(pitch)/100);
        Serial.write(248);
        Serial.print(", Roll: ");
        Serial.print(float(roll)/100);
        Serial.write(248);
        Serial.print(", Bearing: ");
        Serial.print(float(head)/100);
        Serial.write(248);
        Serial.print(", Age: ");
        Serial.print(float(data_age)/1000);
        Serial.println("s");
    }
}
