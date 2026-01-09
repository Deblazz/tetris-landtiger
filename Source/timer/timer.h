#ifndef __TIMER_H
#define __TIMER_H

#include "../music/music.h"
#include "LPC17xx.h"

/* init_timer.c */
extern uint32_t init_timer(uint8_t timer_num, uint32_t timerInterval);
extern void enable_timer(uint8_t timer_num);
extern void disable_timer(uint8_t timer_num);
extern void reset_timer(uint8_t timer_num);
extern uint32_t get_timer_value(uint8_t timer_num);
/* IRQ_timer.c */
extern void TIMER0_IRQHandler(void);
extern void TIMER1_IRQHandler(void);

#endif /* end __TIMER_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
