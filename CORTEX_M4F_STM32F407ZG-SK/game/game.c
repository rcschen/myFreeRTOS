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
#define MAX_BALL_NUM  20
Ball ball[MAX_BALL_NUM];

void
Init_L3GD20()
{
    L3GD20_InitTypeDef L3GD20_InitStructure;
    L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
    L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
    L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
    L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
    L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
    L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
    L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_250;
    L3GD20_Init(&L3GD20_InitStructure);
}


void
Ball_Single_Reset( Ball *b, int size )
{

       b->ballX = player2X;
       b->ballY = player2Y;

       b->ballVX = 5;
       b->ballVY = 5;
       
       b->ballSize = 5;
       b->isRun = 1;
       b->canRun = 0;

}

void
BallReset()
{
    ballX = player2X;
    ballY = player2Y;
    ballVX = 5;
    ballVY = 5;
    ballIsRun = 1;
    int i = 0;
    for(; i < MAX_BALL_NUM; i++) {
        Ball_Single_Reset( &ball[i],i );
    }
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
        int i = 0;
        for(; i < MAX_BALL_NUM; i++) {
            if( ball[i].isRun == 0 ) {
               Ball_Single_Reset( &ball[i],i );
            }
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
 	if( player2IsReversed ) {
		player2X -= 5;
                int i = 0;
                for(;i< MAX_BALL_NUM; i++){
                    if ( ball[i].isRun == 1 && ball[i].canRun == 0) {
                       ball[i].ballX = player2X;
                       ball[i].ballY = player2Y;

                       ball[i].canRun = 1 ;
                       break;
                      
                    }
                }
        }
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
	if( ballX <= 0 ){
		ballX = 0;
		ballVX *= -1;
	}
	else if( ballX + ballSize >= LCD_PIXEL_WIDTH ){
		ballX = LCD_PIXEL_WIDTH - ballSize;
		ballVX *= -1;
	}
    int i = 0;
    for(; i < MAX_BALL_NUM; i++) {

	if( ball[i].ballX <= 0 ){
		ball[i].ballX = 0;
		ball[i].ballVX *= -1;
	}
	else if( ballX + ballSize >= LCD_PIXEL_WIDTH ){
		ball[i].ballX = LCD_PIXEL_WIDTH - ballSize;
		ball[i].ballVX *= -1;
	}
    }
        

}

void 
BALL_Check_Player2_Loss()
{
	if( ballY + ballSize >= player2Y ){
		if( ballX + ballSize >= player2X && ballX <= player2X + player2W ){
			if( ballX - ballSize <= player2Y + player2W/4 ){
				ballVY =-3;
			}
			else if( ballX >= player2Y + player2W - player2W/4 ){
				ballVY =-3;
			}
			else if( ballX + ballSize < player2Y + player2W/2 ){
				ballVY =-3;
			}
			else if( ballX > player2Y + player2W/2 ){
				ballVY =-3;
			}
			else{
				ballVY =-3;
			}
		}
		else
			BallReset();
	}
    int i = 0;
    for(; i < MAX_BALL_NUM; i++) {
	if( ball[i].ballY + ball[i].ballSize >= player2Y ){
           if( ball[i].ballX + ball[i].ballSize >= player2X && ball[i].ballX <= player2X + player2W ){
		if( ball[i].ballX - ball[i].ballSize <= player2Y + player2W/4 ){
			ball[i].ballVY =-3;
		}
		else if( ball[i].ballX >= player2Y + player2W - player2W/4 ){
			ball[i].ballVY =-3;
		}
		else if( ball[i].ballX + ball[i].ballSize < player2Y + player2W/2 ){
			ball[i].ballVY =-3;
		}
		else if( ball[i].ballX > player2Y + player2W/2 ){
			ball[i].ballVY =-3;
		}
		else{
			ball[i].ballVY =-3;
		}
           }
           else
		BallReset();
        }
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
    int i = 0;
    for(; i < MAX_BALL_NUM; i++) {

	if( ball[i].ballY <= player1Y + player1H ){
           if( ball[i].ballX + ball[i].ballSize >= player1X && ball[i].ballX <= player1X + player1W ){
		if( ball[i].ballX - ball[i].ballSize <= player1Y + player1W/4 ){
			ball[i].ballVY = 3;
			ball[i].ballVX =-7;
	 	}
		else if( ball[i].ballX >= player1Y + player1W - player1W/4 ){
			ball[i].ballVY = 3;
			ball[i].ballVX = 7;
		}
		else if( ball[i].ballX + ballSize < player1Y + player1W/2 ){
			ball[i].ballVY = 7;
			ball[i].ballVX =-3;
		}
		else if( ball[i].ballX > player1Y + player1W/2 ){
			ball[i].ballVY = 7;
			ball[i].ballVX = 3;
		}
		else{
			ball[i].ballVY = 9;
			ball[i].ballVX = 0;
		}
          }
          else
		BallReset();
      }
   }
}

void
DrawBalls( int16_t color ){
   int i = 0;
   for(; i < MAX_BALL_NUM; i++) {
       if (ball[i].isRun == 1) {
         LCD_SetTextColor( color );
         LCD_DrawFullRect( ball[i].ballX, ball[i].ballY, ball[i].ballSize, ball[i].ballSize );
       }
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
        int i = 0;
        for(;i < MAX_BALL_NUM; i++ ){
            if( ball[i].isRun == 1 ){

                DrawBalls(LCD_COLOR_BLACK);
		//Touch wall
                BALL_Check_Touch_Wall();

		//PONG!
                ball[i].ballY += ball[i].ballVY;
                BALL_Check_Player2_Loss();
                BALL_Check_Player1_Loss();
           }

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
        DrawBalls(LCD_COLOR_BLUE);
	LCD_SetTextColor( LCD_COLOR_RED );
	LCD_DrawLine( 10, LCD_PIXEL_HEIGHT / 2, LCD_PIXEL_WIDTH - 20, LCD_DIR_HORIZONTAL );
}
