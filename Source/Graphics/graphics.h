#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "GLCD.h"
#include <stdint.h>
#include <stdio.h>

/*Include every asset to draw here*/
#include "Sprites/block_piece.h"
#include "Sprites/saintbasil.h"
typedef enum {
  TETROMINO_EMPTY = 0,
  TETROMINO_O = 1,
  TETROMINO_I = 2,
  TETROMINO_T = 3,
  TETROMINO_L = 4,
  TETROMINO_J = 5,
  TETROMINO_S = 6,
  TETROMINO_Z = 7,

  NUM_TETROMINOS
} TetrominoType;

typedef struct {
  uint8_t cells[4][4][4];
  uint16_t color;
} Tetromino;

static const Tetromino TETROMINOS[9] = {
    // Empty
    {
        {{{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
        Black // Color associated based on shape
    },
    // Yellow (O) - No Rotation
    {
        {{{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
         {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
        Yellow // Color associated based on shape
    },
    // Cyan (I)
    {
        {
            {{2, 2, 2, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 0 deg
            {{2, 0, 0, 0}, {2, 0, 0, 0}, {2, 0, 0, 0}, {2, 0, 0, 0}}, // 90 deg
            {{2, 2, 2, 2}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 180 deg
            {{2, 0, 0, 0}, {2, 0, 0, 0}, {2, 0, 0, 0}, {2, 0, 0, 0}}  // 270 deg
        },
        Cyan // Color associated based on shape
    },
    // Magenta (T)
    {
        {
            {{0, 3, 0, 0}, {3, 3, 3, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 0 deg
                                                                      // (Up)
            {{3, 0, 0, 0}, {3, 3, 0, 0}, {3, 0, 0, 0}, {0, 0, 0, 0}}, // 90 deg
                                                                      // (Right)
            {{3, 3, 3, 0}, {0, 3, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 180 deg
                                                                      // (Down)
            {{0, 3, 0, 0}, {3, 3, 0, 0}, {0, 3, 0, 0}, {0, 0, 0, 0}}  // 270 deg
                                                                      // (Left)
        },
        Magenta // Color associated based on shape
    },
    // Orange (J)
    {
        {
            {{0, 0, 4, 0}, {4, 4, 4, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 0 deg
            {{4, 0, 0, 0}, {4, 0, 0, 0}, {4, 4, 0, 0}, {0, 0, 0, 0}}, // 90 deg
            {{4, 4, 4, 0}, {4, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 180 deg
            {{4, 4, 0, 0}, {0, 4, 0, 0}, {0, 4, 0, 0}, {0, 0, 0, 0}}  // 270 deg
        },
        Orange // Color associated based on shape
    },
    // Blue (L)
    {
        {
            {{5, 0, 0, 0}, {5, 5, 5, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 0 deg
            {{5, 5, 0, 0}, {5, 0, 0, 0}, {5, 0, 0, 0}, {0, 0, 0, 0}}, // 90 deg
            {{5, 5, 5, 0}, {0, 0, 5, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 180 deg
            {{0, 5, 0, 0}, {0, 5, 0, 0}, {5, 5, 0, 0}, {0, 0, 0, 0}}  // 270 deg
        },
        Blue // Color associated based on shape
    },
    // Green (S)
    {
        {
            {{0, 6, 6, 0}, {6, 6, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 0 deg
            {{6, 0, 0, 0}, {6, 6, 0, 0}, {0, 6, 0, 0}, {0, 0, 0, 0}}, // 90 deg
            {{0, 6, 6, 0}, {6, 6, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 180 deg
            {{6, 0, 0, 0}, {6, 6, 0, 0}, {0, 6, 0, 0}, {0, 0, 0, 0}}  // 270 deg
        },
        Green // Color associated based on shape
    },
    // Red (Z)
    {
        {
            {{7, 7, 0, 0}, {0, 7, 7, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 0 deg
            {{0, 0, 7, 0}, {0, 7, 7, 0}, {0, 7, 0, 0}, {0, 0, 0, 0}}, // 90 deg
            {{7, 7, 0, 0}, {0, 7, 7, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, // 180 deg
            {{0, 7, 0, 0}, {7, 7, 0, 0}, {7, 0, 0, 0}, {0, 0, 0, 0}}  // 270 deg
        },
        Red // Color associated based on shape
				},// Grey, used for compatibility w/malus -> Not a real tetromino
    {
        {{}
        },
        Malus_Grey
    }};
void drawTetrominoSprite(uint16_t x, uint16_t y, uint16_t color_filter);
void drawSprite(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                uint8_t *pixel_set);
void drawTetrominoShape(uint16_t x, uint16_t y, TetrominoType type,
                        uint8_t rotation);
void clearTetromino(uint8_t newTetrominoXpos, uint8_t newTetrominoYpos);
void initUI();
void drawUpdateScore();
void updateUI(uint8_t Xpos, uint8_t Ypos);
void drawGameField();
void drawEmptyGameField();
void drawInfoUI();
void drawStartGameMessage();
void drawPausedGame();
void updateScore();
#endif