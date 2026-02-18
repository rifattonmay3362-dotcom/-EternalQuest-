#pragma once

struct Boss {
    float x, y;
    int   hp;
    bool  alive;
    int   dir;
    float attackTimer;
    int   phase;   // 1=normal, 2=enraged

    Boss();
    void reset();
    void update(int nightBonus, bool allEnemiesDead);
    void draw();
};

extern Boss boss;
