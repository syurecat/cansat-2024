#include <SoftwareSerial.h>


void setup() {
    Serial.begin(38400);
    Serial1.begin(38400);
    Serial.println("Ready");
}

void loop() {
    if (Serial.available()) {
        char dataFromPC = Serial.read();
        Serial1.write(dataFromPC);
    }

    if (Serial1.available()) {
        char dataFromDevice = Serial1.read();
        Serial.write(dataFromDevice);
    }
}
