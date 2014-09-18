#include "snake.h"
#include "vector.h"
#include "random.h"

using namespace std;

// Constants
const int DELAY = 100000;
const int DELAY_OFFSET = 20000; // to increase the speed every xxx score interval
const int SCORE_INCREMENTER = 20;
const int SNAKE_START_SIZE = 5;
const int GAME_OVER_DELAY = 3;
const char SNAKE_BODY_CHAR = 'X';
const int BORDER_CHAR = char(128);
const char FOOD_CHAR = '*';

// Stucture Constructors
SnakeComponent::SnakeComponent(int yc, int xc) { // x and y switched to match ncurses' craziness
	y = yc;
	x = xc;
}

SnakeComponent::SnakeComponent() {
	y = 0;
	x = 0;
}

// Class Constructor/Destructor
Snake::Snake() {
	initscr();
	noecho();
	curs_set(0);
	nodelay(stdscr, true);
	keypad(stdscr, true);
	getmaxyx(stdscr, maxHeight, maxWidth);
	
	delayInMicroseconds = DELAY;

	score = 0;
	currentHeading = 'l';
	snakeBodyChar = SNAKE_BODY_CHAR;
	borderChar = BORDER_CHAR;
	foodChar = FOOD_CHAR;
	Vector<SnakeComponent> snakeBody;
	SnakeComponent food;
	bool didEatFoodFlag = false;

	drawBoard();
	drawScore();
	placeSnake();
	placeFood();
	refresh();	
}

Snake::~Snake() {
    nodelay(stdscr,false); 
    getch();
    endwin();
}

// Class Methods
void Snake::start() {
	while (true) {
		if(iCanHazDeadlyCollision()) {
			gameOver();
			break;
		} else {
			moveSnakeBody();
			usleep(delayInMicroseconds);
			if (currentHeading == 'q') {
				break;
			}
		}
	}
}

void Snake::drawBoard() {
	// draw top border
	for (int i = 0; i < maxWidth - 1; i++) {
		move(0, i);
		addch(borderChar);
	}
	// draw bottom border
	for (int i = 0; i < maxWidth - 1; i++) {
		move(maxHeight - 2, i);
		addch(borderChar);
	}
	// draw left border
	for (int i = 0; i < maxHeight - 1; i++) {
		move(i, 0);
		addch(borderChar);
	}
	// draw right border
	for (int i = 0; i < maxHeight - 1; i++) {
		move(i, maxWidth - 1);
		addch(borderChar);
	}
}

void Snake::drawScore() {
	move(maxHeight - 1, 0);
	printw("%d", score);
}

void Snake::placeFood() {
	int tempX = 0;
	int tempY = 0;
	char tester = snakeBodyChar;
	while (tester == snakeBodyChar) {
		tempX = randomInteger(1, maxWidth - 2);
		tempY = randomInteger(1, maxHeight - 3);
		tester = mvinch(tempY, tempX);
	}
	food.y = tempY;
	food.x = tempX;
	move(food.y, food.x);
	addch(foodChar);
}

void Snake::placeSnake() {
	for (int i = 0; i < SNAKE_START_SIZE; i++) {
		SnakeComponent segment(15, 40 + i);
		snakeBody.add(segment);
		move(snakeBody[i].y, snakeBody[i].x);
		addch(snakeBodyChar);
	}
}

void Snake::checkForFood() {
	char charAtLocation = mvinch(snakeBody[0].y, snakeBody[0].x);
	if (charAtLocation == foodChar) {
		didEatFoodFlag = true;
		updateScore();
		placeFood();
	}
}

bool Snake::iCanHazDeadlyCollision() {
	char charAtLocation = mvinch(snakeBody[0].y, snakeBody[0].x);
	if (charAtLocation == borderChar) {
		return true;
	} else if (charAtLocation == snakeBodyChar) {
		for (int i = 2; i < snakeBody.size(); i++) {
               if(snakeBody[0].x == snakeBody[i].x && snakeBody[0].y == snakeBody[i].y) {
               		return true;
               }
        }
	}
}

void Snake::updateScore() {
	score += SCORE_INCREMENTER;
	if (score % 100 == 0) {
		delayInMicroseconds -= DELAY_OFFSET;
	}
	drawScore();
}

void Snake::gameOver() {
	string gameOver = "GAME OVER";
	move((maxHeight - 3) / 2, (maxWidth - gameOver.length()) / 2);
	printw("GAME OVER");
	refresh();
	sleep(GAME_OVER_DELAY);
}

void Snake::detectKeyFromPlayer() {
	int input = getch();
	switch (input) {
		case KEY_LEFT:
			if (currentHeading != 'r') {
				currentHeading = 'l';
			}
			break;
		case KEY_RIGHT:
			if (currentHeading != 'l') {
				currentHeading = 'r';
			}
			break;
		case KEY_UP:
			if (currentHeading != 'd') {
				currentHeading = 'u';
			}
			break;
		case KEY_DOWN:
			if (currentHeading != 'u') {
				currentHeading = 'd';
			}
			break;
		case KEY_BACKSPACE:
			currentHeading = 'q';
			break;
	}
}

void Snake::moveSnakeBody() {
	

	detectKeyFromPlayer();
	int tempY = snakeBody[0].y;
	int tempX = snakeBody[0].x;
	if (currentHeading == 'r') {
		snakeBody.insert(0, SnakeComponent(tempY, tempX + 1));
	} else if (currentHeading == 'l') {
		snakeBody.insert(0, SnakeComponent(tempY, tempX - 1));
	} else if (currentHeading == 'u') {
		snakeBody.insert(0, SnakeComponent(tempY - 1, tempX));
	} else if (currentHeading == 'd') {
		snakeBody.insert(0, SnakeComponent(tempY + 1, tempX));
	} 

	checkForFood();
	if (!didEatFoodFlag) {
		move(snakeBody[snakeBody.size() - 1].y, snakeBody[snakeBody.size() - 1].x);
        printw(" ");
        refresh();
        snakeBody.remove(snakeBody.size() - 1);
	}

	if (!iCanHazDeadlyCollision()) {
		move(snakeBody[0].y, snakeBody[0].x);
		addch(snakeBodyChar);
		didEatFoodFlag = false;
		refresh();
	} else {
		refresh();
	}


	
}



// Free Functions

