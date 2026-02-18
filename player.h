#pragma once

struct Player {
    float x, y;
    int   hp;
    int   dir;        // 1=right, -1=left
    int   wood, stone, food;
    bool  attacking;
    float attackTimer;
    float invincTimer;

    Player();
    void reset();
    void update();
    void draw();
    void shoot();
};

extern Player player;
