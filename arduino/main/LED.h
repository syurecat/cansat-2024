#ifndef LED_H
#define LED_H

// #################### SD ####################
#include "./PIN_ASSIGN.h"

// 初期化
void LED_Init();
// 状態表示
void LED_States(uint8_t gpsQuality);
// エラー表示
void LED_Error(bool flag);

typedef struct {
    bool active;
    uint8_t count;
    unsigned long lastTime;
	unsigned long currentTime;
} Led_t;


#endif