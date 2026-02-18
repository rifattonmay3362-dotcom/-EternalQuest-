#pragma once

// Screen
const int SCREEN_W = 800;
const int SCREEN_H = 600;

// Game States
const int STATE_MENU = 0;
const int STATE_PLAYING = 1;
const int STATE_GAMEOVER = 2;
const int STATE_WIN = 3;
const int STATE_HOWTOPLAY = 4;

// Player constants
const int PLAYER_SPEED = 4;
const int PLAYER_W = 40;
const int PLAYER_H = 50;
const int PLAYER_MAX_HP = 100;

// Enemy constants
const int MAX_ENEMIES = 6;
const int ENEMY_W = 36;
const int ENEMY_H = 44;
const int ENEMY_SPEED = 1;
const int ENEMY_MAX_HP = 40;

// Boss constants
const int BOSS_W = 70;
const int BOSS_H = 80;
const int BOSS_MAX_HP = 200;
const int BOSS_SPEED = 1;

// Resource constants
const int MAX_RESOURCES = 8;
const int RES_W = 20;
const int RES_H = 20;

// Projectile constants (Updated to float)
const int MAX_PROJ = 10;
const float PROJ_SPEED = 7.0f;
const int PROJ_W = 12;
const int PROJ_H = 8;

// Global game variables (defined in iMain.cpp)
extern int   gameState;
extern int   animTick;
extern int   dayPhase;
extern float dayTimer;
extern bool  bossDefeated;

// Key states
extern int keyLeft, keyRight, keyUp, keyDown;

// Collision helper
bool collides(float ax, float ay, int aw, int ah,
	float bx, float by, int bw, int bh);