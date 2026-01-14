#include "graphics.h"
#include "Game/game.h"

void drawTetrominoShape(uint16_t x, uint16_t y, TetrominoType type,
                        uint8_t rotation) {
  const Tetromino *tetromino = &TETROMINOS[type];
  uint16_t block_size = BLOCK_PIECE_WIDTH; // Size of each block in pixels
  uint16_t i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (tetromino->cells[rotation][i][j]) {
        // Draw the block at the calculated position
        drawTetrominoSprite(x + j * block_size, y + i * block_size,
                            tetromino->color);
      }
    }
  }
}

// Used to draw dinamically on top of gameField
void drawNewTetromino(uint16_t x, uint16_t y) {
  uint8_t i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (newTetromino[i][j]) {
        // Draw the block at the calculated position
        drawTetrominoSprite(2 + (x + j) * BLOCK_PIECE_WIDTH,
                            18 + (y + i) * BLOCK_PIECE_HEIGHT,
                            TETROMINOS[newTetromino[i][j]].color);
      }
    }
  }
}

void drawTetrominoSprite(uint16_t x, uint16_t y, uint16_t color_filter) {
  uint32_t index = 0;
  uint16_t i, j;
  uint16_t r_tint = (color_filter >> 11) & 0x1F; // range 0-31
  uint16_t g_tint = (color_filter >> 5) & 0x3F;  // range 0-63
  uint16_t b_tint = (color_filter) & 0x1F;       // range 0-31

  // Iterate first on j because of RowMajor in the sprite
  for (j = 0; j < BLOCK_PIECE_HEIGHT; j++) {
    for (i = 0; i < BLOCK_PIECE_WIDTH; i++) {
      uint8_t low = BLOCK_PIECE_PIXEL_DATA[index];
      uint8_t high = BLOCK_PIECE_PIXEL_DATA[index + 1];
      uint16_t src_pixel = (high << 8) | low;

      uint16_t intensity = (src_pixel >> 5) & 0x3F;

      uint16_t r_new = (r_tint * intensity) / 63;
      uint16_t g_new = (g_tint * intensity) / 63;
      uint16_t b_new = (b_tint * intensity) / 63;

      uint16_t final_color = (r_new << 11) | (g_new << 5) | b_new;

      LCD_SetPoint(x + i, y + j, final_color);

      index += 2;
    }
  }
}

void drawSprite(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                uint8_t *pixel_set) {
  uint32_t index = 0;
  uint16_t color;
  uint16_t i, j;

  for (j = 0; j < h; j++) {
    for (i = 0; i < w; i++) {
      // Composing pixel color using 2 near values
      color = (pixel_set[index + 1] << 8) | pixel_set[index];

      // Drawing the pixel
      LCD_SetPoint(x + i, y + j, color);

      index += 2;
    }
  }
}

void initUI() {
  LCD_Clear(Black);
  drawInfoUI();
  drawSprite(160, 200, 80, 85, (uint8_t *)saintbasil.pixel_data);
  drawStartGameMessage();
}

void updateUI(uint8_t Xpos, uint8_t Ypos) {
  drawGameField();
  drawNewTetromino(Xpos, Ypos);
}

void clearTetromino(uint8_t newTetrominoXpos, uint8_t newTetrominoYpos) {
  uint8_t i, j;

  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (newTetromino[i][j]) {

        uint16_t px = 2 + (newTetrominoXpos + j) * BLOCK_PIECE_WIDTH;
        uint16_t py = 18 + (newTetrominoYpos + i) * BLOCK_PIECE_HEIGHT;

        LCD_FillRect(px, py, BLOCK_PIECE_WIDTH, BLOCK_PIECE_HEIGHT, Black);
      }
    }
  }
}

