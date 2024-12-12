#include "./LED.h"

unsigned long prevMillis = 0;
bool ledErrState = false;
int blinkCount = 0;
unsigned long interval = 500

void LED_Init() {
    pinMode(PIN_LED_STATES, OUTPUT);
    pinMode(PIN_LED_ERROR, OUTPUT);
}

void LED_Error(bool flag) {
    if (!flag) {
        digitalWrite(PIN_LED_ERROR, LOW);
        return;
    }
    unsigned long curMillis = millis();
    if (curMillis - prevMillis >= interval) {
        prevMillis = curMillis;
        ledErrState = !ledErrState;
        digitalWrite(PIN_LED_ERROR, ledErrState)
    }
}