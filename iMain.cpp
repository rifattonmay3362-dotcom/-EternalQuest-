// ================================================================
//  ETERNAL QUEST: REBIRTH OF CIVILIZATION
//  COMPLETE SINGLE FILE - iGraphics Version
//
//  THIS FILE CONTAINS EVERYTHING - NO OTHER .CPP FILES NEEDED
//
//  PROJECT SETUP (ONLY 3 STEPS):
//  ---------------------------------------------------------------
//  Step 1: In Solution Explorer -> Source Files:
//          ONLY add: iMain.cpp  (this file)
//          REMOVE:   Resource.cpp, Inventory.cpp,
//                    Player.cpp, Zone.cpp, main.cpp
//          (Right-click each one -> Remove)
//
//  Step 2: Properties -> General -> Character Set ->
//          "Use Multi-Byte Character Set"
//
//  Step 3: F7 to Build, Ctrl+F5 to Run
// ================================================================

#include "iGraphics.h"
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>

// ================================================================
//  RESOURCE TYPES
// ================================================================
enum ResourceType { WOOD, STONE, FOOD, METAL };

// ================================================================
//  RESOURCE CLASS (replaces Resource.h + Resource.cpp)
// ================================================================
class Resource {
private:
    ResourceType type;
    float x, y;
    int   amount;
    bool  isCollected;
    float collectionRadius;

public:
    Resource() : type(WOOD),x(0),y(0),amount(1),
                 isCollected(false),collectionRadius(40.0f) {}

    Resource(ResourceType t, float px, float py, int amt)
        : type(t),x(px),y(py),amount(amt),
          isCollected(false),collectionRadius(40.0f) {}

    ResourceType getType()      const { return type; }
    float        getX()         const { return x; }
    float        getY()         const { return y; }
    int          getAmount()    const { return amount; }
    bool         getCollected() const { return isCollected; }

    std::string getTypeName() const {
        if (type==WOOD)  return "Wood";
        if (type==STONE) return "Stone";
        if (type==FOOD)  return "Food";
        if (type==METAL) return "Metal";
        return "Unknown";
    }

    bool canCollect(float px, float py) const {
        if (isCollected) return false;
        float dx=px-x, dy=py-y;
        return std::sqrt(dx*dx+dy*dy) <= collectionRadius;
    }

    void collect()  { isCollected = true;  }
    void respawn()  { isCollected = false; }
};

// ================================================================
//  INVENTORY CLASS (replaces Inventory.h + Inventory.cpp)
// ================================================================
class Inventory {
private:
    int wood, stone, food, metal;
    int maxWood, maxStone, maxFood, maxMetal;

public:
    Inventory() : wood(0),stone(0),food(0),metal(0),
                  maxWood(50),maxStone(50),maxFood(30),maxMetal(20) {}

    int getWood()  const { return wood;  }
    int getStone() const { return stone; }
    int getFood()  const { return food;  }
    int getMetal() const { return metal; }

    bool addResource(ResourceType t, int amt) {
        if (t==WOOD)  { if(wood +amt>maxWood)  {wood =maxWood; return false;} wood +=amt; return true; }
        if (t==STONE) { if(stone+amt>maxStone) {stone=maxStone;return false;} stone+=amt; return true; }
        if (t==FOOD)  { if(food +amt>maxFood)  {food =maxFood; return false;} food +=amt; return true; }
        if (t==METAL) { if(metal+amt>maxMetal) {metal=maxMetal;return false;} metal+=amt; return true; }
        return false;
    }

    bool canCompleteZone(int rw,int rs,int rf,int rm) const {
        return wood>=rw && stone>=rs && food>=rf && metal>=rm;
    }
};

// ================================================================
//  ZONE CLASS (replaces Zone.h + Zone.cpp)
// ================================================================
class Zone {
private:
    std::string          name;
    std::vector<Resource> resources;

    void spawn(ResourceType t, int count, int maxAmt) {
        // Divide the full playable area into a grid of `count` cells and place
        // one resource per cell (with a small random nudge inside the cell).
        // Playable area: x=20..1180 (width=1160), y=10..215 (height=205)
        // Grid: find cols x rows where cols*rows >= count, cols proportional to aspect.
        int cols = (int)std::ceil(std::sqrt((double)count * 1160.0 / 205.0));
        int rows = (int)std::ceil((double)count / cols);
        if(cols < 1) cols = 1;
        if(rows < 1) rows = 1;

        float cellW = 1160.0f / cols;
        float cellH =  205.0f / rows;

        int placed = 0;
        for(int r = 0; r < rows && placed < count; r++){
            for(int c = 0; c < cols && placed < count; c++){
                // Cell origin
                float ox = 20.0f  + c * cellW;
                float oy = 10.0f  + r * cellH;
                // Random position inside the inner 70% of the cell
                float margin = 0.15f;
                float x = ox + cellW  * margin + (float)(rand() % (int)(cellW  * (1.0f - 2*margin) + 1));
                float y = oy + cellH  * margin + (float)(rand() % (int)(cellH  * (1.0f - 2*margin) + 1));
                int   a = (rand() % maxAmt) + 1;
                resources.push_back(Resource(t, x, y, a));
                placed++;
            }
        }
    }

public:
    Zone() {}
    Zone(const std::string& zoneName) : name(zoneName) {
        resources.clear();
        if (name=="Forest")      { spawn(WOOD,15,3); spawn(FOOD,10,2); spawn(STONE,3,2); spawn(METAL,4,2); }
        else if (name=="Mountain"){ spawn(STONE,20,3);spawn(METAL,8,2); spawn(WOOD,5,2); }
        else if (name=="Desert")  { spawn(STONE,12,3);spawn(METAL,10,2);spawn(FOOD,6,2); }
        else if (name=="River")   { spawn(FOOD,15,2); spawn(METAL,5,2); spawn(STONE,6,2);}
        else if (name=="FinalCity"){ spawn(WOOD,10,2);spawn(STONE,10,2);spawn(FOOD,10,2);spawn(METAL,15,2);}
    }

    std::string            getName()      const { return name; }
    std::vector<Resource>& getResources()       { return resources; }
};

// ================================================================
//  PLAYER CLASS (replaces Player.h + Player.cpp)
// ================================================================
class Player {
private:
    float     x, y, speed;
    int       health, maxHealth;
    Inventory inventory;

public:
    // y=60 places the player's feet near the ground (ground occupies y=0..270)
    Player() : x(600),y(60),speed(5.0f),health(100),maxHealth(100) {}
    Player(float sx,float sy) : x(sx),y(sy),speed(5.0f),health(100),maxHealth(100) {}

    float getX()         const { return x; }
    float getY()         const { return y; }
    int   getHealth()    const { return health; }
    int   getMaxHealth() const { return maxHealth; }

    void move(float dx, float dy) { x+=dx; y+=dy; }
    void setPosition(float nx,float ny) { x=nx; y=ny; }
    void takeDamage(int dmg) { health-=dmg; if(health<0) health=0; }
    void heal(int amt)       { health+=amt; if(health>maxHealth) health=maxHealth; }

    Inventory&       getInventory()       { return inventory; }
    const Inventory& getInventory() const { return inventory; }

    int collectNearbyResources(std::vector<Resource>& res) {
        int n=0;
        for (size_t i=0;i<res.size();i++) {
            if (!res[i].getCollected() && res[i].canCollect(x,y)) {
                if (inventory.addResource(res[i].getType(),res[i].getAmount())) {
                    res[i].collect(); n++;
                }
            }
        }
        return n;
    }
};

// ================================================================
//  SCREEN CONSTANTS
// ================================================================
#define SW 1200
#define SH 700

// ================================================================
//  GAME STATE
// ================================================================
enum GameState { MENU, PLAYING, PAUSED, ZONE_COMPLETE };

static GameState gameState      = MENU;
static int       menuSel        = 0;
static int       currentZoneIdx = 0;
static int       playerDir      = 0;

// ---- In-game top menu bar ----
// Items: 0=New Game  1=Continue  2=Resources Collected  3=Exit
static bool   menuBarOpen    = false;
static int    menuBarHover   = -1;
static bool   showResPanel   = false;

static bool zoneUnlocked[5] = {true, false,false,false,false};
static bool zoneCompleted[5] = {false,false,false,false,false};

static const char* ZONE_NAMES[5] = {
    "FOREST ZONE","MOUNTAIN ZONE","DESERT ZONE","RIVER ZONE","FINAL CITY"
};
static const char* ZONE_KEYS[5] = {
    "Forest","Mountain","Desert","River","FinalCity"
};
// [wood, stone, food, metal]
static const int ZONE_REQ[5][4] = {
    {15, 0, 8, 0},
    { 8,22, 0, 0},
    { 0,12,15, 0},
    { 0, 0,18,15},
    {30,30,22,22}
};

static Player* gPlayer = NULL;
static Zone*   gZone   = NULL;

// ================================================================
//  ENEMY SYSTEM
// ================================================================
struct Enemy {
    float x, y;
    float speed;
    int   health, maxHealth;
    int   dir;           // 0=down,1=up,2=left,3=right
    int   attackTimer;   // frames until next attack
    int   attackCooldown;
    int   damage;
    bool  alive;
    // slight wobble for animation
    int   walkFrame;
    int   walkTimer;

