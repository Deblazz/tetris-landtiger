/*********************************************************************************************************
**--------------File
* Info---------------------------------------------------------------------------------
** File name:           IRQ_adc.c
** Last modified Date:  20184-12-30
** Last Version:        V1.00
** Descriptions:        functions to manage A/D interrupts
** Correlated files:    adc.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/

#include "Game/game.h"
#include "LPC17xx.h"
#include "adc.h"

#define histeresis 10

/*----------------------------------------------------------------------------
  A/D IRQ: Executed when A/D Conversion is ready (signal from ADC peripheral)
 *----------------------------------------------------------------------------*/

unsigned short AD_current;
volatile short AD_last = 0xFF; /* Last converted value               */

void ADC_IRQHandler(void) {
  AD_current = ((LPC_ADC->ADGDR >> 4) & 0xFFF); /* Read Conversion Result */
  if (abs(AD_current - AD_last) >
      histeresis) { // I use histeresis to prevent flickering
    changeGameSpeed(AD_current);
    AD_last = AD_current;
  }
}
