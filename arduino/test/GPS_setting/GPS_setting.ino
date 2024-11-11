#include <SoftwareSerial.h>

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    mySerial.println("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28");
    Serial.println("Ready");
}
void loop() {
    // 作りかけだよ！
}

int getChecksum(data) {
    checksum = 0
    raw_data.unpack("C*").each do |byte|
        checksum += byte
    end
    return raw_data.unpack("C*").push(checksum / 100)
}