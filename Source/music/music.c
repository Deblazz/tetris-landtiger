#include "music.h"
#include "timer/timer.h" // Assicurati del percorso giusto

void playNote(NOTE note) {
    if (note.freq != pause) {
        reset_timer(0);
        // Timer 0 genera interrupt veloci per il DAC
        init_timer(0, note.freq); 
        enable_timer(0);
    } else {
        disable_timer(0);
    }
    
    // Timer 3 gestisce quanto dura la nota
    reset_timer(3);
    init_timer(3, note.duration);
    enable_timer(3);
}

BOOL isNotePlaying(void) {
    // La nota suona finché Timer 3 (durata) è attivo
    return (LPC_TIM3->TCR & 1);
}