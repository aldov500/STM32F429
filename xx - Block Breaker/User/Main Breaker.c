/*
Project: 				Juego Block Breaker
Programmer: 		Aldo Alexandro Vargas Meza
Code Languaje: 	C99
*/

#define BACKCOLOR LCD_COLOR_BLACK
#define numeroBloques 8
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"
#include "stm32f429i_discovery.h"
#include "stm32f4xx_adc.h"
/*********************Cabeceras para manejo de Software*********/
#include "main.h"					
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/*********************Prototipos de Funciones******************/
void GAME_LCD_Init();
void GAME_LCD_Board_Print();
void GPIO_Led_Button();
void TM_Delay_Init();
void TM_DelayMillis(uint32_t millis);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
/***********************Variables Globales*********************/
GPIO_InitTypeDef	GPIO_InitStructure;
uint32_t delay_multiplier;
/*********************Estructura Player*********************/
typedef struct Player{
	int coordX, coordY, alto, ancho;
	int vidas;
	int nivel;
	bool gameover;
}Player1;
void Player_DrawPlayer(Player1 p){
	LCD_SetTextColor(LCD_COLOR_GREY);
	LCD_DrawFullRect(p.coordX, p.coordY,p.ancho, p.alto);
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawRect(p.coordX, p.coordY, p.alto-1,p.ancho-1);
}
void Player_ErasePlayer(Player1 p){
	LCD_SetTextColor(BACKCOLOR);
	LCD_DrawFullRect(p.coordX, p.coordY,p.ancho,p.alto);
}
/*********************Estructura Ball*********************/
typedef struct Ball{
	int coordX, coordY, radio;
}Ball1;
void Ball_DrawBall(Ball1 b){
	LCD_SetTextColor(LCD_COLOR_WHITE);
	LCD_DrawFullCircle(b.coordX, b.coordY, b.radio);	
	LCD_SetTextColor(LCD_COLOR_GREY);
	LCD_DrawCircle(b.coordX, b.coordY, b.radio);
}
void Ball_EraseBall(Ball1 b){
	LCD_SetTextColor(BACKCOLOR);
	LCD_DrawFullCircle(b.coordX, b.coordY, b.radio);
}
void Ball_MoveBall(Ball1* ball, Ball1* eraseb,int* ball_DirectionX,int* ball_DirectionY){
		/*Arriba a la izquierda*/
		if(*ball_DirectionY == 1 && *ball_DirectionX == 1)
		{
			/*Impresion Movimiento Bola*/
			eraseb->coordX--;
			eraseb->coordY++;
			ball->coordX--;
			ball->coordY++;
			/*Deteccion Esquinas del tablero*/
			if(ball->coordX - eraseb->radio <= 3)   *ball_DirectionX = 2;		//Cambios de Direccion
			if(ball->coordY + eraseb->radio >= 317) *ball_DirectionY = 1;
		}
		/*Abajo a la izquierda*/
		else if(*ball_DirectionY == 2 && *ball_DirectionX == 1){
			/*Impresion Movimiento Bola*/
			eraseb->coordX--;
			eraseb->coordY--;
			ball->coordX--;
			ball->coordY--;
			/*Deteccion Esquinas del tablero*/
			if(ball->coordX - eraseb->radio <= 3)	*ball_DirectionX = 2;			
			if(ball->coordY - eraseb->radio <= 36) *ball_DirectionY = 1;
		}
		/*Abajo a la derecha*/
		else if(*ball_DirectionY == 2 && *ball_DirectionX == 2){
			/*Impresion Movimiento Bola*/
			eraseb->coordX++;
			eraseb->coordY--;
			ball->coordX++;
			ball->coordY--;
			/*Deteccion Esquinas del tablero*/
			if(ball->coordX + eraseb->radio >= 237)*ball_DirectionX = 1;
			if(ball->coordY - eraseb->radio <= 36)	*ball_DirectionY = 1;	
		}
		/*Abajo a la izquierda*/
		else if(*ball_DirectionY == 1 && *ball_DirectionX == 2){
			/*Impresion Movimiento Bola*/
			eraseb->coordX++;
			eraseb->coordY++;
			ball->coordX++;
			ball->coordY++;
			/*Deteccion Esquinas del tablero*/
			if(ball->coordX + eraseb->radio >= 237)	*ball_DirectionX = 1;
			if(ball->coordY + eraseb->radio >= 317)	*ball_DirectionY = 2;
		}
	
}
/*********************Estructura blocks*********************/
typedef struct blocks{
	int coordX, coordY;
	int ancho, alto;	
	bool eliminado;
}bloque;
/*********************Construccion de bloques***************************/
void Block_DrawBlock(bloque b){
	LCD_SetTextColor(LCD_COLOR_RED);
	LCD_DrawFullRect(b.coordX, b.coordY, b.ancho, b.alto);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_DrawRect(b.coordX, b.coordY, b.alto, b.ancho);
}
void Block1_DrawBlock(bloque b){
	LCD_SetTextColor(LCD_COLOR_YELLOW);
	LCD_DrawFullRect(b.coordX, b.coordY, b.ancho, b.alto);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_DrawRect(b.coordX, b.coordY, b.alto, b.ancho);
}
void Block2_DrawBlock(bloque b){
	LCD_SetTextColor(LCD_COLOR_GREEN);
	LCD_DrawFullRect(b.coordX, b.coordY, b.ancho, b.alto);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_DrawRect(b.coordX, b.coordY, b.alto, b.ancho);
}
/*********************************Eliminador******************************/
void Block_EraseBlock(bloque b){
	LCD_SetTextColor(BACKCOLOR);
	LCD_DrawFullRect(b.coordX, b.coordY, b.ancho, b.alto);
	LCD_DrawRect(b.coordX, b.coordY, b.alto, b.ancho);
}
/*********************Funciones sistema de Juego*********************/
void GAME_StartButton(Player1* play, int* ball_DirectionX, int* ball_DirectionY){
		GPIO_SetBits(GPIOG,GPIO_Pin_13);
		if(play->coordX%2 != 0 ){
			*ball_DirectionX = 1;
			*ball_DirectionY = 2;
		}
		else{
			*ball_DirectionX = 2;
			*ball_DirectionY = 2;
		}
}
void GAME_StartConditions(Ball1* ball, Ball1* eraseb, Player1* play, Player1* erasep){
	ball->coordY 	= play->coordY - eraseb->radio - 2;
	ball->coordX 	= play->coordX+(play->ancho/2);
	eraseb->coordX 	= ball->coordX;
	eraseb->coordY 	= ball->coordY;
	erasep->coordY 	= erasep->coordY-play->alto-8;
	Player_ErasePlayer(*erasep);
	erasep->coordY 	= erasep->coordY+play->alto;
	Player_ErasePlayer(*erasep);
}
void GAME_CollisionBallPlayer(Ball1* ball, Ball1* eraseb, Player1* play,int* ball_DirectionX, int* ball_DirectionY ){
	/*********************Condiciones de Colision Objeto Player - Bola *********************/
	/*Contacto por arriba*/
	if((ball->coordY+eraseb->radio>=play->coordY) && (ball->coordY-eraseb->radio<play->coordY))
	{
		if((ball->coordX>=play->coordX) && ball->coordX<play->coordX+play->ancho){
		*ball_DirectionY = 2;
		}
	}
	/*Contacto por la izquierda*/
	else if((ball->coordX+eraseb->radio>=play->coordX)&&(ball->coordX-eraseb->radio<play->coordX))
	{
		if((ball->coordY<=play->coordY+play->alto) && (ball->coordY>=play->coordY)){
			*ball_DirectionX = 2;
		}
	}
	/*Contacto por la derecha*/
	else if((ball->coordX-eraseb->radio<=play->coordX+play->ancho)&&(ball->coordX+eraseb->radio>play->coordX+play->ancho))
	{
		if((ball->coordY<=play->coordY+play->alto) && (ball->coordY>=play->coordY)){
			*ball_DirectionX = 1;
		}
	}
	/*Contacto por abajo*/
	else if((ball->coordY-eraseb->radio<=play->coordY+play->alto) && (ball->coordY+eraseb->radio>play->coordY))
	{
		if((ball->coordX>=play->coordX) && ball->coordX<play->coordX+play->ancho){
			*ball_DirectionY = 1;
		}
	}
}
void GAME_CollisionBallBlock(Ball1* ball, Ball1* eraseb, bloque* block, bloque* block1, bloque* block2, int* ball_DirectionX, int* ball_DirectionY, int* score1){
	for(int i=0; i<numeroBloques; i++)
		{
			if(block[i].eliminado == false)
			{	
				/*Contacto por la izquierda*/
				if((ball->coordY+eraseb->radio>=block[i].coordY)&&(ball->coordY-eraseb->radio<block[i].coordY))
				{
					if((ball->coordX>=block[i].coordX)&&(ball->coordX<=block[i].coordX+block[i].ancho)){
						*ball_DirectionY = 2;
						block[i].eliminado = true;
						Block_EraseBlock(block[i]);
						*score1+=100;
					}
				}
				else if((ball->coordX+eraseb->radio>=block[i].coordX)&&(ball->coordX-eraseb->radio<block[i].coordX))
				{
					if((ball->coordY>block[i].coordY)&&(ball->coordY<block[i].coordY+block[i].alto)){
						*ball_DirectionX = 1;
						block[i].eliminado = true;
						Block_EraseBlock(block[i]);
						*score1+=100;
					}
				}
				else if((ball->coordX-eraseb->radio<=block[i].coordX+block[i].ancho)&&(ball->coordX+eraseb->radio>block[i].coordX+block[i].ancho))
				{
					if((ball->coordY>block[i].coordY)&&(ball->coordY<block[i].coordY+block[i].alto)){
						*ball_DirectionX = 2;
						block[i].eliminado = true;
						Block_EraseBlock(block[i]);
						*score1+=100;
					}
				}
				else if((ball->coordY-eraseb->radio<=block[i].coordY+block[i].alto)&&(ball->coordY+eraseb->radio>block[i].coordY+block[i].alto))
				{
					if((ball->coordX>=block[i].coordX)&&(ball->coordX<=block[i].coordX+block[i].ancho)){
						*ball_DirectionY = 1;
						block[i].eliminado = true;
						Block_EraseBlock(block[i]);
						*score1+=100;
					}
				}	
			}
			
			if(block1[i].eliminado == false)
			{	
				if((ball->coordY+eraseb->radio>=block1[i].coordY)&&(ball->coordY-eraseb->radio<block1[i].coordY))
				{
					if((ball->coordX>=block1[i].coordX)&&(ball->coordX<=block1[i].coordX+block1[i].ancho)){
						*ball_DirectionY = 2;
						block1[i].eliminado = true;
						Block_EraseBlock(block1[i]);
						*score1+=100;
					}
				}
				else if((ball->coordX+eraseb->radio>=block1[i].coordX)&&(ball->coordX-eraseb->radio<block1[i].coordX))
				{
					if((ball->coordY>block1[i].coordY)&&(ball->coordY<block1[i].coordY+block1[i].alto)){
						*ball_DirectionX = 1;
						block1[i].eliminado = true;
						Block_EraseBlock(block1[i]);
						*score1+=100;
					}
				}
				else if((ball->coordX-eraseb->radio<=block1[i].coordX+block1[i].ancho)&&(ball->coordX+eraseb->radio>block1[i].coordX+block1[i].ancho))
				{
					if((ball->coordY>block1[i].coordY)&&(ball->coordY<block1[i].coordY+block1[i].alto)){
						*ball_DirectionX = 2;
						block1[i].eliminado = true;
						Block_EraseBlock(block1[i]);
						*score1+=100;
					}
				}
				else if((ball->coordY-eraseb->radio<=block1[i].coordY+block1[i].alto)&&(ball->coordY+eraseb->radio>block1[i].coordY+block1[i].alto))
				{
					if((ball->coordX>=block1[i].coordX)&&(ball->coordX<=block1[i].coordX+block1[i].ancho)){
						*ball_DirectionY = 1;
						block1[i].eliminado = true;
						Block_EraseBlock(block1[i]);
						*score1+=100;
					}
				}	
			}
			if(block2[i].eliminado == false)
			{	
				if((ball->coordY+eraseb->radio>=block2[i].coordY)&&(ball->coordY-eraseb->radio<block2[i].coordY))
				{
					if((ball->coordX>=block2[i].coordX)&&(ball->coordX<=block2[i].coordX+block2[i].ancho)){
						*ball_DirectionY = 2;
						block2[i].eliminado = true;
						Block_EraseBlock(block2[i]);
						*score1+=100;
					}
				}
				else if((ball->coordX+eraseb->radio>=block2[i].coordX)&&(ball->coordX-eraseb->radio<block2[i].coordX))
				{
					if((ball->coordY>block2[i].coordY)&&(ball->coordY<block2[i].coordY+block2[i].alto)){
						*ball_DirectionX = 1;
						block2[i].eliminado = true;
						Block_EraseBlock(block2[i]);
						*score1+=100;
					}
				}
				else if((ball->coordX-eraseb->radio<=block2[i].coordX+block2[i].ancho)&&(ball->coordX+eraseb->radio>block2[i].coordX+block2[i].ancho))
				{
					if((ball->coordY>block2[i].coordY)&&(ball->coordY<block2[i].coordY+block2[i].alto)){
						*ball_DirectionX = 2;
						block2[i].eliminado = true;
						Block_EraseBlock(block2[i]);
						*score1+=100;
					}
				}
				else if((ball->coordY-eraseb->radio<=block2[i].coordY+block2[i].alto)&&(ball->coordY+eraseb->radio>block2[i].coordY+block2[i].alto))
				{
					if((ball->coordX>=block2[i].coordX)&&(ball->coordX<=block2[i].coordX+block2[i].ancho)){
						*ball_DirectionY = 1;
						block2[i].eliminado = true;
						Block_EraseBlock(block2[i]);
						*score1+=100;
					}
				}		
			}
		}

}	
void GAME_DefineBallPlayer(Player1* play, Player1* erasep, Ball1* ball, Ball1* eraseb){
	/*********************Definicion de Objeto Player******************/
	play->coordX 	= 100;
	play->coordY 	= 280;
	play->ancho 	= 38;
	play->alto 		= 7;
	play->vidas 	= 3;
	play->nivel  	= 1;
	play->gameover = false;

	erasep->coordX = play->coordX;
	erasep->coordY = play->coordY;
	erasep->ancho 	= play->ancho;
	erasep->alto 	= play->alto;
/*********************Definicion de Objeto Bola******************/
	ball->coordX = play->coordX - 40/2;
	ball->coordY = play->coordY;
	ball->radio 	= 3;
	
	eraseb->coordX = ball->coordX;
	eraseb->coordY = ball->coordY;
	eraseb->radio = 5;
}
void GAME_DefineBlocks(bloque* block, bloque* block1, bloque* block2, int nivelx){
/*********************Definicion de Objetos Bloque****************/
	switch(nivelx){
		case 1:
			for(int i=0; i<numeroBloques; i++)
			{
				block[i].coordX 	= 7 + (i*30);
				block[i].coordY 	= 50;
				block[i].alto 		= 15;
				block[i].ancho 		= 15;
				block[i].eliminado 	= false;
				Block_DrawBlock(block[i]);

				block1[i].coordX 	= 7 + (i*29);
				block1[i].coordY 	= 70+ i*7;
				block1[i].alto 		= 15;
				block1[i].ancho 	= 20;
				block1[i].eliminado = false;
				Block1_DrawBlock(block1[i]);
						
				block2[i].coordX 	= 7 + (i*30);
				block2[i].coordY 	= 140;
				block2[i].alto 		= 8;
				block2[i].ancho 	= 15;
				block2[i].eliminado = false;
				Block2_DrawBlock(block2[i]);
			}
				
		break;

		case 2:
			for(int i=0; i<numeroBloques; i++)
			{
				block[i].coordX 	= 15 + (i*27);
				block[i].coordY 	= 40;
				block[i].alto 		= 7;
				block[i].ancho 		= 15;
				block[i].eliminado 	= false;
				Block_DrawBlock(block[i]);

				block1[i].coordX 	= 15 + (i*27);
				block1[i].coordY 	= 60;
				block1[i].alto 		= 7;
				block1[i].ancho 	= 15;
				block1[i].eliminado = false;
				Block1_DrawBlock(block1[i]);
					
				block2[i].coordX 	= 15 + (i*27);
				block2[i].coordY 	= 80;
				block2[i].alto 		= 35;
				block2[i].ancho 	= 15;
				block2[i].eliminado = false;
				Block2_DrawBlock(block2[i]);
			}
		break;
			
			case 3:
			for(int i=0; i<numeroBloques; i++)
			{
				block[i].coordX 	= 10 + (i*27);
				block[i].coordY 	= 40;
				block[i].alto 		= 10;
				block[i].ancho 		= 27;
				block[i].eliminado 	= false;
				Block_DrawBlock(block[i]);

				block1[i].coordX 	= 20 + (i*27);
				block1[i].coordY 	= 80;
				block1[i].alto 		= 8;
				block1[i].ancho 	= 8;
				block1[i].eliminado = false;
				Block1_DrawBlock(block1[i]);
						
				block2[i].coordX 	= 20 + (i*27);
				block2[i].coordY 	= 120;
				block2[i].alto 		= 100;
				block2[i].ancho 	= 5;
				block2[i].eliminado = false;
				Block2_DrawBlock(block2[i]);
			}
			break;
				
	}
				
}
void GAME_LCD_UpdateData(Player1 p, Ball1 ball, char score[20]){
	LCD_SetFont(&Font8x12);
	LCD_SetTextColor(BACKCOLOR);
	LCD_DrawFullRect(160,5,70,25);	
	for(int i=0; i<p.vidas; i++){
		LCD_SetTextColor(LCD_COLOR_GOLDEN);
		LCD_DrawFullRect(170 +(i*20),15,10,5);
		LCD_SetTextColor(LCD_COLOR_GREEN2);
		LCD_DrawRect(170+(i*20),15,5,10);	
	}
}
void GAME_Score(char* score,int* score1,char* nivel,int* nivelx){
	LCD_SetTextColor(LCD_COLOR_WHITE);
	sprintf(nivel," P1:%04d   LVL %d",*score1,*nivelx);
	LCD_DisplayStringLine(LINE(1), (uint8_t*)nivel);
}
void Game_Over(Player1* play, int* nivelx){
	play->gameover = true;
	*nivelx=0;
}
/*********************Funcion Principal****************************/
int main(){	
	/*Variables de Asistencia*/
	bool start;
	bool blocksCreated 		= false;
	int lecturaPot 			= 0;
	int ball_DirectionX 	= 0;
	int ball_DirectionY 	= 0;
	char score[20];
	char nivel[20];
	int score1=0;
	int nivelx=1;
/*********************Configuracion de Sistema*********************/
	GAME_LCD_Init();
	/*Imprime el tablero*/
	GAME_LCD_Board_Print();
	/*Inicializa los Leds de la tarjeta*/
	GPIO_Led_Button();
	/*Inicializa sistema ADC*/
  SystemInit();
	/*Inicia el ADC1, en el Canal 0 = PA0 */
 	TM_ADC_Init(ADC1, ADC_Channel_0);
	/*Inicializa Delay*/
	TM_Delay_Init();
	/*Creacion de Objetos Player*/
	Player1 play;
	Player1 erasep;
	/*Creacion de Objetos Ball*/
	Ball1 ball;
	Ball1 eraseb;
	/*Creacion de Objetos Bloque*/
	bloque block [numeroBloques];
	bloque block1[numeroBloques];
	bloque block2[numeroBloques];
	/*Definicion de Objetos*/
	GAME_DefineBallPlayer(&play,&erasep,&ball,&eraseb);
	
/*********************Loop Principal******************************/
	while (1)	{
		switch(nivelx){
			case 0:
			/*Color del Texto*/
			LCD_SetTextColor(LCD_COLOR_CYAN);
			LCD_DisplayStringLine(LINE(11), (uint8_t*)"           GAME OVER          ");
			LCD_SetBackColor(LCD_COLOR_BLACK);
			
			/*Impresion del Marco*/
			LCD_DrawRect(  1,35,287, 237);			
			while(nivelx==0){
				GPIO_SetBits(GPIOG,GPIO_Pin_14);
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==1){
					GPIO_ResetBits(GPIOG,GPIO_Pin_13);
					GPIO_ResetBits(GPIOG,GPIO_Pin_14);
					LCD_SetTextColor(BACKCOLOR);
					LCD_DrawFullRect(5,37,230,200);
					TM_DelayMillis(500);
					score1 = 0;
					play.vidas=3;
					blocksCreated=false;
					play.gameover=false;
					nivelx=1;
				}
			}
				
			break;
			case 1:

			/*Condicion para bandera de creacion de bloques*/
			if(blocksCreated==false){
				GAME_DefineBlocks(block,block1,block2,nivelx);
				blocksCreated = true;
			}
			/*Actualiza valores del juego*/
			GAME_LCD_UpdateData(play,ball, score);
			/*Lectura Analogica del Potenciometro*/
			lecturaPot = TM_ADC_Read(ADC1, ADC_Channel_0);
			/*Division para ajuste a la resolucion de la pantalla*/
			lecturaPot = lecturaPot/20;
			/*Deteccion de limites del marco para objeto Player*/
			if(lecturaPot<3)lecturaPot   = 3;
			if(lecturaPot>196)lecturaPot = 197;
			/*********************Direcciones Objeto Bola****************/
			Ball_MoveBall(&ball,&eraseb,&ball_DirectionX,&ball_DirectionY);
			/*********************Condiciones de Colision Objeto Player - Bola ************/
			GAME_CollisionBallPlayer(&ball,&eraseb,&play,&ball_DirectionX,&ball_DirectionY );
			/*********************Condiciones de Colision Objeto Bola - Bloque *********************/	
			GAME_CollisionBallBlock(&ball,&eraseb,block,block1,block2,&ball_DirectionX,&ball_DirectionY,&score1);
			/*********************Condiciones de Inicio con Boton****************************/	
			if(ball_DirectionX == 0 && ball_DirectionY == 0){
				if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==1 && play.gameover==false){
					GAME_StartButton(&play,&ball_DirectionX,&ball_DirectionY);
				}
				else if(play.gameover==true && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==1){
					play.gameover = false;
					nivelx=0;
				}
				GAME_StartConditions(&ball,&eraseb,&play,&erasep);
			}
			/*********************Actualiza los valores**************************************/	
			erasep.coordY = play.coordY;
			erasep.coordX = play.coordX;
			play.coordX   = lecturaPot;
			/*Imprime player en limpio*/
			Player_ErasePlayer(erasep);
			Player_DrawPlayer(play);
			/*Imprime Bola*/
			Ball_EraseBall(eraseb);
			Ball_DrawBall(ball);
			/*********************Condiciones de Colision Objeto Bola - Suelo *****************/	
			if(ball.coordY + eraseb.radio>=317){
				ball_DirectionX = 0;
				ball_DirectionY = 0;
				LCD_SetTextColor(BACKCOLOR);
				LCD_DrawFullRect(4,309,233,8);
				if(play.vidas>0){
					play.vidas--;
				}
				else{
					Game_Over(&play,&nivelx);
				}
			}
			/*********************Condiciones de Colision Objeto Bola - Suelo *****************/
			GAME_Score(score,&score1,nivel,&nivelx);
			if(score1==2400){
				ball_DirectionX=0;
				ball_DirectionY=0;
				nivelx++;
				blocksCreated = false;
				GAME_StartConditions(&ball,&eraseb,&play,&erasep);
				TM_DelayMillis(1000);
				LCD_DrawFullRect(5,45,230,200);
			}
			/*Retardo sistema*/
			TM_DelayMillis(6);
					
			break;
				
			case 2:
			/*Condicion para bandera de creacion de bloques*/
			if(blocksCreated==false){
				GAME_DefineBlocks(block,block1,block2,nivelx);
				score1=3400;
				blocksCreated = true;
			}
			/*Actualiza valores del juego*/
			GAME_LCD_UpdateData(play,ball, score);
			/*Lectura Analogica del Potenciometro*/
			lecturaPot = TM_ADC_Read(ADC1, ADC_Channel_0);
			/*Division para ajuste a la resolucion de la pantalla*/
			lecturaPot = lecturaPot/20;
			/*Deteccion de limites del marco para objeto Player*/
			if(lecturaPot<3)lecturaPot   = 3;
			if(lecturaPot>196)lecturaPot = 197;
			/*********************Direcciones Objeto Bola****************/
			Ball_MoveBall(&ball,&eraseb,&ball_DirectionX,&ball_DirectionY);
			/*********************Condiciones de Colision Objeto Player - Bola ************/
			GAME_CollisionBallPlayer(&ball,&eraseb,&play,&ball_DirectionX,&ball_DirectionY );
			/*********************Condiciones de Colision Objeto Bola - Bloque *********************/	
			GAME_CollisionBallBlock(&ball,&eraseb,block,block1,block2,&ball_DirectionX,&ball_DirectionY,&score1);
			/*********************Condiciones de Inicio con Boton****************************/	
			if(ball_DirectionX == 0 && ball_DirectionY == 0){
				if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==1 && play.gameover==false){
					GAME_StartButton(&play,&ball_DirectionX,&ball_DirectionY);
				}
				else if(play.gameover==true && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==1){
					play.gameover = false;
					nivelx=0;
				}
				GAME_StartConditions(&ball,&eraseb,&play,&erasep);
			}
			/*********************Actualiza los valores**************************************/	
			erasep.coordY = play.coordY;
			erasep.coordX = play.coordX;
			play.coordX 	= lecturaPot;
			/*Imprime player en limpio*/
			Player_ErasePlayer(erasep);
			Player_DrawPlayer(play);
			/*Imprime Bola*/
			Ball_EraseBall(eraseb);
			Ball_DrawBall(ball);
			/*********************Condiciones de Colision Objeto Bola - Suelo *****************/	
			if(ball.coordY + eraseb.radio>=317){
				ball_DirectionX = 0;
				ball_DirectionY = 0;
				LCD_SetTextColor(BACKCOLOR);
				LCD_DrawFullRect(4,309,233,8);
				if(play.vidas>0){
					play.vidas--;
				}
				else{
					Game_Over(&play,&nivelx);
				}
			}
			/*********************Condiciones de Colision Objeto Bola - Suelo *****************/
			GAME_Score(score,&score1,nivel,&nivelx);
			if(score1==5800){
				ball_DirectionX=0;
				ball_DirectionY=0;
				nivelx++;
				blocksCreated = false;
				GAME_StartConditions(&ball,&eraseb,&play,&erasep);
				TM_DelayMillis(1000);
				LCD_DrawFullRect(5,41,230,200);
			}
			/*Retardo sistema*/
			TM_DelayMillis(5);
					
			break;
				
			case 3:
			/*Condicion para bandera de creacion de bloques*/
			if(blocksCreated==false){
				GAME_DefineBlocks(block,block1,block2,nivelx);
				blocksCreated = true;
				score1=6800;
			}
			/*Actualiza valores del juego*/
			GAME_LCD_UpdateData(play,ball, score);
			/*Lectura Analogica del Potenciometro*/
			lecturaPot = TM_ADC_Read(ADC1, ADC_Channel_0);
			/*Division para ajuste a la resolucion de la pantalla*/
			lecturaPot = lecturaPot/20;
			/*Deteccion de limites del marco para objeto Player*/
			if(lecturaPot<3)lecturaPot   = 3;
			if(lecturaPot>196)lecturaPot = 197;
			/*********************Direcciones Objeto Bola****************/
			Ball_MoveBall(&ball,&eraseb,&ball_DirectionX,&ball_DirectionY);
			/*********************Condiciones de Colision Objeto Player - Bola ************/
			GAME_CollisionBallPlayer(&ball,&eraseb,&play,&ball_DirectionX,&ball_DirectionY );
			/*********************Condiciones de Colision Objeto Bola - Bloque *********************/	
			GAME_CollisionBallBlock(&ball,&eraseb,block,block1,block2,&ball_DirectionX,&ball_DirectionY,&score1);
			/*********************Condiciones de Inicio con Boton****************************/	
			if(ball_DirectionX == 0 && ball_DirectionY == 0){
				if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==1 && play.gameover==false){
					GAME_StartButton(&play,&ball_DirectionX,&ball_DirectionY);
				}
				else if(play.gameover==true && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==1){
					play.gameover = false;
					nivelx=0;
				}
				GAME_StartConditions(&ball,&eraseb,&play,&erasep);
			}
			/*********************Actualiza los valores**************************************/	
			erasep.coordY = play.coordY;
			erasep.coordX = play.coordX;
			play.coordX 	= lecturaPot;
			/*Imprime player en limpio*/
			Player_ErasePlayer(erasep);
			Player_DrawPlayer(play);
			/*Imprime Bola*/
			Ball_EraseBall(eraseb);
			Ball_DrawBall(ball);
			/*********************Condiciones de Colision Objeto Bola - Suelo *****************/	
			if(ball.coordY + eraseb.radio>=317){
				ball_DirectionX = 0;
				ball_DirectionY = 0;
				LCD_SetTextColor(BACKCOLOR);
				LCD_DrawFullRect(4,309,233,8);
				if(play.vidas>0){
						play.vidas--;
				}
				else{
					Game_Over(&play,&nivelx);
				}
			}
			/*********************Condiciones de Colision Objeto Bola - Suelo *****************/
			GAME_Score(score,&score1,nivel,&nivelx);
			if(score1==9200){
				ball_DirectionX=0;
				ball_DirectionY=0;
				nivelx++;
				blocksCreated = false;
				GAME_StartConditions(&ball,&eraseb,&play,&erasep);
				TM_DelayMillis(1000);
				LCD_DrawFullRect(5,41,210,200);
			}
			/*Retardo sistema*/
			TM_DelayMillis(4);
				
			break;
				
			case 4:
				
			/*Color del Texto*/
			LCD_SetTextColor(LCD_COLOR_WHITE);
			LCD_DisplayStringLine(LINE(11), (uint8_t*)"           WINNER!          ");
			
			sprintf(nivel,"          Score:%04d          ",score1);
			LCD_DisplayStringLine(LINE(12), (uint8_t*)nivel);
			
			LCD_SetTextColor(LCD_COLOR_CYAN);
			/*Impresion del Marco*/
			LCD_DrawRect(  1,35,287, 237);			
			
			while(nivelx==4){
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)==1){
					LCD_SetTextColor(BACKCOLOR);
					LCD_DrawFullRect(6,41,230,200);
					TM_DelayMillis(500);
					score1 = 0;
					play.vidas=3;
					blocksCreated=false;
					play.gameover=false;
					nivelx=1;
				}
			}
			break;
		}
  	}
	return 0;
}
/*Funciones Retardo*/
void TM_Delay_Init(void) {
    RCC_ClocksTypeDef RCC_Clocks;
    /*Obtiene la frecuencia del reloj*/
    RCC_GetClocksFreq(&RCC_Clocks);
    /* While loop takes 4 cycles */
    /* For 1 us delay, we need to divide with 4M */
    delay_multiplier = RCC_Clocks.HCLK_Frequency / 4000000;
}

