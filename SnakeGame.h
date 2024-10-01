#pragma once

#ifdef _WIN32
#define _USE_MATH_DEFINES
#include<windows.h>
#else
#include<sys/time.h>
#include<unistd.h>
#endif

#ifdef __APPLE__
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif

#include<iostream>
#include<deque>
#include<vector>
#include<cmath>
#include<string>
#include<cstdlib>
#include<ctime>
#include<sstream>
#include<fstream>

// Defining Global Constants
const int window_width = 500, window_height = 500;
const int numx = 10, numy = 10;
const double dx = 2. / numx, dy = 2. / numx;
const double max_delay = 200000;

enum class Direction
{
	up, down, left, right
};

enum SnakeOrApple
{
	s = 0, // Snake
	a = 1, // Apple
	h = 2  // Head of snake
};

class SnakePos
{
private:
	std::deque<std::vector<int>> positions;
public:
	void reset()
	{
		positions.clear();
		std::vector<int> intStart1{ 0,0 };
		std::vector<int> intStart2{ -1,0 };
		positions.push_back(intStart1);
		positions.push_back(intStart2);
	}
	SnakePos()
	{
		reset();
	}
	void eat(Direction dir)
	{
		std::vector<int> newPos = positions[0];
		if (dir == Direction::up) // Up
		{
			newPos[1] += 1;
			if (newPos[1] > numy/2 - 1)
			{
				newPos[1] *= -1;
			}
		}
		else if (dir == Direction::down) // Down
		{
			if (newPos[1] <= -numy/2)
			{
				newPos[1] *= -1;
			}
			newPos[1] -= 1;
		}
		else if (dir == Direction::left) // Left
		{
			if (newPos[0] <= -numx/2)
			{
				newPos[0] *= -1;
			}
			newPos[0] -= 1;
		}
		else // Right
		{
			newPos[0] += 1;
			if (newPos[0] > numx/2 - 1)
			{
				newPos[0] *= -1;
			}
		}

		positions.push_front(newPos);
	}

	void retract()
	{
		positions.pop_back();
	}

	bool overlapCheck()
	{
		for (auto it = ++positions.begin(); it != positions.end(); ++it)
		{
			if (positions[0] == *it) return true;
		}
		return false;
	}

	const std::deque<std::vector<int>> getPositions()
	{
		return positions;
	}

	const std::vector<int> getPosition(unsigned int index)
	{
		return positions[index];
	}

	~SnakePos() {}
};

class ApplePos
{
private:
	std::vector<int> position; // Actual position scaled up to be from [-5, 4] in integers.
public:
	void newPosition()
	{
		std::srand(std::time(NULL));
		int randx = (rand() % numx) - numx/2;
		std::srand(std::time(NULL) + 1);
		int randy = (rand() % numy) - numy/2;
		position = { randx,randy };
	}

	ApplePos()
	{
		this->newPosition();
	}

	const std::vector<int> getPosition()
	{
		return position;
	}

	~ApplePos(){}
};

// Declaration of the GL Window
int window;

// Game State Parameters
bool help = false;
bool paused = false;
bool game_over = false;
int game_speed = 1;
int game_speed_buffer = 1;
unsigned int score = 0;
unsigned int high_score = 0;
Direction inDirection = Direction::right;
int test_counter = 0;

// Declaration of Global Snake and Apple Objects
SnakePos Snake;
ApplePos Apple;

// Graphics Functions Declarations
void init();
void display();
void glutPrint(double x, double y, std::string str, void* FONT);
void windowReshaping(int width, int height);
void drawRect(std::vector<double> coords, SnakeOrApple val);
void drawSnake(SnakePos positions);
void drawApple(ApplePos apple);

// Function Declarations
void display_help();
void keyControl(unsigned char key, int x, int y);
void specialKeyControl(int key, int x, int y);
void updateGame();
