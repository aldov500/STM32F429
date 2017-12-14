#ifndef __MAIN_H
#define __MAIN_H

#include "stm32F4xx.h"	
#include "stm32f429i_discovery.h"
#include "SevenSeg.h"

void gpiod_Init();
void delay_Init();
void delay_Ms(uint32_t millis);
void button_Init();
void board_LedInit();
void bcd_7Seg_printValue(bool bcd);

#endif
