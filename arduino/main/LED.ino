#include "./LED.h"

Led_t ledState;
Led_t ledError;
unsigned long interval = 1000;

void LED_Init() {
    pinMode(PIN_LED_STATES, OUTPUT);
    ledState.active = false;
    ledState.count = 0;
    ledState.currentTime = 0;
    ledState.lastTime = 0;
    pinMode(PIN_LED_ERROR, OUTPUT);
    ledError.active = false;
    ledError.count = 0;
    ledError.currentTime = 0;
    ledError.lastTime = 0;
    Serial.println(F("LED init done"));
}

void LED_States(uint8_t gpsQuality){
    switch (gpsQuality) {
    case 1: // GPS fix
        interval = 500;
        break;
    case 2: // Differential GPS fix
        interval = 300;
        break;
    case 4: // RTK Fixed
        interval = 200;
        break;
    case 5: // RTK Float
        interval = 400;
        break;
    default: // その他または無効
        blinkInterval = 10000;
    }
        ledState.currentTime = millis();
    if (ledState.currentTime - ledState.lastTime >= interval) {
        ledState.lastTime = ledState.currentTime;
        ledState.active = !ledState.active;
        digitalWrite(PIN_LED_ERROR, ledState.active)
    }
}

void LED_Error(bool flag) {
    if (!flag) {
        digitalWrite(PIN_LED_ERROR, LOW);
        return;
    }
    unsigned long errorInterval = 1000;
    ledError.currentTime = millis();
    if (ledError.currentTime - ledError.lastTime >= interval) {
        ledError.lastTime = ledError.currentTime;
        ledError.active = !ledError.active;
        digitalWrite(PIN_LED_ERROR, ledError.active)
    }
}