    Enemy(float px, float py)
        : x(px), y(py), speed(1.8f), health(60), maxHealth(60),
          dir(0), attackTimer(0), attackCooldown(90), damage(8),
          alive(true), walkFrame(0), walkTimer(0) {}
};

static std::vector<Enemy> gEnemies;
static int enemySpawnTimer = 0;
static int enemySpawnInterval = 300; // frames between spawns

// ================================================================
//  UTILITIES
// ================================================================
static std::string itos(int n) {
    std::ostringstream ss; ss<<n; return ss.str();
}

static void drawStr(double x,double y,const std::string& s,
                    void* font=GLUT_BITMAP_HELVETICA_18) {
    char buf[512];
    int len=(int)s.size(); if(len>511)len=511;
    for(int i=0;i<len;i++) buf[i]=s[i];
    buf[len]='\0';
    iText(x,y,buf,font);
}

static void drawPanel(double x,double y,double w,double h,
                      double r,double g,double b) {
    iSetColor(r,g,b); iFilledRectangle(x,y,w,h);
    iSetColor(180,180,180); iRectangle(x,y,w,h);
}

static void drawBar(double x,double y,double w,double h,
                    double val,double maxVal,
                    double fr,double fg,double fb) {
    iSetColor(45,45,45); iFilledRectangle(x,y,w,h);
    if(maxVal>0){ double f=(val/maxVal)*w; if(f>w)f=w;
        iSetColor(fr,fg,fb); iFilledRectangle(x,y,f,h); }
    iSetColor(160,160,160); iRectangle(x,y,w,h);
}

// iGraphics has no iFilledTriangle — this wrapper calls iFilledPolygon instead.
static void iFilledTriangle(double x1,double y1,
                             double x2,double y2,
                             double x3,double y3) {
    double px[]={x1,x2,x3};
    double py[]={y1,y2,y3};
    iFilledPolygon(px,py,3);
}

// ================================================================
//  DRAW: Player sprite  (realistic humanoid)
// ================================================================
static void drawCharacter(double cx,double cy,int dir) {
    // ---- BOOTS ----
    iSetColor(50,30,10);                                          // dark leather
    iFilledRectangle(cx-11,cy,   10,10);                         // left boot
    iFilledRectangle(cx+2, cy,   10,10);                         // right boot
    iSetColor(30,15,5);                                           // boot sole
    iFilledRectangle(cx-13,cy,   12,4);
    iFilledRectangle(cx+1, cy,   12,4);

    // ---- LEGS (trousers) ----
    iSetColor(45,45,90);                                          // dark navy
    iFilledRectangle(cx-10,cy+10, 9,20);                         // left thigh
    iFilledRectangle(cx+2, cy+10, 9,20);                         // right thigh
    iSetColor(35,35,75);                                          // knee shadow
    iFilledRectangle(cx-10,cy+24, 9, 6);
    iFilledRectangle(cx+2, cy+24, 9, 6);

    // ---- BELT ----
    iSetColor(60,35,10);
    iFilledRectangle(cx-11,cy+30,22, 5);
    iSetColor(200,170,60);                                        // buckle
    iFilledRectangle(cx-4, cy+30, 8, 5);
    iSetColor(240,210,80);
    iFilledRectangle(cx-2, cy+31, 4, 3);

    // ---- TORSO ----
    iSetColor(70,110,190);                                        // blue shirt
    iFilledRectangle(cx-11,cy+35,22,22);
    iSetColor(50, 90,160);                                        // shirt shadow sides
    iFilledRectangle(cx-11,cy+35, 5,22);
    iFilledRectangle(cx+6, cy+35, 5,22);
    iSetColor(90,130,210);                                        // shirt highlight centre
    iFilledRectangle(cx-4, cy+38,  8,14);
    // collar
    iSetColor(230,200,160);
    iFilledRectangle(cx-4, cy+55,  8, 4);

    // ---- ARMS ----
    if(dir==2){  // facing left — left arm extended
        iSetColor(60,100,180);  iFilledRectangle(cx-22,cy+36,11,8);   // upper arm L
        iSetColor(50, 90,160);  iFilledRectangle(cx-22,cy+44,11,7);   // lower arm L
        iSetColor(230,190,140); iFilledCircle(cx-18,cy+52,5);          // hand L
        iSetColor(60,100,180);  iFilledRectangle(cx+11,cy+36, 9,8);   // upper arm R (tucked)
        iSetColor(50, 90,160);  iFilledRectangle(cx+11,cy+44, 9,7);
        iSetColor(230,190,140); iFilledCircle(cx+16,cy+52,5);
    } else if(dir==3){  // facing right — right arm extended
        iSetColor(60,100,180);  iFilledRectangle(cx+11,cy+36,11,8);
        iSetColor(50, 90,160);  iFilledRectangle(cx+11,cy+44,11,7);
        iSetColor(230,190,140); iFilledCircle(cx+18,cy+52,5);
        iSetColor(60,100,180);  iFilledRectangle(cx-20,cy+36, 9,8);
        iSetColor(50, 90,160);  iFilledRectangle(cx-20,cy+44, 9,7);
        iSetColor(230,190,140); iFilledCircle(cx-15,cy+52,5);
    } else {  // forward / back — arms at sides
        iSetColor(60,100,180);
        iFilledRectangle(cx-21,cy+36,10,8);
        iFilledRectangle(cx+11,cy+36,10,8);
        iSetColor(50,90,160);
        iFilledRectangle(cx-21,cy+44,10,8);
        iFilledRectangle(cx+11,cy+44,10,8);
        iSetColor(230,190,140);
        iFilledCircle(cx-16,cy+53,5);
        iFilledCircle(cx+16,cy+53,5);
    }

    // ---- NECK ----
    iSetColor(230,190,140);
    iFilledRectangle(cx-4,cy+57,8,7);

    // ---- HEAD ----
    iSetColor(230,190,140);                                       // skin
    iFilledCircle(cx,cy+72,13);
    // jaw/chin shaping
    iSetColor(215,175,130);
    iFilledRectangle(cx-7,cy+60,14,10);
    iSetColor(230,190,140);
    iFilledRectangle(cx-6,cy+62,12,10);

    // ---- HAIR ----
    iSetColor(80,45,10);
    iFilledCircle(cx,   cy+81,13);                               // top of head
    iFilledRectangle(cx-13,cy+72,4,10);                          // left sideburn
    iFilledRectangle(cx+9, cy+72,4,10);                          // right sideburn
    iFilledRectangle(cx-10,cy+80,20,5);                          // fringe

    // ---- EYES (direction-sensitive) ----
    iSetColor(255,255,255);
    if(dir==2){                                                   // left
        iFilledCircle(cx-5,cy+71,3);
        iSetColor(50,30,10); iFilledCircle(cx-6,cy+71,2);
        iSetColor(0,0,0);    iFilledCircle(cx-7,cy+71,1);
    } else if(dir==3){                                            // right
        iFilledCircle(cx+5,cy+71,3);
        iSetColor(50,30,10); iFilledCircle(cx+6,cy+71,2);
        iSetColor(0,0,0);    iFilledCircle(cx+7,cy+71,1);
    } else if(dir==1){                                            // up (back)
        // back of head — no face visible
    } else {                                                      // forward
        iFilledCircle(cx-5,cy+71,3);
        iFilledCircle(cx+5,cy+71,3);
        iSetColor(60,40,20); iFilledCircle(cx-5,cy+71,2); iFilledCircle(cx+5,cy+71,2);
        iSetColor(0,0,0);    iFilledCircle(cx-5,cy+71,1); iFilledCircle(cx+5,cy+71,1);
    }

    // ---- NOSE & MOUTH ----
    if(dir!=1){
        iSetColor(200,160,120);
        iFilledCircle(cx,cy+68,2);                               // nose
        iSetColor(160,90,80);
        if(dir==2) iFilledRectangle(cx-6,cy+64,5,2);
        else if(dir==3) iFilledRectangle(cx+1,cy+64,5,2);
        else iFilledRectangle(cx-3,cy+64,6,2);                  // mouth
    }

    // ---- EYEBROWS ----
    if(dir!=1){
        iSetColor(70,40,10);
        if(dir==2){      iLine(cx-8,cy+75,cx-2,cy+76); }
        else if(dir==3){ iLine(cx+2,cy+75,cx+8,cy+76); }
        else { iLine(cx-8,cy+75,cx-2,cy+76); iLine(cx+2,cy+75,cx+8,cy+76); }
    }
}

