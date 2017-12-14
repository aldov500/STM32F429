/*	Funcion de Retardo semi-calculado.
	Cicla el procesador haciendo nada por un tiempo aproximado, para retener el proceso.
*/
void Delay(__IO uint32_t nCount){
	nCount *= 1800;
  	while(nCount != 0){
  		nCount --;
  		}
	}
/*	Funcion de retardo con el Systick

*/
extern volatile uint32_t msTicks;
volatile uint32_t msTicks;
void Delay(__IO uint32_t dlyTicks);

void SysTick_Handler(void){
	msTicks++;
	}

/*

*/
void delay_Init() {
    RCC_ClocksTypeDef RCC_Clocks;			//ESTRUCTURA PARA CLK
    RCC_GetClocksFreq(&RCC_Clocks);		//OBTIENE FREQ DE CLK
    /*Los ciclos while toman 4 ciclos*/
    /*Dividimos entre 4000000 para obtener ms*/
    varDelay = RCC_Clocks.HCLK_Frequency / 4000000;
}

void delay_Ms(uint32_t millis) {
	/* Multiplica para obtener millis por 1000*/
    /* Resta 10*/
    millis = 1000 * millis * varDelay - 10;
    /*4 ciclos por 1 loop*/
    while (millis--);
}



void GPIO_InitPort(GPIO_InitTypeDef Gpio_InitStructure, char letra){

	}
