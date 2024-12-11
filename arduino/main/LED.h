#ifndef LED_H
#define LED_H

// #################### SD ####################
#include "./PIN_ASSIGN.h"

// 初期化
void LED_Init();
// 状態表示
void LED_States();
// エラー表示
void LED_Error(bool flag);

#endif