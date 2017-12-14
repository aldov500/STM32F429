#include "main.h"

// Estructura para GPIO
GPIO_InitTypeDef	GPIO_InitStructure;	

extern volatile uint32_t msTicks;
void Delay(__IO uint32_t dlyTicks);

int main(void) {
	// Configuracion para Systick
  SystemCoreClockUpdate();
	if(SysTick_Config(SystemCoreClock/10000)){	
		while(1);
		}
	
	// Configuracion GPIOG
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);

  while (1) { 
		GPIOG->BSRRL = GPIO_Pin_13;
		GPIOG->BSRRH = GPIO_Pin_14;
		Delay(10000);
		
		GPIOG->BSRRH = GPIO_Pin_13;
		GPIOG->BSRRL = GPIO_Pin_14;
		Delay(10000);
		}
	}  

void Delay(__IO uint32_t dlyTicks){
	uint32_t curTicks = msTicks;
	while((msTicks - curTicks) < dlyTicks);
	}
