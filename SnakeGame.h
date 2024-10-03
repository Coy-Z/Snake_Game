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
#include<set>
#include<unordered_set>
#include<cmath>
#include<string>
#include<cstdlib>
#include<ctime>
#include<sstream>
#include<fstream>

// Defining Global Constants
const int WINDOW_WIDTH = 500, WINDOW_HEIGHT = 500;
const int NUMX = 10, NUMY = 10;
const double DX = 2. / NUMX, DY = 2. / NUMY;
const unsigned long MAX_DELAY = 200000;

// Prototypes of Enumerations and Classes
enum class Direction;
enum SnakeOrApple;
class SnakePos;
class ApplePos;

// Prototype of the GL Window
int window;

// Graphics Functions Prototypes
void init();
void display();
void glutPrint(double x, double y, std::string str, void* FONT);
void windowReshaping(int width, int height);
void drawRect(std::vector<double> coords, SnakeOrApple val);
void drawSnake(SnakePos positions);
void drawApple(ApplePos apple);

// Function Prototypes
void display_help();
void keyControl(unsigned char key, int x, int y);
void specialKeyControl(int key, int x, int y);
void updateGame();
