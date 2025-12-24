#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

typedef enum{
	PAUSE = 0,
	PLAY
	
} GameStatus;

extern uint8_t gameField[10][20]; 
extern uint8_t newTetromino[4][4];
extern uint8_t newTetrominoXpos;
extern uint8_t newTetrominoYpos;
extern uint32_t topScore;
extern uint32_t currentScore;
extern uint16_t clearedRows;
extern volatile GameStatus gameStatus;



void initGame(void);
void genRandomTetromino();

#endif