#ifndef __BLOCKBREAKER_H
#define __BLOCKBREAKER_H

/* DISCOVERY */
#include "stm32f4xx.h"
#include "stm32f429i_discovery.h"
#include "stm32f429i_discovery_lcd.h"
#include "stm32f429i_discovery_ioe.h"

/* SYSTEM*/
#include "stdbool.h"

void Game_Init();
void Game_TouchInit();

/* PLAYER */
typedef struct Player {
	int X, Y;
	int height, width;
	int lives, level;
	bool gameover;
	
	} Player;

void Player_Init(Player *p);
void Player_Draw(Player p);

/* BALL */
typedef struct Ball {
	int x,y,radio;
	bool dirX, dirY;
	} Ball;

void Ball_Init(Ball *b, int X, int Y);
void Ball_Draw(Ball b);
void Ball_Move(Ball* ball);
	

#endif