// ================================================================
//  DRAW: Enemy sprite  (dark armoured humanoid with sword)
// ================================================================
static void drawEnemy(double cx, double cy, int dir, int walkFrame) {
    double legSwing = (walkFrame % 2 == 0) ? 3.0 : -3.0;

    // ---- BOOTS (dark) ----
    iSetColor(25,15,5);
    iFilledRectangle(cx-11+legSwing, cy,    10,10);
    iFilledRectangle(cx+2-legSwing,  cy,    10,10);
    iSetColor(15,8,2);
    iFilledRectangle(cx-13+legSwing, cy,    12,4);
    iFilledRectangle(cx+1-legSwing,  cy,    12,4);

    // ---- LEGS (dark leather) ----
    iSetColor(40,20,10);
    iFilledRectangle(cx-10+legSwing, cy+10, 9,20);
    iFilledRectangle(cx+2-legSwing,  cy+10, 9,20);
    iSetColor(25,12,5);
    iFilledRectangle(cx-10+legSwing, cy+24, 9,6);
    iFilledRectangle(cx+2-legSwing,  cy+24, 9,6);

    // ---- BELT ----
    iSetColor(35,15,5);
    iFilledRectangle(cx-11,cy+30,22,5);
    iSetColor(160,30,30);   // red buckle
    iFilledRectangle(cx-4,cy+30,8,5);
    iSetColor(200,50,50);
    iFilledRectangle(cx-2,cy+31,4,3);

    // ---- TORSO (dark armour) ----
    iSetColor(55,55,65);   // dark plate
    iFilledRectangle(cx-11,cy+35,22,22);
    iSetColor(35,35,45);
    iFilledRectangle(cx-11,cy+35,5,22);
    iFilledRectangle(cx+6, cy+35,5,22);
    iSetColor(80,80,95);   // armour highlight
    iFilledRectangle(cx-3,cy+40,6,10);
    // rivets
    iSetColor(180,160,60);
    iFilledCircle(cx-7,cy+40,2); iFilledCircle(cx+7,cy+40,2);
    iFilledCircle(cx-7,cy+50,2); iFilledCircle(cx+7,cy+50,2);
    // collar/neck plate
    iSetColor(60,60,70);
    iFilledRectangle(cx-4,cy+55,8,4);

    // ---- ARMS ----
    if(dir==2){
        iSetColor(50,50,60);  iFilledRectangle(cx-22,cy+36,11,8);
        iSetColor(40,40,50);  iFilledRectangle(cx-22,cy+44,11,7);
        iSetColor(180,140,100); iFilledCircle(cx-18,cy+52,5);
        iSetColor(50,50,60);  iFilledRectangle(cx+11,cy+36,9,8);
        iSetColor(40,40,50);  iFilledRectangle(cx+11,cy+44,9,7);
        iSetColor(180,140,100); iFilledCircle(cx+16,cy+52,5);
        // SWORD — facing left, sword extends left
        iSetColor(180,180,190); iFilledRectangle(cx-40,cy+46,24,5); // blade
        iSetColor(220,220,230); iFilledRectangle(cx-40,cy+47,24,2); // highlight
        iSetColor(200,50,50);   iFilledRectangle(cx-22,cy+43,4,11); // guard
        iSetColor(100,65,25);   iFilledRectangle(cx-20,cy+44,10,9); // grip
        // sword tip triangle
        iSetColor(160,160,170);
        double tx[]={cx-40,cx-48,cx-40}; double ty[]={cy+46,cy+48,cy+51};
        iFilledPolygon(tx,ty,3);
    } else if(dir==3){
        iSetColor(50,50,60);  iFilledRectangle(cx+11,cy+36,11,8);
        iSetColor(40,40,50);  iFilledRectangle(cx+11,cy+44,11,7);
        iSetColor(180,140,100); iFilledCircle(cx+18,cy+52,5);
        iSetColor(50,50,60);  iFilledRectangle(cx-20,cy+36,9,8);
        iSetColor(40,40,50);  iFilledRectangle(cx-20,cy+44,9,7);
        iSetColor(180,140,100); iFilledCircle(cx-15,cy+52,5);
        // SWORD — facing right, sword extends right
        iSetColor(180,180,190); iFilledRectangle(cx+18,cy+46,24,5);
        iSetColor(220,220,230); iFilledRectangle(cx+18,cy+47,24,2);
        iSetColor(200,50,50);   iFilledRectangle(cx+18,cy+43,4,11);
        iSetColor(100,65,25);   iFilledRectangle(cx+12,cy+44,8,9);
        iSetColor(160,160,170);
        double tx[]={cx+42,cx+50,cx+42}; double ty[]={cy+46,cy+48,cy+51};
        iFilledPolygon(tx,ty,3);
    } else {
        iSetColor(50,50,60);
        iFilledRectangle(cx-21,cy+36,10,8);
        iFilledRectangle(cx+11,cy+36,10,8);
        iSetColor(40,40,50);
        iFilledRectangle(cx-21,cy+44,10,8);
        iFilledRectangle(cx+11,cy+44,10,8);
        iSetColor(180,140,100);
        iFilledCircle(cx-16,cy+53,5);
        iFilledCircle(cx+16,cy+53,5);
        // SWORD — vertical (held downward when facing screen)
        iSetColor(180,180,190); iFilledRectangle(cx+11,cy+52,5,22);
        iSetColor(220,220,230); iFilledRectangle(cx+12,cy+52,2,22);
        iSetColor(200,50,50);   iFilledRectangle(cx+8, cy+50,11,5);
        iSetColor(100,65,25);   iFilledRectangle(cx+9, cy+44,7,8);
        iSetColor(160,160,170);
        double tx[]={cx+11,cx+13,cx+16}; double ty[]={cy+74,cy+82,cy+74};
        iFilledPolygon(tx,ty,3);
    }

    // ---- NECK ----
    iSetColor(180,140,100);
    iFilledRectangle(cx-4,cy+57,8,7);

    // ---- HEAD (with dark helmet) ----
    iSetColor(180,140,100);  // skin — lower face
    iFilledCircle(cx,cy+72,13);
    iSetColor(160,125,90);
    iFilledRectangle(cx-7,cy+60,14,10);
    iSetColor(180,140,100);
    iFilledRectangle(cx-6,cy+62,12,10);

    // Helmet (metal cap + cheek guards)
    iSetColor(65,65,75);
    iFilledCircle(cx,cy+81,14);          // top dome
    iFilledRectangle(cx-13,cy+72,4,12); // left cheek guard
    iFilledRectangle(cx+9, cy+72,4,12); // right cheek guard
    iFilledRectangle(cx-14,cy+78,28,6); // brow plate
    iSetColor(90,90,105);
    iFilledRectangle(cx-12,cy+80,24,3); // highlight on brow
    // Rivets on helmet
    iSetColor(180,160,60);
    iFilledCircle(cx-8,cy+83,2); iFilledCircle(cx+8,cy+83,2);

    // ---- GLOWING RED EYES ----
    iSetColor(220,30,10);
    if(dir!=1){
        if(dir==2){
            iFilledCircle(cx-5,cy+71,3);
            iSetColor(255,80,50); iFilledCircle(cx-6,cy+71,1);
        } else if(dir==3){
            iFilledCircle(cx+5,cy+71,3);
            iSetColor(255,80,50); iFilledCircle(cx+6,cy+71,1);
        } else {
            iFilledCircle(cx-5,cy+71,3); iFilledCircle(cx+5,cy+71,3);
            iSetColor(255,80,50); iFilledCircle(cx-5,cy+71,1); iFilledCircle(cx+5,cy+71,1);
        }
    }

    // ---- ENEMY HEALTH BAR (above head) ----
    // drawn small above the sprite
}

// Draw a small health bar above an enemy
static void drawEnemyHealthBar(double cx, double cy, int hp, int maxhp) {
    double bw = 40, bh = 6;
    double bx = cx - bw/2, by = cy + 88;
    iSetColor(60,10,10);  iFilledRectangle(bx,by,bw,bh);
    double fill = (maxhp>0)?(bw*hp/maxhp):0;
    iSetColor(220,40,40); iFilledRectangle(bx,by,fill,bh);
    iSetColor(150,50,50); iRectangle(bx,by,bw,bh);
}


