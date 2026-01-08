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
 
volatile int key1_debounce = 0;
volatile int key2_debounce = 0;  
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
	static int left_counter = 0;
	static int right_counter = 0;
	static int rotate_counter = 0;
	
	if (key1_debounce > 0) key1_debounce--;
	if (key2_debounce > 0) key2_debounce--;

	if(gameStatus == 1){
		if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {
			rotate_counter++;
			if (rotate_counter > 8){
				rotateTetromino();
				rotate_counter = 0;
			}
		} else {
			rotate_counter = 8;
		}

		if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {
			right_counter++;
			if(right_counter > 4){
				moveRight();
				right_counter = 0;
			}
		} else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) {
			left_counter++;
			if(left_counter > 4){
				moveLeft();
				left_counter = 0;
			}
		} else {
			right_counter = 4;
			left_counter = 4;
		}

		if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) {
			down_counter++;
			if (down_counter >= 20) {
				movePieceDown();
				down_counter = 0;
				reset_timer(1);
				enable_timer(1);
			}
		} else {
			down_counter = 0;
		}
	}
}

/******************************************************************************
**                            End Of File
******************************************************************************/
