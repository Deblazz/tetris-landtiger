#include "graphics.h"
#include "game.h"
/*Function used to draw a given tetramino at a given position. Wrapper of drawTetrominoShape*/
void drawTetromino(uint16_t x, uint16_t y, uint8_t tetromino_num)
{

    switch (tetromino_num)
    {
    case 0:
        drawTetrominoShape(x, y, 0, 0);
        break;
    case 1:
        drawTetrominoShape(x, y, 1, 0);
        break;
    case 2:
        drawTetrominoShape(x, y, 2, 0);
        break;
    case 3:
        drawTetrominoShape(x, y, 3, 0);
        break;
    case 4:
        drawTetrominoShape(x, y, 4, 0);
        break;
    case 5:
        drawTetrominoShape(x, y, 5, 0);
        break;
    case 6:
        drawTetrominoShape(x, y, 6, 0);
        break;
    }
}


void drawTetrominoShape(uint16_t x, uint16_t y, TetrominoType type, uint8_t rotation){
    const Tetromino* tetromino = &TETROMINOS[type];
    uint16_t block_size = BLOCK_PIECE_WIDTH; // Size of each block in pixels
    uint16_t i, j;

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (tetromino->cells[i][j]) {
                // Draw the block at the calculated position
                            drawTetrominoSprite(x + j * block_size, y + i * block_size, tetromino->color);
            }
        }
    }
}

// Used to draw dinamically on top of gameField
void drawNewTetromino(uint16_t x, uint16_t y)
{
    uint8_t i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (newTetromino[i][j])
            {
                // Draw the block at the calculated position
                drawTetrominoSprite(2 + (x + j) * BLOCK_PIECE_WIDTH, 18 + (y + i) * BLOCK_PIECE_HEIGHT, TETROMINOS[newTetromino[i][j]].color);
            }
        }
    }
}

void drawTetrominoSprite(uint16_t x, uint16_t y, uint16_t color_filter)
{
    uint32_t index = 0;
    uint16_t i, j;
    uint16_t r_tint = (color_filter >> 11) & 0x1F; // range 0-31
    uint16_t g_tint = (color_filter >> 5) & 0x3F;  // range 0-63
    uint16_t b_tint = (color_filter) & 0x1F;       // range 0-31

    // Iterate first on j because of RowMajor in the sprite
    for (j = 0; j < BLOCK_PIECE_HEIGHT; j++)
    {
        for (i = 0; i < BLOCK_PIECE_WIDTH; i++)
        {
            // 2. Leggiamo il pixel dallo sprite ORIGINALE (Giallo/Vetro)
            uint8_t low = BLOCK_PIECE_PIXEL_DATA[index];
            uint8_t high = BLOCK_PIECE_PIXEL_DATA[index + 1];
            uint16_t src_pixel = (high << 8) | low;

            // 3. Calcoliamo la LUMINOSIT� del pixel originale.
            // Il trucco: siccome il tuo sprite � giallo, usiamo il canale VERDE come riferimento di luminosit�.
            // Il verde in RGB565 ha 6 bit (0-63), quindi � il pi� preciso.
            uint16_t intensity = (src_pixel >> 5) & 0x3F; // Valore 0 (scuro) -> 63 (massima luce)

            // 4. CALCOLO MATEMATICO (TINTING)
            // Nuova componente = (ComponenteColore * Intensit�Pixel) / MaxIntensit�
            // Questo preserva le ombre!

            // Calcolo Rosso
            uint16_t r_new = (r_tint * intensity) / 63;
            // Calcolo Verde
            uint16_t g_new = (g_tint * intensity) / 63;
            // Calcolo Blu
            uint16_t b_new = (b_tint * intensity) / 63;

            // 5. Ricomponiamo il pixel
            uint16_t final_color = (r_new << 11) | (g_new << 5) | b_new;

            LCD_SetPoint(x + i, y + j, final_color);

            index += 2;
        }
    }
}

void drawSprite(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t *pixel_set)
{
    uint32_t index = 0;
    uint16_t color;
    uint16_t i, j;

    for (j = 0; j < h; j++)
    {
        for (i = 0; i < w; i++)
        {
            // Composing pixel color using 2 near values
            color = (pixel_set[index + 1] << 8) | pixel_set[index];

            // Drawing the pixel
            LCD_SetPoint(x + i, y + j, color);

            index += 2;
        }
    }
}

void initUI()
{
    drawInfoUI();
    drawGameField();
		drawStartGameMessage();
}

void updateUI(uint8_t Xpos, uint8_t Ypos)
{
    drawGameField();
    drawNewTetromino(Xpos, Ypos);
}

void drawGameField()
{
    int i, j;

    for (i = 0; i < 20; i++)
    {
        for (j = 0; j < 10; j++)
        {
            if (gameField[i][j])
            {
                drawTetrominoSprite(2 + j * BLOCK_PIECE_WIDTH, 18 + i * BLOCK_PIECE_HEIGHT, TETROMINOS[gameField[i][j]].color);
            }
            else
            {
                LCD_FillRect(2 + j * BLOCK_PIECE_WIDTH, 18 + i * BLOCK_PIECE_HEIGHT, BLOCK_PIECE_WIDTH, BLOCK_PIECE_HEIGHT, Black);
            }
        }
    }
}

void drawInfoUI()
{
    char buffer[20];
    LCD_EmptyRect(0, 16, 154, 304, White, 2); // Adapted sizes to address white border
    LCD_FillRect(0, 0, 240, 16, Blue_UI);     // Drawing Top Bar
    LCD_FillRect(154, 16, 86, 304, Blue_UI);  // Drawing Side Score Bar

    GUI_Text(160, 20, (uint8_t *)"TOP", White, Blue_UI);
    sprintf(buffer, "%lu", (unsigned long)topScore);
    GUI_Text(160, 40, (uint8_t *)buffer, White, Blue_UI);

    GUI_Text(160, 80, (uint8_t *)"SCORE", White, Blue_UI);
    sprintf(buffer, "%lu", (unsigned long)currentScore);
    GUI_Text(160, 100, (uint8_t *)buffer, White, Blue_UI);

    GUI_Text(160, 140, (uint8_t *)"LINES", White, Blue_UI);
    sprintf(buffer, "%lu", (unsigned long)clearedRows);
    GUI_Text(160, 160, (uint8_t *)buffer, White, Blue_UI);
}

void drawStartGameMessage(){
	GUI_Text(30, 100, (uint8_t *)"TETRIS", White, Black);
	GUI_Text(30, 120, (uint8_t *)"Press KEY1", White, Black);
	GUI_Text(30, 140, (uint8_t *)"to start game", White, Black);
}