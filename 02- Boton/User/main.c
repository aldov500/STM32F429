#include "main.h"

// Estructura para GPIO
GPIO_InitTypeDef	GPIO_InitStructure;	

int main(void) {
  // Puerto G
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	// Boton
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

  while (1) { 
		GPIOG->BSRRH = GPIO_Pin_13;
		GPIOG->BSRRH = GPIO_Pin_14;
		
		while(GPIOA->IDR & GPIO_Pin_0){
			GPIOG->BSRRL = GPIO_Pin_13;
			GPIOG->BSRRL = GPIO_Pin_14;
			}		
		}
	}  

