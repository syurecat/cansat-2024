#include <SoftwareSerial.h>

SoftwareSerial GpsSerial(PIN_GPS_TX, PIN_GPS_RX);

void setup() {
    Serial.begin(115200);
    GpsSerial.begin(9600);
    GpsSerial.println("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28");
    Serial.println("Ready");
}
void loop() {
    // 作りかけだよ！
}

int getChecksum(String data) {
    byte checksum = 0;
    for (int i = 0; i < data.length(); i++) {
        checksum += data[i];
    }
    return checksum
}