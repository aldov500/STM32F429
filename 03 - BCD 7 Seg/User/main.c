#include "main.h"
#include "stdbool.h"
#include "SevenSeg.h"

uint32_t dvalue;

int main() {	
	delay_Init();
	SevenSeg_Init();
	SevenSeg n;
	
	n.direccion = false;
	
	int i = 0;
  while (1) {
		
		if(GPIOA->IDR & GPIO_Pin_0)	n.direccion = !n.direccion;
		if(i>=15 && n.direccion) i = 0;
		if(i<=0 && !n.direccion) i = 15;
		
		if(n.direccion)	SevenSeg_SetInt(&n,++i);
		else			SevenSeg_SetInt(&n,--i);
		
		SevenSeg_Display(n);
		delay_Ms(200);
		
		}
	} 

void delay_Init() {
    RCC_ClocksTypeDef RCC_Clocks;			//ESTRUCTURA PARA CLK
    RCC_GetClocksFreq(&RCC_Clocks);		//OBTIENE FREQ DE CLK
    /*Los ciclos while toman 4 ciclos*/
    /*Dividimos entre 4000000 para obtener ms*/
    dvalue = RCC_Clocks.HCLK_Frequency / 4000000;
}

void delay_Ms(uint32_t millis) {
	/* Multiplica para obtener millis por 1000*/
    /* Resta 10*/
    millis = 1000 * millis * dvalue - 10;
    /*4 ciclos por 1 loop*/
    while (millis--);
}
	