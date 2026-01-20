#include "LPC17xx.h"
#include "dac.h"

void DAC_init() {
  // Audio dac config
  LPC_PINCON->PINSEL1 &= ~(3 << 20);
  LPC_PINCON->PINSEL1 |= (2 << 20);

  LPC_SC->PCONP |= (1 << 15);
}