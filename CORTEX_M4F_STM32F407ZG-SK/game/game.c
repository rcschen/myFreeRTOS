#include "game.h"
#include "main.h"

#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Player1
int16_t player1X = 20;
int16_t player1Y = 10;
uint16_t player1W = 200;
uint16_t player1H = 10;
uint8_t player1IsReversed = 1;

//Player2
int16_t player2X = LCD_PIXEL_WIDTH - 20;
int16_t player2Y = LCD_PIXEL_HEIGHT - 20;
uint16_t player2W = 22;
uint16_t player2H = 10;
uint8_t player2IsReversed = 0;

//Ball
uint16_t ballSize = 5;
//int16_t ballX = ( LCD_PIXEL_WIDTH - 5 ) / 2;
//int16_t ballY = ( LCD_PIXEL_HEIGHT - 5 ) / 2;
int16_t ballX = LCD_PIXEL_WIDTH - 20;
int16_t ballY = LCD_PIXEL_HEIGHT - 20;

//int16_t ballVX = 5;
//int16_t ballVY = 5;
int16_t ballVX = LCD_PIXEL_WIDTH - 20;
int16_t ballVY = LCD_PIXEL_HEIGHT - 20;

uint8_t ballIsRun = 0;

//Mode
uint8_t demoMode = 0;
#define MAX_BALL_NUM  100
Ball ball[MAX_BALL_NUM];

void
BallReset()
{
	//ballX = ( LCD_PIXEL_WIDTH - 5 ) / 2;
	//ballY = ( LCD_PIXEL_HEIGHT - 5 ) / 2;

	ballX = player2X;
	ballY = player2Y;

	ballVX = 5;
	ballVY = 5;

	ballIsRun = 1;
}

void
GAME_EventHandler1()
{
	if( STM_EVAL_PBGetState( BUTTON_USER ) ){

		player1IsReversed = 0;

		while( STM_EVAL_PBGetState( BUTTON_USER ) );

		player1IsReversed = 1;
	}
}

void
GAME_EventHandler2()
{
	if( IOE_TP_GetState()->TouchDetected ){

		player2IsReversed = 1;

		while( IOE_TP_GetState()->TouchDetected );

		player2IsReversed = 0;
	}
}

void
GAME_EventHandler3()
{
	if( ballIsRun == 0 ){
		BallReset();
	}
}
void
PLAYER_Clear(){
	LCD_SetTextColor( LCD_COLOR_BLACK );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	LCD_DrawFullRect( player2X, player2Y, player2W, player2H );
}

//Player1
void
PLAYER1_Reversed(){
	if( player1IsReversed )
		player1X -= 5;
	else
		player1X += 5;

	if( player1X <= 0 )
		player1X = 0;
	else if( player1X + player1W >= LCD_PIXEL_WIDTH )
		player1X = LCD_PIXEL_WIDTH - player1W;
}

//Player2
void
PLAYER2_Reversed() 
{
 	if( player2IsReversed )
		player2X -= 5;
        else
		player2X += 5;


	if( player2X <= 0 )
		player2X = 0;
	else if( player2X + player2W >= LCD_PIXEL_WIDTH )
		player2X = LCD_PIXEL_WIDTH - player2W;

}

void
BALL_Check_Touch_Wall()
{
	//ballX += ballVX;
	if( ballX <= 0 ){
		ballX = 0;
		ballVX *= -1;
	}
	else if( ballX + ballSize >= LCD_PIXEL_WIDTH ){
		ballX = LCD_PIXEL_WIDTH - ballSize;
		ballVX *= -1;
	}

}

void 
BALL_Check_Player2_Loss()
{
	if( ballY + ballSize >= player2Y ){
		if( ballX + ballSize >= player2X && ballX <= player2X + player2W ){
			if( ballX - ballSize <= player2Y + player2W/4 ){
				ballVY =-3;
				//ballVX =-7;
			}
			else if( ballX >= player2Y + player2W - player2W/4 ){
				ballVY =-3;
				//ballVX = 7;
			}
			else if( ballX + ballSize < player2Y + player2W/2 ){
				ballVY =-3;
				//ballVX =-3;
			}
			else if( ballX > player2Y + player2W/2 ){
				ballVY =-3;
				//ballVX = 3;
			}
			else{
				ballVY =-3;
				//ballVX = 0;
			}
		}
		else
			BallReset();
	}

}

void 
BALL_Check_Player1_Loss()
{
	if( ballY <= player1Y + player1H ){
			if( ballX + ballSize >= player1X && ballX <= player1X + player1W ){
				if( ballX - ballSize <= player1Y + player1W/4 ){
					ballVY = 3;
					ballVX =-7;
				}
				else if( ballX >= player1Y + player1W - player1W/4 ){
					ballVY = 3;
					ballVX = 7;
				}
				else if( ballX + ballSize < player1Y + player1W/2 ){
					ballVY = 7;
					ballVX =-3;
				}
				else if( ballX > player1Y + player1W/2 ){
					ballVY = 7;
					ballVX = 3;
				}
				else{
					ballVY = 9;
					ballVX = 0;
				}
			}
			else
				BallReset();
	}

}
void
GAME_Update()
{
        PLAYER_Clear();
        PLAYER1_Reversed();
        PLAYER2_Reversed();

	//Ball
	if( ballIsRun == 1 ){

		LCD_SetTextColor( LCD_COLOR_BLACK );
		LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );
                
		//Touch wall
                BALL_Check_Touch_Wall();

		//PONG!
		ballY += ballVY;
                BALL_Check_Player2_Loss();
                BALL_Check_Player1_Loss();

	}
}

void
GAME_Render()
{
	LCD_SetTextColor( LCD_COLOR_WHITE );
	LCD_DrawFullRect( player1X, player1Y, player1W, player1H );
	LCD_DrawFullRect( player2X, player2Y, player2W, player2H );
	LCD_SetTextColor( LCD_COLOR_YELLOW );
	LCD_DrawFullRect( ballX, ballY, ballSize, ballSize );
	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DrawLine( 10, LCD_PIXEL_HEIGHT / 2, LCD_PIXEL_WIDTH - 20, LCD_DIR_HORIZONTAL );
}
