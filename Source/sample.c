#include "GLCD/GLCD.h"
#include "Game/game.h"
#include "Graphics/graphics.h"
#include "LPC17xx.h"
#include "timer/timer.h"


#ifdef SIMULATOR
extern uint8_t
    ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to
               // find the symbol (can be placed also inside system_LPC17xx.h
               // but since it is RO, it needs more work)
#endif

volatile int start_flag = 0;

int main(void) {
  SystemInit(); /* System Initialization (i.e., PLL)  */
  BUTTON_init();
  LCD_Initialization();


  LCD_Clear(Black);

  resetGame();

  while (1) {
		if(gameStatus == 2 && start_flag){
			start_flag = 0;
			startGame();
		}
		
		__WFI();
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