static void drawResourceIcon(double x,double y,ResourceType type) {
    switch(type){

    case WOOD: {
        // Stacked log pile — three logs with end grain rings and bark texture
        // Bottom log
        iSetColor(101,67,33);   iFilledRectangle(x-16,y+1,  32,11);
        iSetColor(130,85,40);   iFilledRectangle(x-16,y+2,  32, 4);   // highlight
        iSetColor(80, 50,20);   iLine(x-16,y+6,  x+16,y+6);            // bark line
        iSetColor(80, 50,20);   iLine(x-16,y+9,  x+16,y+9);
        // End-grain circle left
        iSetColor(140,90,45);   iFilledCircle(x-16,y+6,  6);
        iSetColor(115,72,35);   iFilledCircle(x-16,y+6,  4);
        iSetColor(90, 55,25);   iFilledCircle(x-16,y+6,  2);
        // End-grain circle right
        iSetColor(140,90,45);   iFilledCircle(x+16,y+6,  6);
        iSetColor(115,72,35);   iFilledCircle(x+16,y+6,  4);
        iSetColor(90, 55,25);   iFilledCircle(x+16,y+6,  2);
        // Middle log (slightly offset)
        iSetColor(110,72,36);   iFilledRectangle(x-14,y+13,28,11);
        iSetColor(135,88,42);   iFilledRectangle(x-14,y+14,28, 4);
        iSetColor(80, 50,20);   iLine(x-14,y+18, x+14,y+18);
        iSetColor(140,90,45);   iFilledCircle(x-14,y+18, 5);
        iSetColor(110,70,34);   iFilledCircle(x-14,y+18, 3);
        iSetColor(140,90,45);   iFilledCircle(x+14,y+18, 5);
        iSetColor(110,70,34);   iFilledCircle(x+14,y+18, 3);
        // Top log
        iSetColor(120,78,38);   iFilledRectangle(x-12,y+25,24,10);
        iSetColor(145,95,48);   iFilledRectangle(x-12,y+26,24, 3);
        iSetColor(80, 50,20);   iLine(x-12,y+29, x+12,y+29);
        iSetColor(140,90,45);   iFilledCircle(x-12,y+30, 5);
        iSetColor(110,70,34);   iFilledCircle(x-12,y+30, 3);
        iSetColor(140,90,45);   iFilledCircle(x+12,y+30, 5);
        iSetColor(110,70,34);   iFilledCircle(x+12,y+30, 3);
        // Small axe head resting on top
        iSetColor(150,150,155); iFilledRectangle(x+2, y+32, 8,8);
        iSetColor(180,180,185); iFilledRectangle(x+2, y+32, 8,3);
        iSetColor(101,67,33);   iFilledRectangle(x+4, y+28, 3,8);      // handle
        // Label
        iSetColor(0,0,0);       iFilledRectangle(x-8,y,16,11);
        iSetColor(255,220,100); drawStr(x-6,y+1,"WOOD",GLUT_BITMAP_HELVETICA_12);
        break;
    }

    case STONE: {
        // Rock cluster — 3 rocks of different sizes with facets and cracks
        // Back-left rock (large)
        iSetColor(120,120,125); iFilledCircle(x-8, y+18, 14);
        iSetColor(145,145,150); iFilledCircle(x-10,y+22, 10);          // highlight facet
        iSetColor(90, 90, 95);  iFilledCircle(x-5, y+14,  6);          // shadow facet
        iSetColor(70, 70, 75);  iLine(x-8,y+10,x-14,y+20);             // crack line
        iSetColor(70, 70, 75);  iLine(x-8,y+10,x-3, y+18);
        // Back-right rock (medium)
        iSetColor(130,128,133); iFilledCircle(x+9, y+16, 11);
        iSetColor(155,152,157); iFilledCircle(x+7, y+19,  7);
        iSetColor(80, 78, 82);  iLine(x+9,y+10,x+13,y+18);
        // Front centre rock (smaller, lighter — closest to viewer)
        iSetColor(155,153,158); iFilledCircle(x,   y+10, 10);
        iSetColor(175,172,177); iFilledCircle(x-2, y+12,  6);
        iSetColor(110,108,113); iFilledCircle(x+4, y+8,   4);
        // Dirt base shadow
        iSetColor(60,45,30);    iFilledEllipse(x, y+5, 18, 4);
        // Label
        iSetColor(0,0,0);       iFilledRectangle(x-10,y,20,11);
        iSetColor(220,220,230); drawStr(x-9,y+1,"ROCK",GLUT_BITMAP_HELVETICA_12);
        break;
    }

    case FOOD: {
        // Wicker basket with 3 red apples, stems and leaves
        // Basket body
        iSetColor(160,110,50);  iFilledRectangle(x-13,y+2,  26,16);
        iSetColor(140, 95,40);  iFilledRectangle(x-13,y+2,  26, 5);    // rim shadow
        iSetColor(180,130,65);  iFilledRectangle(x-11,y+4,  22,12);    // weave highlight
        // Basket weave lines
        iSetColor(130,85,35);
        iLine(x-11,y+7,  x+11,y+7);
        iLine(x-11,y+11, x+11,y+11);
        iLine(x-7, y+4,  x-7, y+18);
        iLine(x,   y+4,  x,   y+18);
        iLine(x+7, y+4,  x+7, y+18);
        // Basket base
        iSetColor(120,80,30);   iFilledRectangle(x-14,y,    28, 5);
        // Handle arc
        iSetColor(140,95,40);   iRectangle(x-8,y+14,16,12);
        // Apple left
        iSetColor(190,30,30);   iFilledCircle(x-7,y+26,8);
        iSetColor(220,60,60);   iFilledCircle(x-9,y+28,5);              // highlight
        iSetColor(0,120,0);     iFilledRectangle(x-7,y+34,2,4);        // stem
        iSetColor(0,150,0);     // leaf
        double lx1[]={x-7,x-2,x-5}; double ly1[]={y+36,y+38,y+40};
        iFilledPolygon(lx1,ly1,3);
        // Apple right
        iSetColor(200,35,35);   iFilledCircle(x+5,y+26,8);
        iSetColor(230,65,65);   iFilledCircle(x+3,y+28,5);
        iSetColor(0,120,0);     iFilledRectangle(x+5,y+34,2,4);
        // Apple centre-top (peeking above)
        iSetColor(210,40,40);   iFilledCircle(x,   y+30,7);
        iSetColor(235,70,70);   iFilledCircle(x-2, y+32,4);
        iSetColor(0,120,0);     iFilledRectangle(x,  y+37,2,3);
        // Label
        iSetColor(0,0,0);       iFilledRectangle(x-10,y,20,11);
        iSetColor(255,180,180); drawStr(x-9,y+1,"FOOD",GLUT_BITMAP_HELVETICA_12);
        break;
    }

    case METAL: {
        // Ore vein — dark rock base with glowing crystal shards sticking up
        // Rock base
        iSetColor(55,50,60);    iFilledCircle(x,   y+8,  15);
        iSetColor(70,65,75);    iFilledCircle(x-8, y+6,  10);
        iSetColor(45,40,50);    iFilledCircle(x+8, y+6,  10);
        // Crystal shard left (teal-blue)
        iSetColor(0,180,200);
        double sx1[]={x-10,x-6,x-8};  double sy1[]={y+8,y+8,y+26};
        iFilledPolygon(sx1,sy1,3);
        iSetColor(0,220,240);
        double sh1[]={x-10,x-8,x-9};  double sy1h[]={y+12,y+26,y+26};
        iFilledPolygon(sh1,sy1h,3);   // highlight edge
        // Crystal shard centre (bright)
        iSetColor(30,210,220);
        double sx2[]={x-4,x+4,x};     double sy2[]={y+6,y+6,y+30};
        iFilledPolygon(sx2,sy2,3);
        iSetColor(120,240,250);
        double sh2[]={x-4,x,x-2};     double sy2h[]={y+6,y+30,y+30};
        iFilledPolygon(sh2,sy2h,3);
        // Crystal shard right (slightly orange-gold vein)
        iSetColor(200,150,20);
        double sx3[]={x+6,x+12,x+9};  double sy3[]={y+8,y+8,y+24};
        iFilledPolygon(sx3,sy3,3);
        iSetColor(240,190,60);
        double sh3[]={x+6,x+9,x+7};   double sy3h[]={y+8,y+24,y+24};
        iFilledPolygon(sh3,sy3h,3);
        // Glow dots around shards
        iSetColor(0,200,220);
        iFilledCircle(x-11,y+18,2); iFilledCircle(x+10,y+15,2);
        iSetColor(220,180,30);
        iFilledCircle(x+13,y+10,2);
        // Dirt shadow base
        iSetColor(40,30,20);    iFilledEllipse(x,y+4,16,4);
        // Label
        iSetColor(0,0,0);       iFilledRectangle(x-11,y,22,11);
        iSetColor(0,240,255);   drawStr(x-10,y+1,"ORE",GLUT_BITMAP_HELVETICA_12);
        break;
    }
    }
}

