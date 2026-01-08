#include "game.h"
#include "Graphics/graphics.h"

uint8_t gameField[20][10];
uint8_t newTetromino[4][4];
int newTetrominoXpos;
int newTetrominoYpos;
uint8_t newTetrominoRotation;
uint8_t newTetrominoType;
// Probably can use 16, 16, 8 bits. Using this setup to address top players.
uint32_t topScore = 0;
uint32_t currentScore;
uint16_t clearedRows;
volatile GameStatus gameStatus;

void resetGame() {
  init_timer(0, 0xFFFFFFFF); // Counts how long until unpause
  reset_timer(0);
  enable_timer(0);

  init_timer(2, 0x000927C0); // 50ms
  reset_timer(2);
  enable_timer(2);

  memset(gameField, 0, sizeof(gameField));
  currentScore = 0;
  clearedRows = 0;

  initUI();
  gameStatus = 2;
}

void startGame() {
  uint32_t ticksTaken;
  ticksTaken = get_timer_value(
      0); // Get time taken so that I can init the random seed generator
  srand(ticksTaken); // Init seed

  // drawGameField(); Instead of redrawing the whole playing field i can simply
  // "write out" the start message
  LCD_FillRect(30, 100, 110, 150, Black);

  // Used as game clock
  init_timer(1, 0x017D7840);
  reset_timer(1);

  // For joystick polling, 50ms
  init_timer(2, 0x000927C0);
  reset_timer(2);
  genRandomTetromino();
  updateUI(newTetrominoXpos, newTetrominoYpos);

  gameStatus = 1;

  enable_timer(1);
  enable_timer(2);
}

void genRandomTetromino() {
  int i, j;
  newTetrominoRotation = 0;
  newTetrominoType = 1 + rand() % 7; // 0 is empty tetromino
  memcpy(newTetromino, TETROMINOS[newTetrominoType].cells[0],
         sizeof(newTetromino)); // Copying the content of the newly generated
                                // tetromino
  newTetrominoXpos = 3;
  newTetrominoYpos = 0;
}

void movePieceDown() {

  if (checkContact()) {
    // Fix it on the gaming board and spawn a new block
    lockTetromino();
  } else {
    clearTetromino(newTetrominoXpos, newTetrominoYpos);
    newTetrominoYpos += 1;
    drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
  }
}

uint8_t checkContact() {
  if (isPositionValid(newTetrominoXpos, newTetrominoYpos + 1, newTetromino)) {
    return 0;
  } else {
    return 1;
  }
}

void lockTetromino() {
  uint8_t i, j;
  disable_timer(1);
  disable_timer(2);

  // Raise score
  currentScore += 10;
  drawUpdateScore();
  // Fix it on board
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (newTetromino[i][j]) {
        gameField[newTetrominoYpos + i][newTetrominoXpos + j] =
            newTetromino[i][j];
      }
    }
  }

  checkAndClearRows();

  // First lose condition, the piece sits perfectly on top with no more space
  if (newTetrominoYpos <= 0) {
    endGame();
    return;
  }

  // check and update points -> TODO
  // Generate a new tetromino
  genRandomTetromino();

  updateUI(newTetrominoXpos, newTetrominoYpos);

  // Second lose condition, the piece touches as soon as spawn
  if (checkContact()) {
    endGame();
    return;
  }

  reset_timer(1);
  enable_timer(1);
  enable_timer(2);
}

void endGame() {
  disable_timer(1);
  disable_timer(2);
  gameStatus = 3;
  // Update topscore
  if (currentScore > topScore)
    topScore = currentScore;
  // Variables will be eventually initialized by initUI

  drawEndGame();
}

void rotateTetromino() {
  uint8_t nextRotation = newTetrominoRotation + 1;
  if (nextRotation > 3)
    nextRotation = 0;
  if (isPositionValid(newTetrominoXpos, newTetrominoYpos,
                      TETROMINOS[newTetrominoType].cells[nextRotation])) {
    clearTetromino(newTetrominoXpos, newTetrominoYpos);
    newTetrominoRotation = nextRotation;
    memcpy(newTetromino,
           TETROMINOS[newTetrominoType].cells[newTetrominoRotation],
           sizeof(newTetromino));
    drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
  }
}

void moveRight() {
  int nextTetrominoXPos = newTetrominoXpos + 1;
  if (isPositionValid(nextTetrominoXPos, newTetrominoYpos, newTetromino)) {
    clearTetromino(newTetrominoXpos, newTetrominoYpos);
    newTetrominoXpos = nextTetrominoXPos;
    drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
  }
}

