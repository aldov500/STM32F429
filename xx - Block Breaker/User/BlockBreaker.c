#include "BlockBreaker.h"

#define BACKCOLOR 		LCD_COLOR_WHITE

#define PLAYERCOLOR1	LCD_COLOR_RED
#define PLAYERCOLOR2	LCD_COLOR_BLACK
#define HEIGHT				15
#define WIDTH					35
#define LIVES					5
#define LEVEL					0

#define BALLCOLOR		LCD_COLOR_BLUE
#define BALLCIRCL		LCD_COLOR_BLACK
#define BALLRADIO		10


/*	Funct: Configura el Boton de la tarjeta como entrada GPIOA 
 *         Configura los LED de la tarjeta GPIOG - 14 y GPIOG - 13
 *			   Inicializa la LCD
 *  Param: Nada
 *  Progr: Aldo Vargas
 */
void Game_Init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_13 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	
	LCD_Init();
	
	LTDC_LayerCmd(LTDC_Layer1, ENABLE);
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);
	LTDC_ReloadConfig(LTDC_IMReload);
	LTDC_Cmd(ENABLE);
	
	LCD_LayerInit();
	LCD_DisplayOn();
	LCD_SetLayer(LCD_FOREGROUND_LAYER);
	LCD_Clear(LCD_COLOR_WHITE);
	LCD_SetBackColor(LCD_COLOR_BLUE2);
	LCD_SetTextColor(LCD_COLOR_BLACK);
	LCD_SetFont(&Font12x12);
	
}

void Game_TouchInit(){
	LCD_Clear(LCD_COLOR_BLUE2);
	
	if(IOE_Config() == IOE_OK){
	  LCD_SetTextColor(LCD_COLOR_WHITE); 
	  LCD_DisplayStringLine(LCD_LINE_6,(uint8_t*)"      TOUCH ERROR");
	  LCD_DisplayStringLine(LCD_LINE_8,(uint8_t*)"      PUSH RESTART");
		
		LCD_DrawFullRect(10,180,80,80);
		
		
		
		
		
		}
	else{
	
		}
	
}

void Player_Init(Player *p){
	p->X 				= 20;
	p->Y 				= 290;
	p->height 	= HEIGHT;
	p->width 		= WIDTH;
	p->lives 		= LIVES;
	p->level 		= LEVEL;
	p->gameover = false;
		
	}	
void Player_Draw(Player p){
	LCD_SetTextColor(BACKCOLOR);
	LCD_DrawFullRect(p.X, p.Y,p.height,p.width);
	
	LCD_SetTextColor(PLAYERCOLOR1);
	LCD_DrawFullRect(p.X, p.Y, p.width,p.height);
	LCD_SetTextColor(PLAYERCOLOR2);
	LCD_DrawRect(p.X, p.Y, p.height-1,p.width-1);		
		
	}	

void Ball_Init(Ball *b, int X, int Y){
	b->dirX 	= true;
	b->dirY 	= true;
	b->x 			= X;
	b->y 			= Y;
	b->radio 	= BALLRADIO;
	
	}
void Ball_Draw(Ball b){
	LCD_SetTextColor(BACKCOLOR);
	LCD_DrawFullCircle(b.x, b.y, b.radio + 2);
	
	LCD_SetTextColor(BALLCOLOR);
	LCD_DrawFullCircle(b.x, b.y, b.radio);	
	LCD_SetTextColor(BALLCIRCL);
	LCD_DrawCircle(b.x, b.y, b.radio);
	}
void Ball_Move(Ball *ball){
	
	if(ball->x < 225 || ball->x > 15 || ball->y < 305 || ball->y > 15){
		
		if(ball->dirX)	ball->x++;
		else	ball->x--;
	
		if(ball->dirY)	ball->y++;
		else	ball->y--;
		
		if(ball->x <= 15 || ball->x >= 225)		ball->dirX = !ball->dirX;
		
		if(ball->y <= 15 || ball->y >= 305)	ball->dirY = !ball->dirY;		
		
	}
		
	}
