#ifndef __BCD_7_SEG_H
#define __BCD_7_SEG_H

#include "stdbool.h"
#include "stm32F4xx.h"

typedef struct SevenSeg{
	bool a,b,c,d,e,f,g,h;
	bool direccion;
} SevenSeg;


void SevenSeg_Init();
void SevenSeg_Set(SevenSeg *newDisplay, bool a, bool b, bool c, bool d, bool e, bool f, bool g );
void SevenSeg_SetInt(SevenSeg *newDisplay, int num);
void SevenSeg_Display(SevenSeg newDisplay);
void SevenSeg_Clean();
void SevenSeg_SwitchDirection(SevenSeg *newDisplay);

#endif