/*
 this function manages to draw the whole board (maluses included) using the TETROMINOS array. The powerups are managed separately for sprite drawing reasons
*/
void drawGameField() {
  int i, j;

  for (i = 0; i < 20; i++) {
    for (j = 0; j < 10; j++) {
			
			switch(gameField[i][j]){
				case 0:
					LCD_FillRect(2 + j * BLOCK_PIECE_WIDTH, 18 + i * BLOCK_PIECE_HEIGHT, BLOCK_PIECE_WIDTH, BLOCK_PIECE_HEIGHT, Black);
				break;
				
				case 98:
					//Half line cleared powerup
					drawSprite(2 + j * BOMB_POWERUP_WIDTH, 18 + i * BOMB_POWERUP_HEIGHT, BOMB_POWERUP_WIDTH, BOMB_POWERUP_HEIGHT, (uint8_t *)BOMB_POWERUP_PIXEL_DATA);
					break;
				
				case 99:
					//Slow down powerup
					drawSprite(2 + j * CLOCK_POWERUP_WIDTH, 18 + i * CLOCK_POWERUP_HEIGHT, CLOCK_POWERUP_WIDTH, CLOCK_POWERUP_HEIGHT, (uint8_t *)CLOCK_POWERUP_PIXEL_DATA);
					break;
				
				default:
					drawTetrominoSprite(2 + j * BLOCK_PIECE_WIDTH, 18 + i * BLOCK_PIECE_HEIGHT, TETROMINOS[gameField[i][j]].color);
				break;
			}
    }
  }
}

void drawEmptyGameField() {
  LCD_FillRect(2, 18, 10 * BLOCK_PIECE_WIDTH, 20 * BLOCK_PIECE_HEIGHT, Black);
}

void drawInfoUI() {
  char buffer[20];
  LCD_EmptyRect(0, 16, 154, 304, White,
                2);                     // Adapted sizes to address white border
  LCD_FillRect(0, 0, 240, 16, Blue_UI); // Drawing Top Bar
  LCD_FillRect(154, 16, 86, 304, Blue_UI); // Drawing Side Score Bar

  GUI_Text(53, 0, (uint8_t *)"TETRIS", White, Blue_UI);
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

void drawStartGameMessage() {

  GUI_Text(30, 120, (uint8_t *)"Press KEY1", White, Black);
  GUI_Text(30, 140, (uint8_t *)"to start game", White, Black);
}

void drawUpdateScore() {
  char buffer[20];
  sprintf(buffer, "%lu", (unsigned long)currentScore);
  GUI_Text(160, 100, (uint8_t *)buffer, White, Blue_UI);
}

void drawPausedGame() {
  GUI_Text(30, 100, (uint8_t *)"GAME PAUSED", White, Black);
  GUI_Text(30, 120, (uint8_t *)"Press KEY1", White, Black);
  GUI_Text(30, 140, (uint8_t *)"to play", White, Black);
}

void drawEndGame() {
  char buffer[20];
  LCD_Clear(Blue_UI);
  LCD_EmptyRect(25, 25, 180, 260, Cyan, 2);
  drawSprite(160, 220, 80, 85, (uint8_t *)saintbasil.pixel_data);
  GUI_Text(85, 50, (uint8_t *)"YOU LOST", White, Blue_UI);
  GUI_Text(75, 70, (uint8_t *)"Your score:", White, Blue_UI);

  GUI_Text(100, 95, (uint8_t *)"TOP", White, Blue_UI);
  sprintf(buffer, "%lu", (unsigned long)topScore);
  GUI_Text(102, 115, (uint8_t *)buffer, White, Blue_UI);

  GUI_Text(90, 135, (uint8_t *)"SCORE", White, Blue_UI);
  sprintf(buffer, "%lu", (unsigned long)currentScore);
  GUI_Text(102, 155, (uint8_t *)buffer, White, Blue_UI);

  GUI_Text(90, 175, (uint8_t *)"LINES", White, Blue_UI);
  sprintf(buffer, "%lu", (unsigned long)clearedRows);
  GUI_Text(102, 195, (uint8_t *)buffer, White, Blue_UI);

  GUI_Text(40, 240, (uint8_t *)"Press KEY1 to", White, Blue_UI);
  GUI_Text(40, 260, (uint8_t *)"start over", White, Blue_UI);
}

void updateScores() {
  char buffer[20];
  sprintf(buffer, "%lu", (unsigned long)topScore);
  GUI_Text(160, 40, (uint8_t *)buffer, White, Blue_UI);

  sprintf(buffer, "%lu", (unsigned long)currentScore);
  GUI_Text(160, 100, (uint8_t *)buffer, White, Blue_UI);

  sprintf(buffer, "%lu", (unsigned long)clearedRows);
  GUI_Text(160, 160, (uint8_t *)buffer, White, Blue_UI);
}