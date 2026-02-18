#pragma once
#include "globals.h"

struct Projectile {
    float x, y;
    float vx;
    bool  active;

    Projectile();
};

extern Projectile projs[MAX_PROJ];

void initProjectiles();
void spawnProjectile(float x, float y, int dir);
void updateProjectiles();
void drawProjectiles();
