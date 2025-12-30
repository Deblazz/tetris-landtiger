#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef enum{
	PAUSE = 0,
	PLAY
	
} GameStatus;

extern uint8_t gameField[20][10]; 
extern uint8_t newTetromino[4][4];
extern uint8_t newTetrominoXpos;
extern uint8_t newTetrominoYpos;
extern uint8_t newTetrominoRotation;
extern uint32_t topScore;
extern uint32_t currentScore;
extern uint16_t clearedRows;
extern volatile GameStatus gameStatus;



void initGame();
void endGame();
void genRandomTetromino();
uint8_t checkContact();
void lockTetromino();
void rotateTetromino();
void moveRight();
void moveLeftt();
#endif