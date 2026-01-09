#include "LPC17xx.h"
#include "music/music.h"
#include "../timer/timer.h"
#include "sounds/sounds.h"

// Variabili globali di gestione audio
NOTE* currentMelody = BGM_TETRIS;
int currentNoteIndex = 0;
int currentMelodySize = 0; 

// --- NOVITÀ: Variabile per salvare il punto della canzone ---
int savedMusicIndex = 0; 
// ----------------------------------------------------------

int isSFXPlaying = 0; 
extern volatile int gameStatus;

// Funzione per avviare un suono (SFX)
void playSound(NOTE* sfx_array, int size) {
    // 1. Se stiamo ascoltando MUSICA, salviamo il punto in cui siamo arrivati.
    // Se stiamo già suonando un altro effetto (isSFXPlaying == 1), 
    // NON sovrascrivere il salvataggio, altrimenti perdiamo il segno della musica.
    if (!isSFXPlaying) {
        savedMusicIndex = currentNoteIndex;
    }

    disable_timer(3); // Ferma nota corrente
    disable_timer(0); // Zittisci
    reset_timer(3);
    
    currentMelody = sfx_array;
    currentMelodySize = size;
    currentNoteIndex = 0; // L'effetto parte dall'inizio
    isSFXPlaying = 1; 
    
    playNote(currentMelody[0]);
    currentNoteIndex++;
}

// Funzione per tornare alla musica di sottofondo
void resumeMusic() {
    disable_timer(0); // Zittisci per sicurezza
    
		if (gameStatus == 3) {
        isSFXPlaying = 0;
        disable_RIT(); // Spegne il motore audio. Verrà riacceso da startGame()
        return;
    }
	
    currentMelody = BGM_TETRIS;
    currentMelodySize = BGM_SIZE;
    isSFXPlaying = 0;
    
    // Se è una nuova partita (reset), riparti da 0.
    if (gameStatus == 2) { 
        currentNoteIndex = 0; 
    } else {
        // --- RIPRISTINA IL PUNTO SALVATO ---
        currentNoteIndex = savedMusicIndex; 
        // -----------------------------------
    }
}

void RIT_IRQHandler (void)
{
    // Inizializzazione "lazy" al primo avvio
    if (currentMelodySize == 0) currentMelodySize = BGM_SIZE;

    // Se il Timer 3 (durata) sta ancora contando, NON fare nulla.
    if(isNotePlaying()) {
          LPC_RIT->RICTRL |= 0x1; 
          return;
    }

    // Se la nota è finita, controlla se la melodia corrente è finita
    if(currentNoteIndex >= currentMelodySize) {
        if (isSFXPlaying) {
            // Finito l'effetto sonoro, torna alla musica
            resumeMusic(); 
            LPC_RIT->RICTRL |= 0x1;
            return; // Aspetta il prossimo giro per suonare la nota musicale ripristinata
        } else {
            // Finita la musica -> Loop (Ricomincia da capo)
            currentNoteIndex = 0;
        }
    }
    
    // Suona la prossima nota
    playNote(currentMelody[currentNoteIndex++]);
    
    LPC_RIT->RICTRL |= 0x1;
}