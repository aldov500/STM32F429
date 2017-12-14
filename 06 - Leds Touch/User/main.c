/* Includes ------------------------------------------------------------------*/							
#include "main.h"

/*Estructura para GPIO*/
GPIO_InitTypeDef	GPIO_InitStructure;	//Inicialización de Terminales/Velocidad/Modo

uint32_t n;

void Delay(__IO uint32_t nCount);
void GPIO_Start();

static void TP_Config(void);


int main(void){
	static TP_STATE* TP_State; 
	int verde=0;
	int rojo=0;
  
	
	GPIO_Start();
	/* Inicializar el LCD */
	LCD_Init();
	
	/* Inicializar Capa de LCD */
 	 LCD_LayerInit();
	/* Habilitar el LCD */
	LTDC_Cmd(ENABLE);
	/* Set foreground layer */
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
  	/* Configuración del Touch Panel */
  	TP_Config();
	
  	while (1){
  	TP_State = IOE_TP_GetState();
	if ((TP_State->TouchDetected) && (TP_State->Y <= 150) && (TP_State->Y >= 10) && (TP_State->X >= 10) && (TP_State->X <=115))
	{
			verde=1;
			Delay(0xAFFFF);
			while(verde==1)
		  {
		
			  TP_State = IOE_TP_GetState();
		    if ((TP_State->TouchDetected) && (TP_State->Y <= 150) && (TP_State->Y >= 10) && (TP_State->X >= 10) && (TP_State->X <=115))
		    {
			  verde=1;
		
			  }
				else
				{
					verde=0;
					
				}
				Delay(0xAFFFF);
		  }
			GPIOG->ODR ^= GPIO_Pin_13;
		}
		else if ((TP_State->TouchDetected) && (TP_State->Y <= 150) && (TP_State->Y >= 10) && (TP_State->X >= 125) && (TP_State->X <=235))
		{
			rojo=1;
			Delay(0xAFFFF);
			while(rojo==1)
		  {
			  TP_State = IOE_TP_GetState();
		    if ((TP_State->TouchDetected) && (TP_State->Y <= 150) && (TP_State->Y >= 10) && (TP_State->X >= 125) && (TP_State->X <=235))
		    {
			    rojo=1;

		    }
				else
				{
					rojo=0;

				}
				Delay(0xAFFFF);
		  }
			GPIOG->ODR ^= GPIO_Pin_14;
		}
		else
		{
		}
	}
}

void GPIO_Start(){
	/* Habilitación de reloj del periférico GPIOG */
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	 
  	/* Configurar PG13 y PG14 como Push Pull*/
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

}

void TP_Config(void)
{
  /* Limpiar el LCD */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Configurar el IO Expander */
  if (IOE_Config() == IOE_OK)
  {   
    LCD_Clear(LCD_COLOR_GREY);
		LCD_SetBackColor(LCD_COLOR_GREY);
		LCD_SetTextColor(LCD_COLOR_GREEN);
		LCD_DrawFullRect(10,10,104,154);
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DrawFullRect(125,10,104,154);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DisplayStringLine(LCD_LINE_12, (uint8_t *) "   Touch LED");
		//LCD_SetTextColor(LCD_COLOR_WHITE);
		//LCD_DrawRect(10,165,145,219);
  }  
  else
  {
   /*Tamaño de Fuente*/
	  LCD_SetFont(&Font12x12);
		/*Limpia LCD*/
		LCD_Clear(LCD_COLOR_WHITE);
		/*Fondo Negro*/
		LCD_SetBackColor(LCD_COLOR_BLACK);
		/*Impresion Rectangulos*/
	  LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawFullRect(1, 1, 34, 280);
		LCD_SetTextColor(LCD_COLOR_YELLOW);
		LCD_DrawFullRect(34, 1, 34, 280);
		LCD_SetTextColor(LCD_COLOR_BLUE2);
		LCD_DrawFullRect(68, 1, 34, 280);
		LCD_SetTextColor(LCD_COLOR_GREEN);
		LCD_DrawFullRect(102, 1, 34, 280);
		LCD_SetTextColor(LCD_COLOR_MAGENTA);
		LCD_DrawFullRect(136, 1, 34, 280);
		LCD_SetTextColor(LCD_COLOR_RED);
		LCD_DrawFullRect(170, 1, 34, 280);
		LCD_SetTextColor(LCD_COLOR_BLUE);
		LCD_DrawFullRect(204, 1, 36, 280);
		LCD_SetTextColor(LCD_COLOR_BLACK);
		LCD_DrawFullRect(1, 280,240, 40);
		LCD_SetTextColor(LCD_COLOR_BLUE);
		LCD_DrawFullRect(1, 281	, 50, 40);
		LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DrawFullRect(50, 281	, 50, 40);
		LCD_SetTextColor(LCD_COLOR_BLUE);
		LCD_DrawFullRect(100, 281	, 50, 40);
		/*Mensaje*/
	  LCD_SetTextColor(LCD_COLOR_WHITE);
		LCD_DisplayStringLine(LINE(11), (uint8_t*)"         404!         ");
  }
    
  }

void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

/************************ (C) COPYRIGHT 2016 Luisof Technologies *****END OF FILE****/
