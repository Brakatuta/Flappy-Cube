#ifndef GAME_H
#define GAME_H

#pragma once

// =====================
//  System Includes
// =====================
#include <windows.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>

// =====================
//  Project Includes
// =====================
#include "boosts/coin.h"
#include "boosts/heart.h"

using namespace std;

// =====================
//  Game State
// =====================
enum State { MENU, PLAYING, GAMEOVER, PAUSED };
extern State currentState;

// =====================
//  Data Structures
// =====================
struct Cloud {
    float x, y, z;
    float w, h, d;
    float speedMult;
};

struct Mountain {
    float x, y, z;
    float w, h;
    float r, g, b;
    float speedMult;
};

struct Pipe {
    float x;
    float gapY;
    bool passed;
};

// =====================
//  Globals
// =====================
extern vector<Cloud> clouds;
extern vector<Mountain> activeMountains;
extern vector<Pipe> pipes;

extern vector<Coin> activeCoins;
extern vector<Heart> activeHearts;

extern ModelOBJ playerModel;
extern ModelOBJ heartModel;

extern int score;
extern int highScore;
extern int extraLives;

extern float cubeY;
extern float velocity;
extern float currentSpeed;
extern float currentGap;
extern float invincibilityTimer;

extern GLuint menuBackground;

// =====================
//  Core Functions
// =====================
void saveHighScore();
void loadHighScore();

void resetGame();

void display();
void update(int value);

// =====================
//  Update Helpers
// =====================
void updateMountains();
void updateClouds();

void keyboard(unsigned char key, int x, int y);

// =====================
//  Logic Helpers
// =====================
bool checkCollision(float px, float py, float pGapY);
void initLighting();

#endif
