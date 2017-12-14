#include "main.h"

	
int main() {
	
	LCD_DefInit();

  while (1){
   	startBday();
	}
	} 


void LCD_DefInit(void){
	LCD_Init();															//INICIALIZA LCD
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);			//ACTIVA PRIMERA CAPA
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);			//ACTIVA SEGUNDA CAPA
	LTDC_ReloadConfig(LTDC_IMReload);			//CARGA CONF PREDETERMINADA
	LTDC_Cmd(ENABLE);
	//Inicializa
	LCD_LayerInit();
	LCD_DisplayOn();
	//Color de Fondo
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_Clear(LCD_COLOR_BLACK);
	LCD_SetBackColor(LCD_COLOR_BLACK);
	//Color de Texto y tamaño de fuente
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_SetFont(&Font12x12);
}
//Colocar un Pixel en coordenadas

void startBday(){
	int menu = 0;
	
	switch(menu){
		case 0:
			LCD_Clear(LCD_COLOR_YELLOW);
			LCD_SetTextColor(LCD_COLOR_WHITE);
			LCD_DrawFullRect(100,100,100,100);
		case 1:
			
		case 2:
			
		case 3:
			
		default:
	}
	
}