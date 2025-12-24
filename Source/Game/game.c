#include "game.h"
#include "graphics.h"
uint8_t gameField[20][10];
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
		//Timer used for srand seed
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
		
		drawGameField();
		
		//Used as game clock
		init_timer(1, 0x017D7840);
		enable_timer(1);

}

void genRandomTetromino()
{
    int i, j;

    uint8_t randomTetromino = 1 + rand() % 7;                                      // 0 is empty tetromino
    memcpy(newTetromino, TETROMINOS[randomTetromino].cells, sizeof(newTetromino)); // Copying the content of the newly generated  tetromino
		newTetrominoXpos = 3;
		newTetrominoYpos = 0;
}

void movePieceDown(){
	newTetrominoYpos += 1;
	uint8_t contact = checkContact();
	if(contact){
		//Fix it on the gaming board and spawn a new block
		lockTetromino();
	}
	updateUI(newTetrominoXpos, newTetrominoYpos);
}

uint8_t checkContact(){
	uint8_t i, j, contact = 0;
	
	for(i = 0; i < 4 && !contact; i++){
		for(j = 0; j < 4 && !contact; j++){
			if(newTetromino[i][j]){
				//I check for contact only if there is a block in the current position
				if(newTetrominoYpos+i+1 >= 20){
					//Reached the bottom
					contact = 1;
				}else if(gameField[newTetrominoYpos + i + 1][newTetrominoXpos + j]){
					//Touching another element
					contact = 1;
			}
		}
	}
}

	return contact;
}

void lockTetromino(){
	uint8_t i, j;
	
	//Fix it on board
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if(newTetromino[i][j]){
				gameField[newTetrominoYpos+i][newTetrominoXpos+j] = newTetromino[i][j];
			}
		}
	}
		
	//check and update points -> TODO
	//Generate a new tetromino
	genRandomTetromino();
	
	updateUI(newTetrominoXpos, newTetrominoYpos);
}