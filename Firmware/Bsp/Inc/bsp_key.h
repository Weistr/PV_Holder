#ifndef _BSP_KEY_H
#define _BSP_KEY_H

#include "main.h"
#include "stdbool.h"

void bsp_keyScan_20ms(void);
extern bool key0Val;//按键按下置1，松开置0

extern bool key0OneClicFlag;//按键单击标志位  触发取反
extern bool key0TwoClicFlag;//按键双击标志位  触发取反
extern bool key0LonClicFlag;//按键长按标志位  触发置1
extern uint8_t clicTimeSpan_x20ms;//双击时间间隔 默认500ms
extern uint8_t long_push_x20ms;//长按1000ms

#endif