// ================================================================
//  DRAW: Backgrounds
// ================================================================
static void drawForestBG() {
    // Sky: y=270..SH (upper portion of screen in iGraphics)
    iSetColor(135,206,235); iFilledRectangle(0,270,SW,SH-270);
    // Ground strip: y=0..270 (bottom 270 pixels = ground)
    iSetColor(34,100,34);   iFilledRectangle(0,0,SW,270);

    // 35 trees in a 12-column x 3-row grid covering the FULL playable area.
    // xStep=101px, yStep=102px. TRKH=54, CANR=18 -> total=96px > character height(94px).
    // Vertical gap=6px, horizontal gap=53px — all trees fully separated.
    {
        const int cols  = 12, rows  = 3;
        const int xStart= 40, xEnd  = 1160;
        const int yStart= 5,  yEnd  = 210;
        const int xStep = (xEnd - xStart) / (cols - 1);   // 101 px
        const int yStep = (yEnd - yStart) / (rows - 1);   // 102 px

        const int TRKH = 54;   // trunk height — tree taller than character (96 > 94px)
        const int TRKW = 6;    // trunk width
        const int CANR = 18;   // canopy radius
        const int BLEN = 10;   // branch length

        int idx = 0;
        for(int row = 0; row < rows && idx < 35; row++){
            for(int col = 0; col < cols && idx < 35; col++){
                int tx     = xStart + col * xStep;
                int ty     = yStart + row * yStep;
                int trkTop = ty + TRKH;

                // Trunk shadow
                iSetColor(55,30,8);
                iFilledRectangle(tx-4, ty, TRKW+2, TRKH);
                // Trunk main
                iSetColor(101,67,33);
                iFilledRectangle(tx-3, ty, TRKW, TRKH);
                // Trunk highlight
                iSetColor(140,90,45);
                iFilledRectangle(tx-2, ty+1, 2, TRKH-2);
                // Bark lines
                iSetColor(70,38,12);
                iLine(tx, ty, tx, ty+TRKH);
                iLine(tx+2, ty+TRKH/3, tx+2, ty+TRKH*2/3);
                // Knot
                iSetColor(60,32,10); iFilledCircle(tx, ty+TRKH/2, 2);
                iSetColor(90,50,18); iFilledCircle(tx, ty+TRKH/2, 1);

                // Left branch stub
                iSetColor(85,55,20);
                iFilledRectangle(tx-3-BLEN, ty+TRKH*6/10, BLEN, 3);
                iSetColor(0,100,0);  iFilledCircle(tx-3-BLEN, ty+TRKH*6/10+1, 5);
                iSetColor(0,140,0);  iFilledCircle(tx-3-BLEN, ty+TRKH*6/10+1, 3);

                // Right branch stub
                iSetColor(85,55,20);
                iFilledRectangle(tx+3, ty+TRKH*4/10, BLEN, 3);
                iSetColor(0,100,0);  iFilledCircle(tx+3+BLEN, ty+TRKH*4/10+1, 5);
                iSetColor(0,140,0);  iFilledCircle(tx+3+BLEN, ty+TRKH*4/10+1, 3);

                // Root flares
                iSetColor(75,45,15);
                iFilledRectangle(tx-5, ty, 3, 4);
                iFilledRectangle(tx+2, ty, 3, 4);

                // Canopy — all radii <= CANR
                // Back shadow
                iSetColor(0,65,0);
                iFilledCircle(tx,         trkTop+CANR,     CANR);
                iFilledCircle(tx-CANR/2,  trkTop+CANR/2,   CANR-3);
                iFilledCircle(tx+CANR/2,  trkTop+CANR/2,   CANR-3);
                // Mid
                iSetColor(0,110,0);
                iFilledCircle(tx,         trkTop+CANR+2,   CANR-1);
                iFilledCircle(tx-CANR/3,  trkTop+CANR/3,   CANR-4);
                iFilledCircle(tx+CANR/3,  trkTop+CANR/3,   CANR-4);
                // Bright highlight
                iSetColor(0,155,0);
                iFilledCircle(tx-CANR/4,  trkTop+CANR+3,   CANR-4);
                // Sunlit top
                iSetColor(60,190,60);
                iFilledCircle(tx-CANR/4,  trkTop+CANR+5,   CANR/3);

                idx++;
            }
        }
    }

    // Clouds in sky (y=350..650, well above ground line at y=270)
    iSetColor(255,255,255);
    int cx[]={100,330,560,790,1020};
    int cy[]={380,420,375,405,390};
    for(int i=0;i<5;i++){
        iFilledEllipse(cx[i],   cy[i], 55,22);
        iFilledEllipse(cx[i]-38,cy[i]-5,36,18);
        iFilledEllipse(cx[i]+38,cy[i]-5,36,18);
    }
}

static void drawMountainBG() {
    // Sky: upper portion (y=270..SH)
    iSetColor(90,120,180); iFilledRectangle(0,270,SW,SH-270);
    // Ground (rocky): lower portion (y=0..270)
    iSetColor(90,90,95);   iFilledRectangle(0,0,SW,270);
    // Mountain peaks rise FROM ground line (y=270) upward into sky
    for(int i=0;i<7;i++){
        double mx=(double)(i*185);
        iSetColor(80,80,90);
        double px[]={mx, mx+92, mx+185};
        double py[]={270, 570, 270};   // base at ground (270), peak in sky (570)
        iFilledPolygon(px,py,3);
        iSetColor(230,240,255);        // snow cap
        double sx[]={mx+46, mx+92, mx+138};
        double sy[]={420,   570,   420};
        iFilledPolygon(sx,sy,3);
    }
}

static void drawDesertBG() {
    // Hot sky: upper portion (y=270..SH)
    iSetColor(255,180,60);  iFilledRectangle(0,270,SW,SH-270);
    // Sand ground: lower portion (y=0..270)
    iSetColor(210,170,100); iFilledRectangle(0,0,SW,270);
    // Sand dunes ON the ground surface (at y=270, the ground/sky boundary)
    iSetColor(190,150,80);
    int dx[]={100,295,490,685,880,1075};
    for(int i=0;i<6;i++) iFilledEllipse(dx[i],270,90,28);
    // Cacti sitting ON the ground: trunk base at y=0, growing upward
    int cax[]={60,190,320,450,580,710,840,970,1100};
    for(int i=0;i<9;i++){
        double cx=cax[i];
        double base=5;     // just above bottom edge
        iSetColor(0,130,0);
        iFilledRectangle(cx-6, base,     12, 80);   // main trunk upward
        iFilledRectangle(cx-22,base+30,  16, 10);   // left arm
        iFilledRectangle(cx+6, base+26,  16, 10);   // right arm
        iFilledRectangle(cx-22,base+30,  10, 30);   // left arm up
        iFilledRectangle(cx+12,base+26,  10, 30);   // right arm up
    }
    // Sun high in sky (top-right area)
    iSetColor(255,240,0); iFilledCircle(SW-100,SH-80,40);
}

static void drawRiverBG() {
    // Sky: upper portion (y=270..SH)
    iSetColor(120,200,240); iFilledRectangle(0,270,SW,SH-270);
    // River water: lower portion (y=0..270)
    iSetColor(30,110,220);  iFilledRectangle(0,0,SW,270);
    // Grassy banks at ground/sky boundary (y≈270)
    iSetColor(50,170,50);   iFilledRectangle(0,255,SW,30);
    // Water waves INSIDE the river (y=50..220, well within the water)
    int wx[]={70,200,330,460,590,720,850,980,1110};
    int wy[]={180,130,160,110,180,130,160,110,180};
    iSetColor(80,150,255);
    for(int i=0;i<9;i++) iFilledEllipse(wx[i],wy[i],50,14);
    // Fish inside river (y=40..120)
    int fx[]={80,270,460,650,840,1030};
    int fy[]={60,100,60,100,60,100};
    iSetColor(255,200,0);
    for(int i=0;i<6;i++){
        double ffx=fx[i],ffy=fy[i];
        iFilledEllipse(ffx,ffy,16,7);
        double ttx[]={ffx+16,ffx+28,ffx+16};
        double tty[]={ffy-8,ffy,ffy+8};
        iFilledPolygon(ttx,tty,3);
    }
}

static void drawCityBG() {
    // Night sky: upper portion (y=270..SH)
    iSetColor(20,20,40); iFilledRectangle(0,270,SW,SH-270);
    // Ground/road: lower portion (y=0..270)
    iSetColor(50,50,60); iFilledRectangle(0,0,SW,270);
    // Buildings rise FROM ground line (y=270) upward into sky
    int bh[]={200,160,240,180,200,260,170,220,200,240};
    for(int i=0;i<10;i++){
        double bx=(double)(i*122);
        iSetColor(50,50,65); iFilledRectangle(bx,270,110,bh[i]);
        for(int wy=0;wy<8;wy++) for(int ww=0;ww<4;ww++){
            bool lit=((ww+wy+i)%3!=0);
            if(lit) iSetColor(255,235,100); else iSetColor(30,30,50);
            iFilledRectangle(bx+8+ww*26, 275.0+wy*18, 16,12);
        }
    }
    // Road markings at bottom of screen
    iSetColor(60,60,60); iFilledRectangle(0,0,SW,40);
    iSetColor(240,220,0);
    for(int i=0;i<10;i++) iFilledRectangle((double)(i*130),15,65,8);
    // Moon in sky (upper-right)
    iSetColor(220,220,190); iFilledCircle(SW-90,SH-80,32);
    iSetColor(20,20,40);    iFilledCircle(SW-72,SH-88,27);
}

static void drawEnvironment(){
    if(currentZoneIdx==0) drawForestBG();
    else if(currentZoneIdx==1) drawMountainBG();
    else if(currentZoneIdx==2) drawDesertBG();
    else if(currentZoneIdx==3) drawRiverBG();
    else drawCityBG();
}

// ================================================================
//  DRAW: HUD
// ================================================================
static void drawInventoryHUD(){
    if(!gPlayer) return;
    const Inventory& inv=gPlayer->getInventory();
    // Top-left panel: in iGraphics Y=0=bottom, so SH-148 to SH puts it at top
    drawPanel(8,SH-148,230,140,18,18,18);
    iSetColor(255,210,0); drawStr(72,SH-30,"INVENTORY");
    const char* lbl[]={"Wood : ","Stone: ","Food : ","Metal: "};
    int val[]={inv.getWood(),inv.getStone(),inv.getFood(),inv.getMetal()};
    int mx[]={50,50,30,20};
    int rr[]={160,140,200,210}; int rg[]={100,140,40,130}; int rb[]={30,140,40,20};
    for(int i=0;i<4;i++){
        double ry=SH-62.0-i*26;
        iSetColor(rr[i],rg[i],rb[i]); iFilledCircle(24,ry+5,7);
        iSetColor(255,255,255);
        drawStr(36,ry,std::string(lbl[i])+itos(val[i])+"/"+itos(mx[i]),GLUT_BITMAP_HELVETICA_12);
        drawBar(130,ry,92,11,val[i],mx[i],rr[i],rg[i],rb[i]);
    }
}