void TM_DelayMillis(uint32_t millis) {
    /* Multiply millis with multipler */
    /* Substract 10 */
    millis = 1000 * millis * delay_multiplier - 10;
    /* 4 cycles for one loop */
    while (millis--);
}
/*Funciones Generales Juego*/
void GAME_LCD_Init(){
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
	LCD_Clear(BACKCOLOR);
	LCD_SetBackColor(BACKCOLOR);
	//Color de Texto y tamaño de fuente
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font12x12);
	
}
void GAME_LCD_Boar_dPrint(){
	/* Limpiar el LCD */ 
	LCD_SetFont(&Font12x12);
	/*Limpia LCD*/
	LCD_Clear(BACKCOLOR);
	/*Fondo Negro*/
	LCD_SetBackColor(LCD_COLOR_BLACK);
  
	/* Configurar el IO Expander */
  if (IOE_Config() == IOE_OK){
		/*Tamaño de Fuente*/
		LCD_SetFont(&Font16x24);
		/*Color del Texto*/
		LCD_SetTextColor(LCD_COLOR_CYAN);
		/*Impresion del Marco*/
		LCD_DrawRect(1, 35, 283, 237);
  }  
  else{
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

void GPIO_Led_Button(){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);		//ACTIVA CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;	//DEFINE TARGET PIN 13 Y 14	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//TARGET SALIDA
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//OType como PP
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;			//VELOCIDAD
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//MODO NO PULL
  GPIO_Init(GPIOG, &GPIO_InitStructure);						//INICIA LA ESTRUCTURA
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);		//ACTIVA CLK
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; // bit to be set
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; // Input with pull-up resistor
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
