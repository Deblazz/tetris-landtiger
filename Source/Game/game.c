#include "game.h"
#include "graphics.h"
uint8_t gameField[10][20];
uint8_t newTetromino[4][4];
uint8_t newTetrominoXpos;
uint8_t newTetrominoYpos;
// Probably can use 16, 16, 8 bits. Using this setup to address top players.
uint32_t topScore;
uint32_t currentScore;
uint16_t clearedRows;
volatile GameStatus gameStatus;

void initGame()
{
		uint32_t ticksTaken;
		init_timer(0, 0xFFFFFFFF); //Counts how long until unpause
		enable_timer(0);
	
    // Init the whole 2d array to 0
    memset(gameField, 0, sizeof(gameField));
    topScore = 0;
    currentScore = 0;
    clearedRows = 0;
		gameStatus = 0;		
    initUI();
	
		while(!gameStatus){/*Do nothing*/};
		
		disable_timer(0);
		//Gamestatus is Play
		ticksTaken = get_timer_value(0); //Get time taken so that I can init the random seed generator
		reset_timer(0); //I free the timer
		srand(ticksTaken); //Init seed
}

void genRandomTetromino()
{
    int i, j;

    uint8_t randomTetromino = 1 + rand() % 7;                                      // 0 is empty tetromino
    memcpy(newTetromino, TETROMINOS[randomTetromino].cells, sizeof(newTetromino)); // Copying the content of the newly generated  tetromino
		newTetrominoXpos = 3;
		newTetrominoYpos = 0;
}
