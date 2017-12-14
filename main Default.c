#include "main.h"

uint32_t dvalue;

	
int main() {
	
	
	while (1) {
			
		}
	} 

/*	Funct: Obtiene un valor calculado para ejecutar ciclos y retardar el procesador    
 *  Param: Nada
 *  Progr: Aldo Vargas
 */
void Delay_Init() {
    RCC_ClocksTypeDef RCC_Clocks;			
    RCC_GetClocksFreq(&RCC_Clocks);		
    dvalue = RCC_Clocks.HCLK_Frequency / 4000000;
}
/*	Funct: Ejecuta multiples ciclos para retener el procesador como Delay    
 *  Param: Nada
 *  Progr: Aldo Vargas
 */
void Delay_Ms(uint32_t millis) {
    millis = 1000 * millis * dvalue - 10;
    while (millis--);
}
