#include "music/music.h"

// Background Music Tetris loop declaration
NOTE BGM_TETRIS[] = {
    {e4, time_semiminima}, {b3, time_croma},         {c4, time_croma},

    {d4, time_semiminima}, {c4, time_croma},         {b3, time_croma},

    {a3, time_semiminima}, {a3, time_croma},         {c4, time_croma},

    {e4, time_semiminima}, {d4, time_croma},         {c4, time_croma},

    {b3, time_semiminima}, {b3, time_croma},         {c4, time_croma},

    {d4, time_semiminima}, {e4, time_semiminima},

    {c4, time_semiminima}, {a3, time_semiminima},

    {a3, time_semiminima}, {pause, time_semiminima},

    {d4, time_semiminima}, {f4, time_croma},         {a4, time_semiminima},

    {g4, time_croma},      {f4, time_croma},

    {e4, time_semiminima}, {c4, time_croma},         {e4, time_semiminima},

    {d4, time_croma},      {c4, time_croma},

    {b3, time_semiminima}, {b3, time_croma},         {c4, time_croma},

    {d4, time_semiminima}, {e4, time_semiminima},

    {c4, time_semiminima}, {a3, time_semiminima},

    {a3, time_semiminima}, {pause, time_semiminima}};

// Block placed on grid sound effect
NOTE SFX_PLACE[] = {{e4, time_semicroma}, {pause, time_semibiscroma}};

// Line cleared sound effect
NOTE SFX_LINE[] = {
    {c4, time_semicroma}, {e4, time_semicroma}, {g4, time_semicroma}};

// Tetris sound effect
NOTE SFX_TETRIS[] = {{b4, time_biscroma},
                     {d4, time_biscroma},
                     {f4, time_biscroma},
                     {a4, time_croma}};

// Game over (ending screen) sound effect
NOTE SFX_GAMEOVER[] = {
    {d4, time_semiminima}, {c3b, time_semiminima}, {c3, time_minima}};

// Power up spawn sound effect
NOTE SFX_POWERUP[] = {{g3, time_biscroma},
                      {c4, time_biscroma},
                      {e4, time_biscroma},
                      {g4, time_biscroma},
                      {c5, time_croma}};

// Malus spawn sound effect
NOTE SFX_MALUS[] = {
    {a2b, time_semicroma}, {pause, time_biscroma}, {a2b, time_semicroma}};

const int BGM_SIZE = sizeof(BGM_TETRIS) / sizeof(NOTE);
const int SFX_PLACE_SIZE = sizeof(SFX_PLACE) / sizeof(NOTE);
const int SFX_LINE_SIZE = sizeof(SFX_LINE) / sizeof(NOTE);
const int SFX_TETRIS_SIZE = sizeof(SFX_TETRIS) / sizeof(NOTE);
const int SFX_GAMEOVER_SIZE = sizeof(SFX_GAMEOVER) / sizeof(NOTE);
const int SFX_POWERUP_SIZE = sizeof(SFX_POWERUP) / sizeof(NOTE);
const int SFX_MALUS_SIZE = sizeof(SFX_MALUS) / sizeof(NOTE);