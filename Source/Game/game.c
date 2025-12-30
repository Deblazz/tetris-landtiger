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
		
    // Init the whole 2d array to 0
    memset(gameField, 0, sizeof(gameField));
    topScore = 0;
    currentScore = 0;
    clearedRows = 0;
		gameStatus = 2;		
    initUI();

		while(gameStatus == 2){
			__WFI(); // "Wait For Interrupt"
		}
		
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
	
		//For joystick polling, 50ms
		init_timer(2, 0x000927C0);
		enable_timer(2);
		
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
	
	if(checkContact()){
		//Fix it on the gaming board and spawn a new block
		lockTetromino();
	}else{
		clearTetromino(newTetrominoXpos, newTetrominoYpos);
		newTetrominoYpos += 1;
		drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
	}
}

uint8_t checkContact(){
if (isPositionValid(newTetrominoXpos, newTetrominoYpos + 1, newTetromino)) {
        return 0;
    } else {
        return 1;
    }
}

void lockTetromino(){
	uint8_t i, j;
	disable_timer(1);
	disable_timer(2);
	
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
	
	reset_timer(1);
	enable_timer(1);
	enable_timer(2);
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
	if(isPositionValid(newTetrominoXpos, newTetrominoYpos, TETROMINOS[newTetrominoType].cells[nextRotation])){
		clearTetromino(newTetrominoXpos, newTetrominoYpos);
    newTetrominoRotation = nextRotation;
    memcpy(newTetromino, TETROMINOS[newTetrominoType].cells[newTetrominoRotation], sizeof(newTetromino));
    drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
  }
}

void moveRight(){
	uint8_t nextTetrominoXPos = newTetrominoXpos+1;
	if (isPositionValid(nextTetrominoXPos, newTetrominoYpos, newTetromino)){
		clearTetromino(newTetrominoXpos, newTetrominoYpos);
    newTetrominoXpos = nextTetrominoXPos;
    drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
  }
}

void moveLeft(){
	uint8_t nextTetrominoXPos = newTetrominoXpos-1;
	if (isPositionValid(nextTetrominoXPos, newTetrominoYpos, newTetromino)){
		clearTetromino(newTetrominoXpos, newTetrominoYpos);
    newTetrominoXpos = nextTetrominoXPos;
    drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
  }
}

void pauseGame(){
	disable_timer(2);
	disable_timer(1);
	drawPausedGame();
}

void unpauseGame(){
	updateUI(newTetrominoXpos, newTetrominoYpos);
	enable_timer(2);
	enable_timer(1);
}

void dropTetromino(){
	disable_timer(1);
  disable_timer(2);
	clearTetromino(newTetrominoXpos, newTetrominoYpos);
	while(!checkContact()){	
		newTetrominoYpos += 1;
	}
	drawNewTetromino(newTetrominoXpos, newTetrominoYpos);
  lockTetromino();
}

uint8_t isPositionValid(uint8_t newTetrominoXpos, uint8_t newTetrominoYpos, const uint8_t newTetromino[4][4]){
	int i, j;
	for(i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			if(newTetromino[i][j] > 0){
				uint8_t absX = newTetrominoXpos + j;
				uint8_t absY = newTetrominoYpos + i;
				
				if(absX < 0 || absX >= 10 || absY < 0) return 0;
				
				if(absY >=20) return 0;
				
				if(gameField[absY][absX] > 0) return 0;
			}
		}
	}
	return 1;	
}

