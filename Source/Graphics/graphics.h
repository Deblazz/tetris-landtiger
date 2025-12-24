#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <stdint.h>
#include <stdio.h>
#include "GLCD.h"

/*Include every asset to draw here*/
#include "Sprites/block_piece.h"

typedef enum
{
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

typedef struct
{
    uint8_t cells[4][4];
    uint16_t color;
} Tetromino;

static const Tetromino TETROMINOS[8] = {
    {
        {{0, 0, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        Black // Color associated based on shape
    },
    {
        {{0, 1, 1, 0},
         {0, 1, 1, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        Yellow // Color associated based on shape
    },
    {
        {{2, 2, 2, 2},
         {0, 0, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        Cyan // Color associated based on shape
    },
    {
        {{0, 3, 0, 0},
         {3, 3, 3, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        Magenta // Color associated based on shape
    },
    {
        {{0, 0, 4, 0},
         {4, 4, 4, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        Orange // Color associated based on shape
    },
    {
        {{5, 0, 0, 0},
         {5, 5, 5, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        Blue // Color associated based on shape
    },
    {
        {{0, 6, 6, 0},
         {6, 6, 0, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        Green // Color associated based on shape
    },
    {
        {{7, 7, 0, 0},
         {0, 7, 7, 0},
         {0, 0, 0, 0},
         {0, 0, 0, 0}},
        Red // Color associated based on shape
    }};

void drawTetrominoSprite(uint16_t x, uint16_t y, uint16_t color_filter);
void drawTetromino(uint16_t x, uint16_t y, uint8_t tetramino_num);
void drawSprite(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *pixel_set);
void drawTetrominoShape(uint16_t x, uint16_t y, TetrominoType type, uint8_t rotation);
void initUI();
void updateUI(uint8_t Xpos, uint8_t Ypos);
void drawGameField();
void drawInfoUI();
void drawStartGameMessage();
#endif