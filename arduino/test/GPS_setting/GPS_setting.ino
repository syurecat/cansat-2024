#include <SoftwareSerial.h>

SoftwareSerial GpsSerial(PIN_GPS_TX, PIN_GPS_RX);

bool irq = false;
String line;

void push();
void menu();
void updaterate(String rate);
int getChecksum(String data);

void setup() {
    Serial.begin(115200);
    GpsSerial.begin(9600);
    pinMode(2,INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2),push,RISING);
    delay(2000);
    GpsSerial.println("$PMTK314,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28");
    GpsSerial.println("$PMTK220,200*2C");
    Serial.println("Ready");
}
void loop() {
    if (GpsSerial.available()) {
        char dataFromDevice = GpsSerial.read();
        Serial.write(dataFromDevice);
    }
    if (irq) {
        menu();
    }
}

void push() {
    irq = true;
}

void menu(){
    Serial.println("");
    Serial.println("==========================================================");
    Serial.println("||                     ★ Setting ★                     ||");
    Serial.println("==========================================================");
    Serial.println("");
    Serial.println("1: pmtk set NMEA updaterate.");
    Serial.println("2: pmtk api set NMEA output.");
    Serial.println("3: Reserved");
    Serial.println("4: close.");
    Serial.println("");
    Serial.println("==========================================================");
    Serial.println("Please enter your choices: ");
    char choice = Serial.readStringUntil('\n')[0];
    switch (choice) {
        case '1':
            Serial.println("1: pmtk set NMEA updaterate.");
            Serial.println("==========================================================");
            Serial.println("||          ★ Setting NMEA port update rate ★          ||");
            Serial.println("==========================================================");
            Serial.println("The possible interval values range between 100 and 10000 millisecond.");
            Serial.println("Please enter:");
            line = Serial.readStringUntil('\n');
            updaterate(line);
            break;
        case '2':
            Serial.println("2: pmtk api set NMEA output.");
            Serial.println("==========================================================");
            Serial.println("||     ★ Setting NMEA sentence output frequencies★     ||");
            Serial.println("==========================================================");
            Serial.println("There are totally 19 data fields that present output frequencies for the 19 supported NMEA sentences individually.");
            Serial.println("NMEA_SEN_GLL,NMEA_SEN_RMC,NMEA_SEN_VTG,NMEA_SEN_GGA,NMEA_SEN_GSA,NMEA_SEN_GSV,Reserved,Reserved,Reserved,Reserved,Reserved,Reserved,NMEA_SEN_ZDA,NMEA_SEN_MCHN");
            Serial.println("Note: Enter the setting values accurately. They will not be validated!");
            Serial.println("Please enter:");
            line = Serial.readStringUntil('\n');
            sentenceOutput(line);
            break;
        case '3':
            Serial.println("Reserved");
            break;
        case '4':
            Serial.println("4: close.");
            irq = false;
            Serial.println("==========================================================");
            return;
            break;
        default:
            Serial.println("error");
            break;
    }
}

void updaterate(String rate) {
    if (rate.toInt() > 100 && rate.toInt() < 10000) {
        String query = "$PMTK220," + rate;
        GpsSerial.println(query + "*" + String(getChecksum(query), HEX));
        Serial.println("Set to {rate}");
    }
}

void sentenceOutput(String rate) {
    String query = "$PMTK314," + rate;
    GpsSerial.println(query + "*" + String(getChecksum(query), HEX));
    Serial.println("Set to {rate}");
}

int getChecksum(String data) {
    byte checksum = 0;
    for (int i = 0; i < data.length(); i++) {
        checksum ^= data[i];
    }
    return checksum
}