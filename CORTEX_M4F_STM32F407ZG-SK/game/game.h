
#ifndef GAME_H
#define GAME_H
#include "main.h"

void
EventHandler1();

void
EventHandler2();

void
EventHandler3();

void
GAME_Update();

void
GAME_Render();

typedef struct Ball {
  uint16_t ballSize ;
  int16_t ballX  ;
  int16_t ballY ;
  int16_t ballVX ;
  int16_t ballVY ;
} Ball;

#endif
