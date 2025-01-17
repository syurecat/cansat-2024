#include "./LED.h"

void setup(){
    LED_Init();
}

void loop(){
    LED_States_Change();
    LED_Error_Change();
}