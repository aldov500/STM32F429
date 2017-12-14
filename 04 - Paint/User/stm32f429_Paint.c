#include "stm32f429_Paint.h"

void Paint_Init(){
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
	LCD_Clear(LCD_COLOR_WHITE);
	LCD_SetBackColor(LCD_COLOR_WHITE);
	//Color de Texto y tamaño de fuente
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font12x12);
	
	/* Limpiar el LCD */ 
  LCD_Clear(LCD_COLOR_WHITE);
  
  /* Configurar el IO Expander */
  if (IOE_Config() == IOE_OK){   
   	LCD_SetFont(&Font8x8);
   	LCD_DisplayStringLine(LINE(32), (uint8_t*)"              TouchPanel Paint      ");
   	LCD_DisplayStringLine(LINE(34), (uint8_t*)"              Aldo V.           ");
   	LCD_SetTextColor(LCD_COLOR_BLUE2); 
   	LCD_DrawFullRect(5, 250, 30, 30);
   	LCD_SetTextColor(LCD_COLOR_CYAN); 
   	LCD_DrawFullRect(40, 250, 30, 30);
   	LCD_SetTextColor(LCD_COLOR_YELLOW); 
   	LCD_DrawFullRect(75, 250, 30, 30);
   	LCD_SetTextColor(LCD_COLOR_RED); 
   	LCD_DrawFullRect(5, 288, 30, 30);
   	LCD_SetTextColor(LCD_COLOR_BLUE); 
   	LCD_DrawFullRect(40, 288, 30, 30);
   	LCD_SetTextColor(LCD_COLOR_GREEN); 
	  LCD_DrawFullRect(75, 288, 30, 30);
	  LCD_SetTextColor(LCD_COLOR_MAGENTA); 
	  LCD_DrawFullRect(145, 288, 30, 30);
	  LCD_SetTextColor(LCD_COLOR_BLACK); 
	  LCD_DrawFullRect(110, 288, 30, 30);
	  LCD_DrawRect(180, 270, 48, 50);
	  LCD_SetFont(&Font16x24);
	  LCD_DisplayChar(LCD_LINE_12, 195, 0x43);
	  LCD_DrawLine(0, 248, 240, LCD_DIR_HORIZONTAL);
	  LCD_DrawLine(0, 284, 180, LCD_DIR_HORIZONTAL);
	  LCD_DrawLine(1, 248, 71, LCD_DIR_VERTICAL);
	  LCD_DrawLine(37, 248, 71, LCD_DIR_VERTICAL);
	  LCD_DrawLine(72, 248, 71, LCD_DIR_VERTICAL);
	  LCD_DrawLine(107, 248, 71, LCD_DIR_VERTICAL);
	  LCD_DrawLine(142, 284, 36, LCD_DIR_VERTICAL);
	  LCD_DrawLine(0, 319, 240, LCD_DIR_HORIZONTAL);
  }  
  else{
	  LCD_Clear(LCD_COLOR_RED);
	  LCD_SetTextColor(LCD_COLOR_BLACK); 
	  LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)" IOE NO ESTA BIEN ");
	  LCD_DisplayStringLine(LCD_LINE_7,(uint8_t*)"REINICE LA TARJETA");
	  LCD_DisplayStringLine(LCD_LINE_8,(uint8_t*)"E INTENTE DE NUEVO");
  	}
	
	}
void Paint_Detect(){
	uint16_t linenum = 0;
	TP_STATE* TP_State; 
	TP_State = IOE_TP_GetState();
    
    if((TP_State->TouchDetected) && ((TP_State->Y < 245) && (TP_State->Y >= 3)))
    {
      if((TP_State->X >= 237) || (TP_State->X < 3))
      {}     
      else
      {
        LCD_DrawFullCircle(TP_State->X, TP_State->Y, 3);
      }
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 280) && (TP_State->Y >= 250) && (TP_State->X >= 5) && (TP_State->X <= 35))
    {
      LCD_SetTextColor(LCD_COLOR_BLUE2);
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 280) && (TP_State->Y >= 250) && (TP_State->X >= 40) && (TP_State->X <= 70))
    {
      LCD_SetTextColor(LCD_COLOR_CYAN); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 280) && (TP_State->Y >= 250) && (TP_State->X >= 75) && (TP_State->X <= 105))
    {
      LCD_SetTextColor(LCD_COLOR_YELLOW); 
    }      
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 5) && (TP_State->X <= 35))
    {
      LCD_SetTextColor(LCD_COLOR_RED);
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 40) && (TP_State->X <= 70))
    {
      LCD_SetTextColor(LCD_COLOR_BLUE); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 75) && (TP_State->X <= 105))
    {
      LCD_SetTextColor(LCD_COLOR_GREEN); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 110) && (TP_State->X <= 140))
    {
      LCD_SetTextColor(LCD_COLOR_BLACK); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 288) && (TP_State->X >= 145) && (TP_State->X <= 175))
    {
      LCD_SetTextColor(LCD_COLOR_MAGENTA); 
    }
    else if ((TP_State->TouchDetected) && (TP_State->Y <= 318) && (TP_State->Y >= 270) && (TP_State->X >= 180) && (TP_State->X <= 230))
    {
      LCD_SetFont(&Font8x8);
      for(linenum = 0; linenum < 31; linenum++)
      {
        LCD_ClearLine(LINE(linenum));
      }
    }
    else
    {
    }
	
	
	}