static void drawHealthHUD(){
    if(!gPlayer) return;
    // Position well below the top edge so nothing gets clipped by the window border.
    // py=SH-80 means panel bottom at y=620, top at y=676 — fully inside the 700px window.
    double px = SW/2.0 - 150;
    double py = SH - 80;
    // Dark background panel (300 wide, 62 tall)
    iSetColor(10,10,10);     iFilledRectangle(px-2, py-2, 304, 66);
    iSetColor(60,60,60);     iRectangle(px-2, py-2, 304, 66);
    // "HEALTH" label — bright white, large font, left of bar
    iSetColor(255,255,255);
    drawStr(px+4, py+44, "HEALTH", GLUT_BITMAP_HELVETICA_18);
    // Red heart symbol next to label
    iSetColor(220,30,30);
    iFilledCircle(px+80, py+52, 7);
    iFilledCircle(px+92, py+52, 7);
    double hx[]={px+74,px+86,px+98}; double hy[]={py+49,py+38,py+49};
    iFilledPolygon(hx,hy,3);
    // Health bar — very tall (22px) so it can't be missed, bright green on dark trough
    iSetColor(20,20,20);     iFilledRectangle(px+4,  py+10, 292, 28);
    int hp  = gPlayer->getHealth();
    int mhp = gPlayer->getMaxHealth();
    double fill = (mhp>0) ? (292.0*hp/mhp) : 0;
    // Gradient: green when healthy, yellow when low
    if(hp > 50) iSetColor(0,220,0);
    else if(hp > 25) iSetColor(220,200,0);
    else             iSetColor(220,40,0);
    iFilledRectangle(px+4, py+10, fill, 28);
    iSetColor(180,180,180);  iRectangle(px+4, py+10, 292, 28);
    // Numeric value centred inside the bar — black bg slab so it always reads
    iSetColor(0,0,0);        iFilledRectangle(px+118, py+14, 72, 18);
    iSetColor(255,255,255);
    drawStr(px+122, py+15, itos(hp)+"/"+itos(mhp), GLUT_BITMAP_HELVETICA_12);
}

static void drawObjectivesHUD(){
    if(!gPlayer) return;
    const Inventory& inv=gPlayer->getInventory();
    const int* req=ZONE_REQ[currentZoneIdx];
    int cur[]={inv.getWood(),inv.getStone(),inv.getFood(),inv.getMetal()};
    int numObj=0; for(int i=0;i<4;i++) if(req[i]>0) numObj++;
    double panH=42.0+numObj*28;
    drawPanel(SW-252,SH-panH-8,244,panH,18,18,18);
    iSetColor(255,210,0); drawStr(SW-210,SH-28,"OBJECTIVES");
    const char* lbl[]={"Wood ","Stone","Food ","Metal"};
    int cr[]={160,140,200,210}; int cg[]={100,140,40,130}; int cb[]={30,140,40,20};
    int row=0;
    for(int i=0;i<4;i++){
        if(req[i]==0) continue;
        double ry=SH-58.0-row*28;
        iSetColor(cr[i],cg[i],cb[i]); iFilledCircle(SW-238,ry+5,6);
        bool done=(cur[i]>=req[i]);
        iSetColor(done?0:255,done?220:255,done?0:255);
        drawStr(SW-228,ry,std::string(lbl[i])+": "+itos(cur[i])+"/"+itos(req[i]),GLUT_BITMAP_HELVETICA_12);
        if(done){ iSetColor(0,220,0); drawStr(SW-90,ry,"[DONE]",GLUT_BITMAP_HELVETICA_12); }
        row++;
    }
}

static void drawZoneNameBar(){
    // Top-centre bar
    drawPanel(SW/2-160, SH-42, 320,34, 10,10,50);
    iSetColor(180,210,255);
    drawStr(SW/2-120, SH-30, ZONE_NAMES[currentZoneIdx], GLUT_BITMAP_HELVETICA_18);
}

static void drawResources(){
    if(!gZone) return;
    std::vector<Resource>& res=gZone->getResources();
    for(size_t i=0;i<res.size();i++)
        if(!res[i].getCollected())
            drawResourceIcon(res[i].getX(),res[i].getY(),res[i].getType());
}

// ================================================================
//  DRAW: In-game Top Menu Bar
// ================================================================
// Bar geometry constants
#define MBAR_H   34          // height of the top strip
#define MBAR_Y   (SH-MBAR_H) // bottom-left y of the bar (iGraphics y=0 at bottom)
#define MBAR_BTN_W  160      // width of the "MENU" trigger button

static const char* MBAR_LABELS[] = { "New Game", "Continue", "Resources Collected", "Exit to Menu" };
static const int   MBAR_COUNT    = 4;
static const double MBAR_DROP_X  = 0;           // x of dropdown panel
static const double MBAR_DROP_W  = 220;         // width of dropdown
static const double MBAR_ITEM_H  = 38;          // height of each item row

static void drawMenuBar(){
    // ---- solid bar across the very top ----
    iSetColor(15,15,35);
    iFilledRectangle(0, MBAR_Y, SW, MBAR_H);
    iSetColor(60,80,160);
    iLine(0, MBAR_Y, SW, MBAR_Y);              // bottom border glow

    // "☰ MENU" trigger button (left side of bar)
    bool btnHover = (menuBarHover == -2);
    iSetColor(btnHover ? 60:30, btnHover ? 80:40, btnHover ? 180:100);
    iFilledRectangle(0, MBAR_Y, MBAR_BTN_W, MBAR_H);
    iSetColor(100,140,255);
    iRectangle(0, MBAR_Y, MBAR_BTN_W, MBAR_H);
    // Hamburger icon lines
    iSetColor(200,210,255);
    double bx=16, by=MBAR_Y+8;
    iFilledRectangle(bx,by,    20,4);
    iFilledRectangle(bx,by+10, 20,4);
    iFilledRectangle(bx,by+20, 20,4);
    iSetColor(menuBarOpen ? 255:200, menuBarOpen ? 220:210, 255);
    drawStr(bx+26, MBAR_Y+10, menuBarOpen ? "CLOSE MENU" : "MENU", GLUT_BITMAP_HELVETICA_18);

    // ---- Zone name centred in bar ----
    iSetColor(180,210,255);
    drawStr(SW/2-120, MBAR_Y+9, ZONE_NAMES[currentZoneIdx], GLUT_BITMAP_HELVETICA_18);

    // ---- Paused badge (right side) ----
    if(gameState == PAUSED){
        iSetColor(220,160,0);
        iFilledRectangle(SW-130, MBAR_Y+5, 120, 24);
        iSetColor(0,0,0);
        drawStr(SW-112, MBAR_Y+9, "  PAUSED", GLUT_BITMAP_HELVETICA_18);
    }

    // ---- Dropdown panel ----
    if(menuBarOpen){
        double dpX = MBAR_DROP_X;
        double dpY = MBAR_Y - MBAR_COUNT * MBAR_ITEM_H;
        double dpW = MBAR_DROP_W;
        double dpH = MBAR_COUNT * MBAR_ITEM_H;

        // Panel shadow
        iSetColor(0,0,0);
        iFilledRectangle(dpX+4, dpY-4, dpW, dpH);
        // Panel background
        iSetColor(18,18,45);
        iFilledRectangle(dpX, dpY, dpW, dpH);
        iSetColor(60,80,160);
        iRectangle(dpX, dpY, dpW, dpH);

        for(int i=0;i<MBAR_COUNT;i++){
            double iy = dpY + (MBAR_COUNT-1-i)*MBAR_ITEM_H;
            bool hov = (menuBarHover == i);
            // hover highlight
            if(hov){ iSetColor(40,50,120); iFilledRectangle(dpX+2, iy+1, dpW-4, MBAR_ITEM_H-2); }
            // separator line
            if(i>0){ iSetColor(50,55,100); iLine(dpX+8, iy+MBAR_ITEM_H-1, dpX+dpW-8, iy+MBAR_ITEM_H-1); }
            // bullet dot
            iSetColor(hov ? 255:100, hov ? 210:140, hov ? 0:200);
            iFilledCircle(dpX+14, iy+MBAR_ITEM_H/2, 4);
            // label
            iSetColor(hov ? 255:200, hov ? 230:220, hov ? 80:255);
            drawStr(dpX+26, iy+10, MBAR_LABELS[i], GLUT_BITMAP_HELVETICA_18);
        }
    }

    // ---- Resources Collected sub-panel ----
    if(showResPanel && gPlayer){
        const Inventory& inv = gPlayer->getInventory();
        double rx=MBAR_DROP_W+10, ry=MBAR_Y - 180, rw=280, rh=170;
        // shadow
        iSetColor(0,0,0); iFilledRectangle(rx+4,ry-4,rw,rh);
        // panel
        iSetColor(10,25,10); iFilledRectangle(rx,ry,rw,rh);
        iSetColor(0,160,80); iRectangle(rx,ry,rw,rh);
        // title
        iSetColor(0,255,130); drawStr(rx+70,ry+rh-26,"RESOURCES COLLECTED",GLUT_BITMAP_HELVETICA_12);
        iSetColor(50,100,50); iLine(rx+8,ry+rh-30,rx+rw-8,ry+rh-30);

        const char* rnames[]={"Wood","Stone","Food","Metal"};
        int rvals[]={inv.getWood(),inv.getStone(),inv.getFood(),inv.getMetal()};
        int rmx[]={50,50,30,20};
        int rcr[]={160,140,200,210}; int rcg[]={100,140,40,130}; int rcb[]={30,140,40,20};
        for(int i=0;i<4;i++){
            double ly = ry + (3-i)*36 + 8;
            iSetColor(rcr[i],rcg[i],rcb[i]); iFilledCircle(rx+18,ly+8,7);
            iSetColor(255,255,255);
            drawStr(rx+30,ly,std::string(rnames[i])+": "+itos(rvals[i])+" / "+itos(rmx[i]),GLUT_BITMAP_HELVETICA_18);
            // mini bar
            double bw=80;
            iSetColor(40,40,40); iFilledRectangle(rx+rw-bw-10,ly+4,bw,12);
            double bf=(rmx[i]>0)?(bw*rvals[i]/rmx[i]):0;
            iSetColor(rcr[i],rcg[i],rcb[i]); iFilledRectangle(rx+rw-bw-10,ly+4,bf,12);
            iSetColor(120,120,120); iRectangle(rx+rw-bw-10,ly+4,bw,12);
        }
    }
}

