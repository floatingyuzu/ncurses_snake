#include <iostream>
#include "ncurses.h"
#include "vector.h"

#ifndef SNAKE_H
#define SNAKE_H

struct SnakeComponent {
	int y; 
	int x;
	SnakeComponent(int yc, int xc);
	SnakeComponent();
};

class Snake {

public:
	Snake();
	~Snake();
	void start();

private:
	int score;
	int delayInMicroseconds;
	int maxHeight;
	int maxWidth;

	char currentHeading;
	char snakeBodyChar;
	char borderChar;
	char foodChar;

	Vector<SnakeComponent> snakeBody;
	SnakeComponent food;
	bool didEatFoodFlag;
	
	// Member Functions
	void drawBoard();
	void drawScore();
	void placeFood();
	void placeSnake();
	void checkForFood();
	bool iCanHazDeadlyCollision();
	void updateScore();
	void gameOver();
	void detectKeyFromPlayer();
	void moveSnakeBody();


};


#endif 