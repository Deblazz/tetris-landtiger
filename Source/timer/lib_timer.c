#include "LPC17xx.h"
#include "timer.h"
/******************************************************************************
** Function name:		enable_timer
**
** Descriptions:		Enable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void enable_timer(uint8_t timer_num) {
  if (timer_num == 0) {
    LPC_TIM0->TCR = 1;
  } else if (timer_num == 1) {
    LPC_TIM1->TCR = 1;
  } else if (timer_num == 2) {
    LPC_TIM2->TCR = 1;
  } else if (timer_num == 3) {
    LPC_TIM3->TCR = 1;
  }
  return;
}

/******************************************************************************
** Function name:		disable_timer
**
** Descriptions:		Disable timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void disable_timer(uint8_t timer_num) {
  if (timer_num == 0) {
    LPC_TIM0->TCR = 0;
  } else if (timer_num == 1) {
    LPC_TIM1->TCR = 0;
  } else if (timer_num == 2) {
    LPC_TIM2->TCR = 0;
  } else if (timer_num == 3) {
    LPC_TIM3->TCR = 0;
  }
  return;
}

/******************************************************************************
** Function name:		reset_timer
**
** Descriptions:		Reset timer
**
** parameters:			timer number: 0 or 1
** Returned value:		None
**
******************************************************************************/
void reset_timer(uint8_t timer_num) {
  uint32_t regVal;
  if (timer_num == 0) {
    regVal = LPC_TIM0->TCR;
    regVal |= 0x02;
    LPC_TIM0->TCR = regVal;
  } else if (timer_num == 1) {
    regVal = LPC_TIM1->TCR;
    regVal |= 0x02;
    LPC_TIM1->TCR = regVal;
  } else if (timer_num == 2) {
    regVal = LPC_TIM2->TCR;
    regVal |= 0x02;
    LPC_TIM2->TCR = regVal;
  } else if (timer_num == 3) {
		regVal = LPC_TIM3->TCR;
    regVal |= 0x02;
    LPC_TIM3->TCR = regVal;
	}
  return;
}

uint32_t init_timer(uint8_t timer_num, uint32_t TimerInterval) {
if (timer_num == 0) {
        // --- TIMER 0: FREQUENZA (PILOTA IL DAC) ---
        LPC_TIM0->MR0 = TimerInterval;
        
        // MCR = 3: Interrupt (bit 0) + Reset (bit 1) su MR0
        // Questo è fondamentale per la sinusoide!
        LPC_TIM0->MCR = 3; 
				NVIC_SetPriority(TIMER0_IRQn, 0);
        NVIC_EnableIRQ(TIMER0_IRQn); // Abilita Interrupt
        return (1);
    
  } else if (timer_num == 1) {
    LPC_SC->PCONP |= (1 << 2);
    LPC_TIM1->MR0 = TimerInterval;
    LPC_TIM1->MCR = 3;
    NVIC_EnableIRQ(TIMER1_IRQn);
    return (1);
  } else if (timer_num == 2) {
    LPC_SC->PCONP |= (1 << 22);
    LPC_TIM2->MR0 = TimerInterval;
    LPC_TIM2->MCR = 3;
    NVIC_EnableIRQ(TIMER2_IRQn);
    return (1);
  }else if (timer_num == 3) {
        // --- TIMER 3: DURATA NOTA (Sostituisce Timer 1 dell'esempio audio) ---
        LPC_TIM3->MR0 = TimerInterval;
        
        // MCR = 7: Interrupt (bit 0) + Reset (bit 1) + STOP (bit 2)
        // Il timer si ferma da solo quando la nota finisce
        LPC_TIM3->MCR = 7; 
        
        NVIC_EnableIRQ(TIMER3_IRQn);
        return (1);
    }
  return (0);
}

/***************************************************************CUSTOM
 * FUNCTIONS*************************/
uint32_t get_timer_value(uint8_t timer_num) {
  if (timer_num == 0) {
    return LPC_TIM0->TC;
  } else {
    return LPC_TIM1->TC;
  }
}

//returns the correct threshold (how many ticks) to use for soft drop based on current game speed

uint32_t get_dynamic_value(){
	uint32_t gravity = LPC_TIM1->MR0;
	uint32_t ticks = LPC_TIM2->MR0;
	
	uint32_t current_ratio = gravity / ticks;
	
	uint32_t dynamic_value = current_ratio / 2;
	
	dynamic_value = dynamic_value?dynamic_value:1; //Used as backup in case of dynamic_value 0 division
	
	return dynamic_value;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
