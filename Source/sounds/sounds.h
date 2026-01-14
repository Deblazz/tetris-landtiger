#ifndef SOUNDS_H
#define SOUNDS_H

#include "music/music.h"

// --- DICHIARAZIONE ARRAY (Diciamo che esistono) ---
extern NOTE BGM_TETRIS[];
extern int BGM_SIZE;

extern NOTE SFX_PLACE[];
extern int SFX_PLACE_SIZE;

extern NOTE SFX_LINE[];
extern int SFX_LINE_SIZE;

extern NOTE SFX_TETRIS[];
extern int SFX_TETRIS_SIZE;

extern NOTE SFX_GAMEOVER[];
extern int SFX_GAMEOVER_SIZE;

extern NOTE SFX_POWERUP[];
extern int SFX_POWERUP_SIZE;

extern NOTE SFX_MALUS[];
extern int SFX_MALUS_SIZE;

// --- DICHIARAZIONE FUNZIONI ---
// Così game.c sa che questa funzione esiste in IRQ_RIT.c
void playSound(NOTE* sfx_array, int size);

#endif