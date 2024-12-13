#include "./LED.h"

Led_t ledState;
Led_t ledError;

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

void LED_Error(bool flag) {
    if (!flag) {
        digitalWrite(PIN_LED_ERROR, LOW);
        return;
    }
    ledError.currentTime = millis();
    if (ledError.currentTime - ledError.lastTime >= interval) {
        ledError.lastTime = ledError.currentTime;
        ledError.active = !ledError.active;
        digitalWrite(PIN_LED_ERROR, ledError.active)
    }
}