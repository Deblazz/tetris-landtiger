#include "../timer/timer.h"
#include "LPC17xx.h"
#include "music/music.h"
#include "sounds/sounds.h"

NOTE *currentMelody = BGM_TETRIS;
int currentNoteIndex = 0;
int currentMelodySize = 0;

// Restart music where it was interrupted after a sfx
int savedMusicIndex = 0;

int isSFXPlaying = 0;
extern volatile int gameStatus;

// Funzione per avviare un suono (SFX)
void playSound(NOTE *sfx_array, int size) {

  // Saving music index to start from after SFX
  disable_RIT();
  if (!isSFXPlaying) {
    savedMusicIndex = currentNoteIndex;
  }

  disable_timer(3);
  disable_timer(0);
  reset_timer(3);

  currentMelody = sfx_array;
  currentMelodySize = size;
  currentNoteIndex = 0; // Start sfx
  isSFXPlaying = 1;

  playNote(currentMelody[0]);
  currentNoteIndex++;

  enable_RIT();
}

// Resume music where left
void resumeMusic() {
  disable_timer(0);

  if (gameStatus == 3) {
    isSFXPlaying = 0;
    disable_RIT();
    return;
  }

  currentMelody = BGM_TETRIS;
  currentMelodySize = BGM_SIZE;
  isSFXPlaying = 0;

  // Start over when reset
  if (gameStatus == 2) {
    currentNoteIndex = 0;
  } else {
    // Start where left
    currentNoteIndex = savedMusicIndex;
  }
}

void RIT_IRQHandler(void) {
  if (currentMelodySize == 0)
    currentMelodySize = BGM_SIZE;

  if (isNotePlaying()) {
    LPC_RIT->RICTRL |= 0x1; // If note has not finished do notihng
    return;
  }

  // has song finished?
  if (currentNoteIndex >= currentMelodySize) {
    if (isSFXPlaying) {
      // Next loop will make the song loop after sfx reproduction
      resumeMusic();
      LPC_RIT->RICTRL |= 0x1;
      return;
    } else {
      // Loop the song
      currentNoteIndex = 0;
    }
  }

  playNote(currentMelody[currentNoteIndex++]);

  LPC_RIT->RICTRL |= 0x1;
}