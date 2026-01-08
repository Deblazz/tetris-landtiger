#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
  /* joystick up functionality */
  LPC_PINCON->PINSEL3 &= ~(3 << 26); // PIN mode GPIO (00b value per P1.29)
  LPC_GPIO1->FIODIR &=
      ~(1 << 29); // P1.25 Input (joysticks on PORT1 defined as Input)
}
