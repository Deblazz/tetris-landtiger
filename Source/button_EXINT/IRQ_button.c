#include "Game/game.h"
#include "LPC17xx.h"
#include "button.h"

extern int down;
extern volatile int key1_debounce;
extern volatile int key2_debounce;
extern volatile int start_flag;
void EINT0_IRQHandler(void) /* INT0
                             */
{

  LPC_SC->EXTINT &= (1 << 0); /* clear pending interrupt         */
}

void EINT1_IRQHandler(void) /* KEY1                             */
{
  LPC_SC->EXTINT &= (1 << 1); /* clear pending interrupt         */
  if (key1_debounce > 0)
    return;
  // 0: pause, 1: play, 2: startup
  switch (gameStatus) {
  case 0:
    gameStatus = 1;
    unpauseGame();
    break;
  case 1:
    gameStatus = 0;
    pauseGame();
    break;
  case 2:
    start_flag = 1;
    break;
  case 3:
    resetGame();
    break;
  }
  key1_debounce = 20; //.5s
}

void EINT2_IRQHandler(void) /* KEY2
                             */
{
  LPC_SC->EXTINT &= (1 << 2); /* clear pending interrupt         */
  if (key2_debounce > 0)
    return;
  if (gameStatus == 1) {
    dropTetromino();
  }
  key2_debounce = 20; //.5s
}
