#ifndef __KEYPAD_H
#define __KEYPAD_H
#include "stm32f4xx_hal.h"

uint8_t Key_Scan(void);
uint8_t Key2BCD(uint8_t key);

#endif
