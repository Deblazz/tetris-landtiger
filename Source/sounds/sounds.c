#include "music/music.h"

// --- 0. BACKGROUND MUSIC (Tetris Loop) ---
// (Usa l'array 'song' che abbiamo definito prima, rinominalo in BGM_TETRIS)

NOTE BGM_TETRIS[] = {

    // ============================
    // PARTE A (Tema Principale)
    // ============================
    
    // Battuta 1: Mi... Si-Do
    {e4, time_semiminima},
    {b3, time_croma},
    {c4, time_croma},
    
    // Battuta 2: Re... Do-Si
    {d4, time_semiminima},
    {c4, time_croma},
    {b3, time_croma},
    
    // Battuta 3: La... La-Do
    {a3, time_semiminima},
    {a3, time_croma},
    {c4, time_croma},
    
    // Battuta 4: Mi... Re-Do
    {e4, time_semiminima},
    {d4, time_croma},
    {c4, time_croma},
    
    // Battuta 5: Si... Si-Do
    {b3, time_semiminima}, // Si lungo
    {b3, time_croma},      // Si ribattuto (spesso mancava questo!)
    {c4, time_croma},
    
    // Battuta 6: Re... Mi...
    {d4, time_semiminima},
    {e4, time_semiminima},
    
    // Battuta 7: Do... La...
    {c4, time_semiminima},
    {a3, time_semiminima},
    
    // Battuta 8: La... (Pausa)
    {a3, time_semiminima}, // La finale
    {pause, time_semiminima}, // Pausa necessaria per il ritmo
    
    // ============================
    // PARTE B (Tema Alto / Coro)
    // ============================
    
    // Battuta 9: Re... Fa-La (Salita)
    {d4, time_semiminima}, // Qui inizia la parte "alta"
    {f4, time_croma},
    {a4, time_semiminima}, // Nota più alta (La4)
    
    // Battuta 10: Sol-Fa
    {g4, time_croma},
    {f4, time_croma},
    
    // Battuta 11: Mi... Do-Mi
    {e4, time_semiminima}, // Mi lungo
    {c4, time_croma},      // Do di passaggio
    {e4, time_semiminima}, // Torna al Mi
    
    // Battuta 12: Re-Do
    {d4, time_croma},
    {c4, time_croma},
    
    // Battuta 13: Si... Si-Do (Ritorno al tema basso)
    {b3, time_semiminima},
    {b3, time_croma},      // Ribattuto
    {c4, time_croma},
    
    // Battuta 14: Re... Mi...
    {d4, time_semiminima},
    {e4, time_semiminima},
    
    // Battuta 15: Do... La...
    {c4, time_semiminima},
    {a3, time_semiminima},
    
    // Battuta 16: La... (Pausa Loop)
    {a3, time_semiminima},
    {pause, time_semiminima}
};


// --- 1. BLOCK PLACED (Un tonfo secco e basso) ---
NOTE SFX_PLACE[] = {
    {e4, time_semicroma}, // Nota più acuta (E4 invece di C3)
    {pause, time_semibiscroma} 
};

// --- 2. LINE CLEAR (Salita ottimista) ---
NOTE SFX_LINE[] = {
    {c4, time_semicroma},
    {e4, time_semicroma},
    {g4, time_semicroma}
};

// --- 3. TETRIS (Arpeggio veloce e acuto) ---
NOTE SFX_TETRIS[] = {
    {b4, time_biscroma},
    {d4, time_biscroma}, // Assicurati di avere le note ottava 5 in music.h, altrimenti usa d4/f4/a4
    {f4, time_biscroma},
    {a4, time_croma}
};

// --- 4. GAME OVER (Tritono discendente / Dissonante) ---
NOTE SFX_GAMEOVER[] = {
    {d4, time_semiminima},
    {c3b, time_semiminima}, // C# (Do diesis / Re bemolle) crea tensione
    {c3, time_minima}
};

// --- 5. POWERUP SPAWN (Salita rapida "magica") ---
NOTE SFX_POWERUP[] = {
    {g3, time_biscroma},
    {c4, time_biscroma},
    {e4, time_biscroma},
    {g4, time_biscroma},
    {c5, time_croma}
};

// --- 6. MALUS SPAWN (Buzz basso e fastidioso) ---
NOTE SFX_MALUS[] = {
    {a2b, time_semicroma}, // Nota molto bassa
    {pause, time_biscroma},
    {a2b, time_semicroma}
};

const int BGM_SIZE = sizeof(BGM_TETRIS) / sizeof(NOTE);
const int SFX_PLACE_SIZE = sizeof(SFX_PLACE) / sizeof(NOTE);
const int SFX_LINE_SIZE = sizeof(SFX_LINE) / sizeof(NOTE);
const int SFX_TETRIS_SIZE = sizeof(SFX_TETRIS) / sizeof(NOTE);
const int SFX_GAMEOVER_SIZE = sizeof(SFX_GAMEOVER) / sizeof(NOTE);
const int SFX_MALUS_SIZE = sizeof(SFX_MALUS) / sizeof(NOTE);