#include "button.h"
#include "LPC17xx.h"
#include "Game/game.h"
extern int down;

void EINT0_IRQHandler (void)	  	/* INT0														 */
{		
	
	LPC_SC->EXTINT &= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{
	//0: pause, 1: play, 2: startup
	LPC_SC->EXTINT &= (1 << 1);     /* clear pending interrupt         */
	if(gameStatus == 0){
		gameStatus = 1;
		unpauseGame();
	}else if(gameStatus == 1){
		gameStatus = 0;
		pauseGame();
	}else if(gameStatus == 2){
		//Startup
		gameStatus = 1; //Play
	}
	
	
	//NVIC_DisableIRQ(EINT1_IRQn);		/* disable Button interrupts			 */
	//LPC_PINCON->PINSEL4    &= ~(1 << 22);     /* GPIO pin selection */
	//down=1;
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */    
	dropTetromino();
}


