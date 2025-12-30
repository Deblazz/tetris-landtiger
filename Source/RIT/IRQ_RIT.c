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
		static int down_counter = 0;
    if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
            rotateTetromino();
    }else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
				//Right 
				moveRight();
			}else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
				//Left
				moveLeft();
			}else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){	
        down_counter++;
        
        //How many rit cycles needed for a down movement
        if(down_counter >= 2){ 
					movePieceDown(); 
          down_counter = 0;
					
					//Block timer so not to have repetitions
					reset_timer(1);
					enable_timer(1);
        }
    }
    else{
        down_counter = 0;
    } 
    LPC_RIT->RICTRL |= 0x1;	/* Pulisci flag interrupt */
}

/******************************************************************************
**                            End Of File
******************************************************************************/
