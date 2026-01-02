#include "Game/game.h"
#include "LPC17xx.h"
#include "button.h"

extern int down;

void EINT0_IRQHandler(void) /* INT0
                             */
{

  LPC_SC->EXTINT &= (1 << 0); /* clear pending interrupt         */
}

void EINT1_IRQHandler(void) /* KEY1
                             */
{
  // 0: pause, 1: play, 2: startup
  LPC_SC->EXTINT &= (1 << 1); /* clear pending interrupt         */
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
		gameStatus = 1;
    break;
  case 3:
    resetGame();
    break;
  }

  // NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts
  // */ LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
  // down=1;
}

void EINT2_IRQHandler(void) /* KEY2
                             */
{
  LPC_SC->EXTINT &= (1 << 2); /* clear pending interrupt         */
  dropTetromino();
}
