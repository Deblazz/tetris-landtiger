#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  PAUSE = 0,
  PLAY,
  INIT,
  END

} GameStatus;

extern uint8_t gameField[20][10];
extern uint8_t newTetromino[4][4];
extern int newTetrominoXpos;
extern int newTetrominoYpos;
extern uint8_t newTetrominoRotation;
extern uint32_t topScore;
extern uint32_t currentScore;
extern uint16_t clearedRows;
extern volatile GameStatus gameStatus;

void endGame();
void genRandomTetromino();
uint8_t checkContact();
void lockTetromino();
void rotateTetromino();
void moveRight();
void moveLeftt();
void dropTetromino();
void checkAndClearRows();
void startGame();
void resetgame();
void animateTetrisEffect(uint8_t rows[]);
void delay_ms(uint32_t ms);
uint8_t isPositionValid(int newTetrominoXpos, int newTetrominoYpos,
                        const uint8_t newTetromino[4][4]);
#endif