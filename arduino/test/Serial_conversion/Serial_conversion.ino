#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 9);

void setup() {
    Serial.begin(9600);
    mySerial.begin(9600);
    Serial.println("Ready");
}

void loop() {
    if (Serial.available()) {
        char dataFromPC = Serial.read();
        mySerial.write(dataFromPC);
    }

    if (mySerial.available()) {
        char dataFromDevice = mySerial.read();
        Serial.write(dataFromDevice);
    }
}
