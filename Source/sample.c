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
SystemInit(); 
  
  // --- CONFIGURAZIONE AUDIO DAC ---
  // 1. Configura P0.26 come AOUT (DAC) - Funzione '10' binaria sui bit 21:20
  LPC_PINCON->PINSEL1 &= ~(3 << 20); // Pulisci
  LPC_PINCON->PINSEL1 |=  (2 << 20); // Imposta DAC
  
  // 2. Accendi timer 0, 1 (gioco), 2 (tasti), 3 (durata note)
  // Nota: PCONP bit 15 è il DAC, bit 1/2/22/23 sono i timer
  LPC_SC->PCONP |= (1 << 15) | (1 << 1) | (1 << 2) | (1 << 22) | (1 << 23);
  // --------------------------------

  BUTTON_init();
	ADC_init();
  LCD_Initialization();
  LCD_Clear(Black);
	init_timer(3, 0x017D7840);
  init_RIT(0x004C4B40);
	
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