// ================================================================
//  DRAW: Pause overlay (dim screen, keep game visible underneath)
// ================================================================
static void drawPauseOverlay(){
    // Semi-transparent dark tint — draw a dark panel across the whole screen
    iSetColor(0,0,0); iFilledRectangle(0,0,SW,MBAR_Y);
    // "PAUSED" text centred
    iSetColor(255,210,0);
    drawStr(SW/2-160,SH/2+20,"GAME PAUSED",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(160,180,220);
    drawStr(SW/2-230,SH/2-20,"Open the MENU bar above to Resume, Restart or Exit",GLUT_BITMAP_HELVETICA_18);
}

// ================================================================
//  DRAW: Menu
// ================================================================
static void drawMenu(){
    iSetColor(8,8,25); iFilledRectangle(0,0,SW,SH);
    iSetColor(255,255,255);
    unsigned int seed=7777;
    for(int i=0;i<130;i++){
        seed=seed*1103515245u+12345u; int sx=(int)((seed>>16)%SW);
        seed=seed*1103515245u+12345u; int sy=(int)((seed>>16)%SH);
        iFilledCircle(sx,sy,1);
    }
    iSetColor(255,210,0);
    drawStr(SW/2-270,SH-72,"ETERNAL QUEST: REBIRTH OF CIVILIZATION",GLUT_BITMAP_TIMES_ROMAN_24);
    // Subtitle banner — dark pill behind bright text so it always reads clearly
    iSetColor(0,40,80);    iFilledRectangle(SW/2-200,SH-118,400,26);
    iSetColor(0,100,180);  iRectangle(SW/2-200,SH-118,400,26);
    iSetColor(0,230,255);
    drawStr(SW/2-178,SH-112,"~ Survive.  Collect.  Conquer ~",GLUT_BITMAP_HELVETICA_18);
    iSetColor(55,55,100); iLine(40,SH-130,SW-40,SH-130);

    for(int i=0;i<5;i++){
        double cw=580,ch=76,cx=SW/2.0-cw/2,cy=SH-190.0-i*88;
        if(!zoneUnlocked[i])       iSetColor(26,26,26);
        else if(zoneCompleted[i])  iSetColor(14,44,14);
        else if(i==menuSel)        iSetColor(40,40,80);
        else                       iSetColor(24,24,54);
        iFilledRectangle(cx,cy,cw,ch);
        if(i==menuSel&&zoneUnlocked[i]) iSetColor(255,210,0);
        else                            iSetColor(55,55,100);
        iRectangle(cx,cy,cw,ch);
        if(!zoneUnlocked[i])       iSetColor(65,65,65);
        else if(zoneCompleted[i])  iSetColor(0,215,0);
        else if(i==menuSel)        iSetColor(255,210,0);
        else                       iSetColor(195,195,255);
        drawStr(cx+14,cy+ch/2+5,"["+itos(i+1)+"] "+ZONE_NAMES[i],GLUT_BITMAP_HELVETICA_18);
        if(zoneUnlocked[i]){
            std::string req="  Needs: ";
            if(ZONE_REQ[i][0]>0) req+="Wood:" +itos(ZONE_REQ[i][0])+"  ";
            if(ZONE_REQ[i][1]>0) req+="Stone:"+itos(ZONE_REQ[i][1])+"  ";
            if(ZONE_REQ[i][2]>0) req+="Food:" +itos(ZONE_REQ[i][2])+"  ";
            if(ZONE_REQ[i][3]>0) req+="Metal:"+itos(ZONE_REQ[i][3]);
            iSetColor(125,125,125); drawStr(cx+14,cy+12,req,GLUT_BITMAP_HELVETICA_12);
        }
        if(!zoneUnlocked[i]){
            iSetColor(48,48,48); iFilledRectangle(cx+cw-115,cy+ch/2-13,105,26);
            iSetColor(95,95,95); drawStr(cx+cw-108,cy+ch/2-6,"[ LOCKED ]",GLUT_BITMAP_HELVETICA_12);
        } else if(zoneCompleted[i]){
            iSetColor(0,85,0);   iFilledRectangle(cx+cw-125,cy+ch/2-13,115,26);
            iSetColor(0,245,75); drawStr(cx+cw-118,cy+ch/2-6,"[ COMPLETE ]",GLUT_BITMAP_HELVETICA_12);
        }
    }
    iSetColor(105,105,105);
    drawStr(SW/2-220,14,"Keys 1-5 = Select Zone    ENTER = Start    ESC = Exit",GLUT_BITMAP_HELVETICA_12);
}

// ================================================================
//  DRAW: Zone complete
// ================================================================
static void drawZoneCompleteScreen(){
    iSetColor(5,18,5); iFilledRectangle(0,0,SW,SH);
    iSetColor(255,255,100);
    unsigned int seed=55555;
    for(int i=0;i<80;i++){
        seed=seed*1103515245u+12345u; int sx=(int)((seed>>16)%SW);
        seed=seed*1103515245u+12345u; int sy=(int)((seed>>16)%SH);
        iFilledCircle(sx,sy,2);
    }
    drawPanel(SW/2-300,SH/2-140,600,280,14,48,14);
    double cx=SW/2.0,cy=SH/2.0+20;
    iSetColor(30,30,30);    iFilledRectangle(cx-11,cy,12,7); iFilledRectangle(cx+1,cy,12,7);
    iSetColor(40,40,120);   iFilledRectangle(cx-8,cy+7,8,14); iFilledRectangle(cx+1,cy+7,8,14);
    iSetColor(60,120,200);  iFilledRectangle(cx-10,cy+21,20,20);
    iSetColor(240,200,140); iFilledRectangle(cx-20,cy+28,10,8); iFilledRectangle(cx+10,cy+28,10,8);
                            iFilledRectangle(cx-20,cy+36,8,14); iFilledRectangle(cx+12,cy+36,8,14);
                            iFilledCircle(cx,cy+50,12);
    iSetColor(80,40,0);     iFilledCircle(cx,cy+60,9);
    iSetColor(255,215,0);
    drawStr(SW/2-210,SH/2+108,std::string(ZONE_NAMES[currentZoneIdx])+"  COMPLETE!",GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(215,255,215); drawStr(SW/2-195,SH/2-20,"All objectives achieved!  Great work!");
    if(currentZoneIdx<4){
        iSetColor(80,220,255);
        drawStr(SW/2-185,SH/2-58,std::string(ZONE_NAMES[currentZoneIdx+1])+" is now unlocked!");
    } else {
        iSetColor(255,215,0); drawStr(SW/2-170,SH/2-58,"ALL ZONES CLEARED!  YOU WIN!");
    }
    iSetColor(135,135,135); drawStr(SW/2-155,SH/2-108,"Press ENTER to return to zone select");
}

// ================================================================
//  GAME LOGIC
// ================================================================
static void startZone(int idx){
    delete gPlayer; delete gZone;
    currentZoneIdx=idx; playerDir=0;
    gEnemies.clear();
    enemySpawnTimer = 0;
    menuBarOpen  = false;
    showResPanel = false;
    // Place player on ground: x=centre, y=60 (feet on ground, ground occupies y=0..270)
    gPlayer=new Player(SW/2.0f, 60.0f);
    gZone  =new Zone(ZONE_KEYS[idx]);
    gameState=PLAYING;
}

static void checkZoneComplete(){
    if(!gPlayer) return;
    const Inventory& inv=gPlayer->getInventory();
    const int* req=ZONE_REQ[currentZoneIdx];
    if(inv.getWood()>=req[0]&&inv.getStone()>=req[1]&&
       inv.getFood()>=req[2]&&inv.getMetal()>=req[3]){
        zoneCompleted[currentZoneIdx]=true;
        if(currentZoneIdx<4) zoneUnlocked[currentZoneIdx+1]=true;
        gameState=ZONE_COMPLETE;
    }
}

// ================================================================
//  iGraphics CALLBACKS
// ================================================================
void iDraw(){
    iClear();
    if(gameState==MENU) drawMenu();
    else if(gameState==PLAYING || gameState==PAUSED){
        drawEnvironment(); drawResources();
        // Draw enemies behind player
        for(size_t i=0;i<gEnemies.size();i++){
            const Enemy& e=gEnemies[i];
            if(e.alive){
                drawEnemy(e.x, e.y, e.dir, e.walkFrame);
                drawEnemyHealthBar(e.x, e.y, e.health, e.maxHealth);
            }
        }
        if(gPlayer) drawCharacter(gPlayer->getX(),gPlayer->getY(),playerDir);
        drawInventoryHUD(); drawHealthHUD();
        drawObjectivesHUD(); drawZoneNameBar();
        // Controls hint at bottom of screen
        iSetColor(150,150,150);
        drawStr(8,8,"WASD / Arrow Keys = Move    Avoid Enemies!    F1 = Toggle Menu",GLUT_BITMAP_HELVETICA_12);
        // If paused draw dim overlay first, then bar on top
        if(gameState==PAUSED) drawPauseOverlay();
        // Menu bar always on top
        drawMenuBar();
    }
    else if(gameState==ZONE_COMPLETE) drawZoneCompleteScreen();
}

void fixedUpdate(){
    if(gameState!=PLAYING||!gPlayer||!gZone) return;
    const float SPD=5.0f; bool moved=false;
    if(isKeyPressed('w')||isKeyPressed('W')||isSpecialKeyPressed(GLUT_KEY_UP))
        {gPlayer->move(0,SPD); playerDir=1; moved=true;}
    if(isKeyPressed('s')||isKeyPressed('S')||isSpecialKeyPressed(GLUT_KEY_DOWN))
        {gPlayer->move(0,-SPD); playerDir=0; moved=true;}
    if(isKeyPressed('a')||isKeyPressed('A')||isSpecialKeyPressed(GLUT_KEY_LEFT))
        {gPlayer->move(-SPD,0); playerDir=2; moved=true;}
    if(isKeyPressed('d')||isKeyPressed('D')||isSpecialKeyPressed(GLUT_KEY_RIGHT))
        {gPlayer->move(SPD,0); playerDir=3; moved=true;}
    {
        float px=gPlayer->getX(),py=gPlayer->getY();
        // X: stay within screen edges
        if(px<20)      gPlayer->setPosition(20.0f,py);
        if(px>SW-20)   gPlayer->setPosition(SW-20.0f,py);
        // Y: stay on the ground (y=0..270). Keep feet inside ground strip.
        if(py<5)       gPlayer->setPosition(px,5.0f);    // don't go off bottom edge
        if(py>220)     gPlayer->setPosition(px,220.0f);  // don't float up into sky (above y=270)
    }
    if(moved){ gPlayer->collectNearbyResources(gZone->getResources()); checkZoneComplete(); }

    // ---- ENEMY UPDATE ----
    float px=gPlayer->getX(), py=gPlayer->getY();
    // Spawn enemies periodically (max 5 at once)
    enemySpawnTimer++;
    if(enemySpawnTimer >= enemySpawnInterval && (int)gEnemies.size() < 5) {
        enemySpawnTimer = 0;
        // Spawn at a random edge
        float ex, ey;
        int edge = rand()%4;
        if(edge==0){ ex=10.0f;      ey=(float)(rand()%200+10); }
        else if(edge==1){ ex=(float)(SW-10); ey=(float)(rand()%200+10); }
        else if(edge==2){ ex=(float)(rand()%SW); ey=5.0f; }
        else            { ex=(float)(rand()%SW); ey=215.0f; }
        gEnemies.push_back(Enemy(ex,ey));
    }

    // Update each enemy
    for(size_t i=0;i<gEnemies.size();i++){
        Enemy& e = gEnemies[i];
        if(!e.alive) continue;

        // Move toward player
        float dx = px - e.x;
        float dy = py - e.y;
        float dist = std::sqrt(dx*dx+dy*dy);

        // Set direction for sprite facing
        if(std::abs(dx)>std::abs(dy)){
            e.dir = (dx>0)?3:2;
        } else {
            e.dir = (dy>0)?1:0;
        }

        if(dist > 30.0f) {
            float nx = dx/dist * e.speed;
            float ny = dy/dist * e.speed;
            e.x += nx; e.y += ny;
            // Walk animation
            e.walkTimer++;
            if(e.walkTimer>12){ e.walkTimer=0; e.walkFrame^=1; }
        }

        // Clamp to play area
        if(e.x<10) e.x=10; if(e.x>SW-10) e.x=(float)(SW-10);
        if(e.y<5)  e.y=5;  if(e.y>220)   e.y=220.0f;

        // Attack if close
        if(e.attackTimer>0) e.attackTimer--;
        if(dist <= 35.0f && e.attackTimer==0) {
            gPlayer->takeDamage(e.damage);
            e.attackTimer = e.attackCooldown;
        }
    }

    // Remove dead enemies
    for(int i=(int)gEnemies.size()-1;i>=0;i--)
        if(!gEnemies[i].alive) gEnemies.erase(gEnemies.begin()+i);
}

void iKeyboard(unsigned char key){
    if(key==27){ // ESC
        if(gameState==PLAYING||gameState==PAUSED||gameState==ZONE_COMPLETE) gameState=MENU;
        else exit(0);
        menuBarOpen=false; showResPanel=false;
        return;
    }
    // F1 key (ASCII 0 via special) — handled in iSpecialKeyboard
    if(gameState==MENU){
        if(key>='1'&&key<='5'){int idx=key-'1'; if(zoneUnlocked[idx]) menuSel=idx;}
        if(key==13&&zoneUnlocked[menuSel]) startZone(menuSel);
    }
    else if(gameState==ZONE_COMPLETE){ if(key==13) gameState=MENU; }
}

// Hit-test helpers for menu bar
static bool inMenuBtn(int mx, int my){
    return mx>=0 && mx<MBAR_BTN_W && my>=MBAR_Y && my<MBAR_Y+MBAR_H;
}
static int hitDropItem(int mx, int my){
    if(!menuBarOpen) return -1;
    double dpX=MBAR_DROP_X, dpY=MBAR_Y - MBAR_COUNT*MBAR_ITEM_H;
    double dpW=MBAR_DROP_W, dpH=MBAR_COUNT*MBAR_ITEM_H;
    if(mx<dpX||mx>dpX+dpW||my<dpY||my>dpY+dpH) return -1;
    int row = (int)((my-dpY)/MBAR_ITEM_H);
    // items are drawn bottom-up: row 0 = bottom item = index MBAR_COUNT-1-0
    return MBAR_COUNT-1-row;
}

void iMouse(int button,int state,int mx,int my){
    // ---------- Main MENU (zone select screen) ----------
    if(gameState==MENU && button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        double cw=580,cx=SW/2.0-cw/2;
        for(int i=0;i<5;i++){
            double cy=SH-190.0-i*88;
            if(mx>=cx&&mx<=cx+cw&&my>=cy&&my<=cy+76)
                if(zoneUnlocked[i]){menuSel=i; startZone(i);}
        }
        return;
    }

    // ---------- In-game menu bar ----------
    if((gameState==PLAYING||gameState==PAUSED) && button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        // Click on the MENU button toggles dropdown
        if(inMenuBtn(mx,my)){
            menuBarOpen = !menuBarOpen;
            showResPanel = false;
            // Opening the menu pauses; closing resumes
            if(menuBarOpen) gameState=PAUSED;
            else             gameState=PLAYING;
            return;
        }
        // Click on a dropdown item
        int item = hitDropItem(mx,my);
        if(item>=0){
            showResPanel = false;
            switch(item){
                case 0: // New Game — restart current zone
                    startZone(currentZoneIdx);
                    break;
                case 1: // Continue — close menu and resume
                    menuBarOpen=false;
                    gameState=PLAYING;
                    break;
                case 2: // Resources Collected — show sub-panel
                    showResPanel = true;
                    break;
                case 3: // Exit to Main Menu
                    menuBarOpen=false;
                    showResPanel=false;
                    gameState=MENU;
                    break;
            }
            if(item!=2) menuBarOpen=(item==2); // keep open only for resources
            return;
        }
        // Click anywhere else while open → close menu and resume
        if(menuBarOpen){
            menuBarOpen=false;
            showResPanel=false;
            gameState=PLAYING;
        }
    }
}

void iMouseMove(int mx,int my){
    // Update hover state for menu bar dropdown
    if((gameState==PLAYING||gameState==PAUSED)&&menuBarOpen){
        int item=hitDropItem(mx,my);
        menuBarHover=item;
        if(inMenuBtn(mx,my)) menuBarHover=-2;
    } else { menuBarHover=-1; }
}
void iPassiveMouseMove(int mx,int my){
    if((gameState==PLAYING||gameState==PAUSED)&&menuBarOpen){
        int item=hitDropItem(mx,my);
        menuBarHover=item;
        if(inMenuBtn(mx,my)) menuBarHover=-2;
    } else { menuBarHover=-1; }
}

int main(){
    srand((unsigned int)time(NULL));
    iInitialize(SW,SH,"Eternal Quest: Rebirth of Civilization");
    iStart();
    return 0;
}
