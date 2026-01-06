/*********************************************************************************************************
**--------------File
* Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "Game/game.h"
#include "LPC17xx.h"
#include "timer.h"
#include <string.h>

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER0_IRQHandler(void) {
  // Do nothing
  LPC_TIM0->IR = 1; /* clear interrupt flag */
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler(void) {
  LPC_TIM1->IR = 1; /* clear interrupt flag */
  if (gameStatus ==
      1) { // Timer 1 is disabled during pause, this is just for added safety
    // During play
    movePieceDown();
  }
  return;
}

void TIMER2_IRQHandler(void) {
  LPC_TIM2->IR = 1;
  static int down_counter = 0;
  if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {
    rotateTetromino();
		down_counter  = 0;
  } else if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {
    // Right
    moveRight();
		down_counter = 0;
  } else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) {
    // Left
    moveLeft();
		down_counter = 0;
  } else if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) {
    down_counter++;

    // How many rit cycles needed for a down movement
    if (down_counter >= 20) {
      movePieceDown();
      down_counter = 0;

      // Block timer so not to have repetitions
      reset_timer(1);
      enable_timer(1);
    }
  } else {
    down_counter = 0;
  }
}

/******************************************************************************
**                            End Of File
******************************************************************************/
