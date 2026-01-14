#include "Game/game.h"
#include "LPC17xx.h"
#include "timer.h"
#include <string.h>

volatile int key1_debounce = 0;
volatile int key2_debounce = 0;
extern uint8_t timerPowerupSpeed;
extern uint16_t powerup_counter_ticks;
/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

uint16_t SinTable[45] = {
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};

// --- TIMER 0: Genera l'onda sonora ---
void TIMER0_IRQHandler(void) {
	   LPC_TIM0->IR = 1; // Clear Interrupt
    static int sineticks = 0;
    static int currentValue;
    
    // Logica DAC
    currentValue = SinTable[sineticks];
    currentValue -= 410;
    currentValue /= 1; // Volume (dividi per 1 = max, per 2, 4, 8 abbassi)
    currentValue += 410;
    
    LPC_DAC->DACR = currentValue << 6; // Scrive al DAC
    
    sineticks++;
    if(sineticks == 45) sineticks = 0;
    
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

//Timer 2 is used for Key debounce, joystick polling and pot polling

void TIMER2_IRQHandler(void) {
  LPC_TIM2->IR = 1;
  static int down_counter = 0;
  static int left_counter = 0;
  static int right_counter = 0;
  static int rotate_counter = 0;

  if (key1_debounce > 0)
    key1_debounce--;
  if (key2_debounce > 0)
    key2_debounce--;

  if (gameStatus == 1) {
    if ((LPC_GPIO1->FIOPIN & (1 << 29)) == 0) {
      rotate_counter++;
      if (rotate_counter > 8) {
        rotateTetromino();
        rotate_counter = 0;
      }
    } else {
      rotate_counter = 8;
    }

    if ((LPC_GPIO1->FIOPIN & (1 << 28)) == 0) {
      right_counter++;
      if (right_counter > 4) {
        moveRight();
        right_counter = 0;
      }
    } else if ((LPC_GPIO1->FIOPIN & (1 << 27)) == 0) {
      left_counter++;
      if (left_counter > 4) {
        moveLeft();
        left_counter = 0;
      }
    } else {
      right_counter = 4;
      left_counter = 4;
    }

    if ((LPC_GPIO1->FIOPIN & (1 << 26)) == 0) {
      down_counter++;
      if (down_counter >= get_dynamic_value()) {
        movePieceDown();
        down_counter = 0;
        reset_timer(1);
        enable_timer(1);
      }
    } else {
      down_counter = 0;
    }
		
		//Pot management
		if(timerPowerupSpeed){
			powerup_counter_ticks ++;
			//300*50ms =15s
			if(powerup_counter_ticks >= 300){
				timerPowerupSpeed = 0;
				powerup_counter_ticks = 0;
				ADC_start_conversion();
			}
		}else{
			ADC_start_conversion();

		}
  }
}

void TIMER3_IRQHandler(void) {
    LPC_TIM3->IR = 1;      // Clear Flag
    disable_timer(0);      // Zittisci Timer 0 (Spegni interrupt DAC)
    // Timer 3 si ferma da solo grazie a MCR = 7
}

/******************************************************************************
**                            End Of File
******************************************************************************/