void moveLeft() {
  int nextTetrominoXPos = newTetrominoXpos - 1;
  if (isPositionValid(nextTetrominoXPos, newTetrominoYpos, newTetromino)) {
    clearTetromino(newTetrominoXpos, newTetrominoYpos);
    newTetrominoXpos = nextTetrominoXPos;
    drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
  }
}

void pauseGame() {
  disable_timer(1);
  drawPausedGame();
}

void unpauseGame() {
  updateUI(newTetrominoXpos, newTetrominoYpos);
  enable_timer(1);
}

void dropTetromino() {
  disable_timer(1);
  disable_timer(2);
  clearTetromino(newTetrominoXpos, newTetrominoYpos);
  while (!checkContact()) {
    newTetrominoYpos += 1;
  }
  drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
  lockTetromino();
}

uint8_t isPositionValid(int newTetrominoXpos, int newTetrominoYpos,
                        const uint8_t newTetromino[4][4]) {
  int i, j;
  for (i = 0; i < 4; i++) {
    for (j = 0; j < 4; j++) {
      if (newTetromino[i][j] > 0) {
        int absX = newTetrominoXpos + j;
        int absY = newTetrominoYpos + i;

        if (absX < 0 || absX >= 10 || absY < 0)
          return 0;

        if (absY >= 20)
          return 0;

        if (gameField[absY][absX] > 0)
          return 0;
      }
    }
  }
  return 1;
}

void checkAndClearRows() {
  int i, j, k, rowsCleared = 0, rowIsFull;
  uint8_t fullRowIndices[4];

  for (i = 0; i < 20; i++) {
    rowIsFull = 1;
    for (j = 0; j < 10; j++) {
      if (gameField[i][j] == 0) {
        rowIsFull = 0;
        break;
      }
    }

    if (rowIsFull) {
      if (rowsCleared < 4)
        fullRowIndices[rowsCleared] = i;
      rowsCleared++;
    }
  }
  if (rowsCleared == 0)
    return; // No need to redraw anything

  if (rowsCleared == 4) {
    animateTetrisEffect(fullRowIndices);
  } else {
    // Show black lines
    for (k = 0; k < rowsCleared; k++) {
      LCD_FillRect(2, 18 + fullRowIndices[k] * BLOCK_PIECE_HEIGHT,
                   10 * BLOCK_PIECE_WIDTH, BLOCK_PIECE_HEIGHT, Black);
    }
  }

  // Shift down

  for (i = 19; i >= 0; i--) {
    rowIsFull = 1;
    for (j = 0; j < 10; j++) {
      if (gameField[i][j] == 0) {
        rowIsFull = 0;
        break;
      }
    }

    if (rowIsFull) {
      for (k = i; k > 0; k--) {
        for (j = 0; j < 10; j++) {
          gameField[k][j] = gameField[k - 1][j];
        }
      }

      for (j = 0; j < 10; j++) {
        gameField[0][j] = 0;
      }
      i++;
    }
  }
  if (rowsCleared == 4) {
    // tetris
    currentScore += 600;
  } else {
    currentScore += rowsCleared * 100;
  }

  clearedRows += rowsCleared;
  updateScores();
  drawGameField();
}

void animateTetrisEffect(uint8_t rows[]) {
  int flash;
  int r;
  uint16_t bgColor, textColor;

  uint16_t textY = 18 + (rows[1] * BLOCK_PIECE_HEIGHT);
  uint16_t textX = 55;

  for (flash = 0; flash < 8; flash++) { // Flash 4 times

    if (flash % 2 == 0) {
      bgColor = Yellow;
      textColor = Red;
    } else {
      bgColor = Black;
      textColor = Black;
    }

    for (r = 0; r < 4; r++) {
      LCD_FillRect(2, 18 + rows[r] * BLOCK_PIECE_HEIGHT, 10 * BLOCK_PIECE_WIDTH,
                   BLOCK_PIECE_HEIGHT, bgColor);
    }

    if (bgColor == Yellow) {
      GUI_Text(textX, textY, (uint8_t *)"TETRIS!", textColor, bgColor);
    }
    delay_ms(150);
  }
}

void delay_ms(uint32_t ms) {
  volatile uint32_t i, j;

  for (i = 0; i < ms; i++) {
    for (j = 0; j < 25000; j++) /*do nothing*/
      ;
  }
}