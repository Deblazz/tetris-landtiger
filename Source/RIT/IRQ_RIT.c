/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int down=0;

void RIT_IRQHandler (void)
{						
    if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
            rotateTetromino();
    }
    else{			
			if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
				//Right 
				moveRight();
			}else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
				//Left
				moveLeft();
			}
    }    
    LPC_RIT->RICTRL |= 0x1;	/* Pulisci flag interrupt */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
