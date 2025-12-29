#include "game.h"
#include "graphics.h"
#include "RIT/RIT.h"
uint8_t gameField[20][10];
uint8_t newTetromino[4][4];
uint8_t newTetrominoXpos;
uint8_t newTetrominoYpos;
uint8_t newTetrominoRotation;
uint8_t newTetrominoType;
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
		
		//For joystick polling, 50ms
		init_RIT(0x004C4B40);
		enable_RIT();
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
		
		
		//drawGameField(); Instead of redrawing the whole playing field i can simply "write out" the start message
		LCD_FillRect(30, 100, 110, 150, Black);
		
		//Used as game clock
		init_timer(1, 0x017D7840);
		enable_timer(1);
	
		genRandomTetromino();
		updateUI(newTetrominoXpos, newTetrominoYpos);
}

void genRandomTetromino()
{
    int i, j;
		newTetrominoRotation = 0;
    newTetrominoType = 1 + rand() % 7;                                      // 0 is empty tetromino
    memcpy(newTetromino, TETROMINOS[newTetrominoType].cells[0], sizeof(newTetromino)); // Copying the content of the newly generated  tetromino
		newTetrominoXpos = 3;
		newTetrominoYpos = 0;
}

void movePieceDown(){
	clearTetromino(newTetrominoXpos, newTetrominoYpos);
	
	newTetrominoYpos += 1;
	
	uint8_t contact = checkContact();
	if(contact){
		//Fix it on the gaming board and spawn a new block
		drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
		lockTetromino();
	}else{
		drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
	}
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
	//Raise score
	currentScore+=10;
	drawUpdateScore();
	//Fix it on board
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if(newTetromino[i][j]){
				gameField[newTetrominoYpos+i][newTetrominoXpos+j] = newTetromino[i][j];
			}
		}
	}
	
	//First lose condition, the piece sits perfectly on top with no more space
	if (newTetrominoYpos <= 0) { 
        endGame();
        return;
    }
		
	//check and update points -> TODO
	//Generate a new tetromino
	genRandomTetromino();
	
	updateUI(newTetrominoXpos, newTetrominoYpos);
	
	//Second lose condition, the piece touches as soon as spawn
	if (checkContact()) {
		endGame();
	}
}

void endGame(){
	gameStatus=2; //LOST
	disable_timer(1);
	LCD_Clear(Black);
	GUI_Text(30, 100, (uint8_t *)"YOU LOST", White, Black);
}


void rotateTetromino(){
	uint8_t nextRotation = newTetrominoRotation + 1;
	if(nextRotation > 3) nextRotation = 0;
	clearTetromino(newTetrominoXpos, newTetrominoYpos);
  memcpy(newTetromino, TETROMINOS[newTetrominoType].cells[nextRotation], sizeof(newTetromino));
  newTetrominoRotation = nextRotation;
  drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
}

void moveRight(){
	uint8_t nextTetrominoXPos = newTetrominoXpos+1;
	if(nextTetrominoXPos == 20){nextTetrominoXPos = 19;}
	clearTetromino(newTetrominoXpos, newTetrominoYpos);
	memcpy(newTetromino, TETROMINOS[newTetrominoType].cells[newTetrominoRotation], sizeof(newTetromino));
	newTetrominoXpos = nextTetrominoXPos;
  drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
}

void moveLeft(){
	uint8_t nextTetrominoXPos = newTetrominoXpos-1;
	if(nextTetrominoXPos <= 0){nextTetrominoXPos = 0;}
	clearTetromino(newTetrominoXpos, newTetrominoYpos);
	memcpy(newTetromino, TETROMINOS[newTetrominoType].cells[newTetrominoRotation], sizeof(newTetromino));
	newTetrominoXpos = nextTetrominoXPos;
  drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
}
