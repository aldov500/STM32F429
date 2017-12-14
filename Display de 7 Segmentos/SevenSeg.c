#include "SevenSeg.h"


void SevenSeg_Init(){
	GPIO_InitTypeDef	GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	}

void SevenSeg_Set(SevenSeg* newDisplay, bool a, bool b, bool c, bool d, bool e, bool f, bool g){
	newDisplay->a = a;
	newDisplay->b = b;
	newDisplay->c = c;
	newDisplay->d = d;
	newDisplay->e = e;
	newDisplay->f = f;
	newDisplay->g = g;
	}

void SevenSeg_SetInt(SevenSeg *newDisplay, int num){
	switch(num){
		case 0:
			SevenSeg_Set(newDisplay,1,1,1,1,1,1,0);
		break;
		case 1:
			SevenSeg_Set(newDisplay,0,1,1,0,0,0,0);
		break;
		case 2:
			SevenSeg_Set(newDisplay,1,1,0,1,1,0,1);
		break;
		case 3:
			SevenSeg_Set(newDisplay,1,1,1,1,0,0,1);
		break;
		case 4:
			SevenSeg_Set(newDisplay,0,1,1,0,0,1,1);
		break;
		case 5:
			SevenSeg_Set(newDisplay,1,0,1,1,0,1,1);
		break;
		case 6:
			SevenSeg_Set(newDisplay,1,0,1,1,1,1,1);
		break;
		case 7:
			SevenSeg_Set(newDisplay,1,1,1,0,0,0,0);
		break;
		case 8:
			SevenSeg_Set(newDisplay,1,1,1,1,1,1,1);
		break;
		case 9:
			SevenSeg_Set(newDisplay,1,1,1,1,0,1,1);
		break;
		case 10:
			SevenSeg_Set(newDisplay,1,1,1,0,1,1,1);
		break;
		case 11:
			SevenSeg_Set(newDisplay,0,0,1,1,1,1,1);
		break;
		case 12:
			SevenSeg_Set(newDisplay,1,0,0,1,1,1,0);
		break;
		case 13:
			SevenSeg_Set(newDisplay,1,1,1,1,1,1,0);
		break;	
		case 14:
			SevenSeg_Set(newDisplay,1,0,0,1,1,1,1);
		break;
		case 15:
			SevenSeg_Set(newDisplay,1,0,0,0,1,1,1);
		break;
		
		default:
		num = 0;
	}
		
	}

void SevenSeg_Display(SevenSeg newDisplay){
	if(newDisplay.a)	GPIO_SetBits(GPIOD,		GPIO_Pin_0);	
	else	GPIO_ResetBits(GPIOD, GPIO_Pin_0);	
		
	if(newDisplay.b)	GPIO_SetBits(GPIOD, 	GPIO_Pin_1);
	else	GPIO_ResetBits(GPIOD, GPIO_Pin_1);
		
	if(newDisplay.c)	GPIO_SetBits(GPIOD, 	GPIO_Pin_2);		
	else	GPIO_ResetBits(GPIOD, GPIO_Pin_2);
		
	if(newDisplay.d)	GPIO_SetBits(GPIOD, 	GPIO_Pin_3);
	else	GPIO_ResetBits(GPIOD, GPIO_Pin_3);
	
	if(newDisplay.e)	GPIO_SetBits(GPIOD, 	GPIO_Pin_4);
	else	GPIO_ResetBits(GPIOD, GPIO_Pin_4);		
		
	if(newDisplay.f)	GPIO_SetBits(GPIOD, 	GPIO_Pin_5);
	else	GPIO_ResetBits(GPIOD, GPIO_Pin_5);
		
	if(newDisplay.g)	GPIO_SetBits(GPIOD, 	GPIO_Pin_6);
	else 	GPIO_ResetBits(GPIOD, GPIO_Pin_6);
	}

void SevenSeg_Clean(){
	GPIO_ResetBits(GPIOD, GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7);
	}	

void SevenSeg_SwitchDirection(SevenSeg *newDisplay){
		newDisplay->direccion = !newDisplay->direccion;
	}

	