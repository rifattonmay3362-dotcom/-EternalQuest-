#pragma once
#include "globals.h"

struct Resource {
    float x, y;
    int   type;       // 0=Wood  1=Stone  2=Food
    bool  collected;

    Resource();
    void draw();
};

extern Resource resources[MAX_RESOURCES];

void initResources();
void checkResourcePickup();
