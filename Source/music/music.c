#include "music.h"
#include "timer/timer.h"

void playNote(NOTE note) {
  if (note.freq != pause) {
    reset_timer(0);
    init_timer(0, note.freq);
    enable_timer(0);
  } else {
    disable_timer(0);
  }

  reset_timer(3);
  init_timer(3, note.duration);
  enable_timer(3);
}

BOOL isNotePlaying(void) { return (LPC_TIM3->TCR & 1); }