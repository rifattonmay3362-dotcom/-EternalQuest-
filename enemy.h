#pragma once
#include "globals.h"

struct Enemy {
    float x, y;
    int   hp;
    bool  alive;
    int   dir;
    float attackTimer;

    Enemy();
    void init(float px, float py);
    void update(int nightBonus);
    void draw();
};

extern Enemy enemies[MAX_ENEMIES];

void initEnemies();
