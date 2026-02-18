
#define _CRT_SECURE_NO_WARNINGS

#include "iGraphics.h"
#include <cstdlib>
#include <ctime>
#include <stdio.h>

// --- RESOLUTION SETTINGS ---
#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

#define PLAYER_SPEED 15
#define RESOURCE_COUNT 8

#define MAX_HEALTH 100
#define MAX_ENERGY 100
#define ENEMY_HEALTH 50

// ---------------- IMAGE IDs ----------------
int playerImg, enemyImg, bgImg, menuBgImg;
int resImgs[4];

// ---------------- GAME STATES ----------------
enum GameState { MAIN_MENU, INSTRUCTIONS, FOREST_ZONE };
GameState currentState = MAIN_MENU;

// ---------------- PLAYER & ENEMY ----------------
int charX = 600, charY = 200;
int playerAttack = 20;

int enemyX = 200, enemyY = 400;
int enemyWidth = 60, enemyHeight = 110;
int enemySpeed = 3;
int enemyHealth = ENEMY_HEALTH;

// ---------------- RESOURCES ----------------
struct Resource {
	int x, y, width, height, type;
	bool collected;
	int respawnTimer;
};

Resource resources[RESOURCE_COUNT];

enum ResourceType { WOOD = 0, STONE, FOOD, METAL };

// ---------------- STATS & INVENTORY ----------------
int playerHealth = MAX_HEALTH;
int playerEnergy = MAX_ENERGY;

int inventoryWood = 0, inventoryStone = 0;
int inventoryFood = 0, inventoryMetal = 0;

bool gameOver = false;
bool victory = false;

// ===================================================
// ✅ SAVE & LOAD SYSTEM (Fixed fopen issue)
// ===================================================

void saveGame() {
	FILE* fp;
	fopen_s(&fp, "savegame.txt", "w");

	if (fp != NULL) {
		fprintf(fp, "%d %d %d %d %d %d %d",
			charX, charY, playerHealth,
			inventoryWood, inventoryStone,
			inventoryFood, inventoryMetal);

		fclose(fp);
		printf("Game Saved!\n");
	}
}

void loadGame() {
	FILE* fp;
	fopen_s(&fp, "savegame.txt", "r");

	if (fp != NULL) {
		fscanf(fp, "%d %d %d %d %d %d %d",
			&charX, &charY, &playerHealth,
			&inventoryWood, &inventoryStone,
			&inventoryFood, &inventoryMetal);

		fclose(fp);

		currentState = FOREST_ZONE;
		printf("Game Loaded!\n");
	}
	else {
		printf("Save file nahi mili!\n");
	}
}

// ===================================================
// RESOURCE SPAWN
// ===================================================

void spawnResources() {
	srand(time(0));

	for (int i = 0; i < RESOURCE_COUNT; i++) {
		resources[i].x = rand() % (SCREEN_WIDTH - 100);
		resources[i].y = rand() % (SCREEN_HEIGHT - 200);

		resources[i].width = 40;
		resources[i].height = 40;

		resources[i].collected = false;
		resources[i].type = rand() % 4;

		resources[i].respawnTimer = 0;
	}
}

// ===================================================
// RESET GAME
// ===================================================

void resetGame() {
	charX = 600;
	charY = 200;

	playerHealth = MAX_HEALTH;
	playerEnergy = MAX_ENERGY;

	inventoryWood = 0;
	inventoryStone = 0;
	inventoryFood = 0;
	inventoryMetal = 0;

	enemyHealth = ENEMY_HEALTH;
	enemyX = 200;
	enemyY = 400;

	gameOver = false;
	victory = false;

	spawnResources();
}

// ===================================================
// LOAD ASSETS
// ===================================================

void loadAssets() {

	// ⚠️ Best: Use BMP files for iGraphics
	bgImg = iLoadImage("Images\\forestbg.bmp");
	menuBgImg = iLoadImage("Images\\bg1.bmp");

	playerImg = iLoadImage("Images\\player.bmp");
	enemyImg = iLoadImage("Images\\enemy.bmp");

	resImgs[0] = iLoadImage("Images\\wood.bmp");
	resImgs[1] = iLoadImage("Images\\stone.bmp");
	resImgs[2] = iLoadImage("Images\\food.bmp");
	resImgs[3] = iLoadImage("Images\\metal.bmp");
}

// ===================================================
// INVENTORY UPDATE
// ===================================================

void updateInventory(Resource& res) {

	if (res.type == WOOD) inventoryWood++;
	if (res.type == STONE) inventoryStone++;
	if (res.type == FOOD) inventoryFood++;
	if (res.type == METAL) inventoryMetal++;

	res.collected = true;
	res.respawnTimer = 300;
}

// ===================================================
// ENEMY MOVEMENT
// ===================================================

void moveEnemy() {

	if (currentState != FOREST_ZONE || enemyHealth <= 0)
		return;

	if (enemyX < charX) enemyX += enemySpeed;
	else if (enemyX > charX) enemyX -= enemySpeed;

	if (enemyY < charY) enemyY += enemySpeed;
	else if (enemyY > charY) enemyY -= enemySpeed;
}

// ===================================================
// RESOURCE RESPAWN
// ===================================================

void respawnResources() {

	for (int i = 0; i < RESOURCE_COUNT; i++) {
		if (resources[i].collected) {

			resources[i].respawnTimer--;

			if (resources[i].respawnTimer <= 0) {
				resources[i].collected = false;

				resources[i].x = rand() % (SCREEN_WIDTH - 100);
				resources[i].y = rand() % (SCREEN_HEIGHT - 200);
			}
		}
	}
}

// ===================================================
// DRAW MENU
// ===================================================

void drawMainMenu() {

	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, menuBgImg);

	iSetColor(255, 255, 255);
	iText(SCREEN_WIDTH / 2 - 140, 600,
		"ETERNAL QUEST HD", GLUT_BITMAP_TIMES_ROMAN_24);

	iSetColor(0, 200, 255);
	iFilledRectangle(SCREEN_WIDTH / 2 - 80, 450, 160, 45);
	iSetColor(0, 0, 0);
	iText(SCREEN_WIDTH / 2 - 35, 465, "START");

	iSetColor(0, 255, 100);
	iFilledRectangle(SCREEN_WIDTH / 2 - 80, 380, 160, 45);
	iSetColor(0, 0, 0);
	iText(SCREEN_WIDTH / 2 - 35, 395, "LOAD");

	iSetColor(200, 200, 200);
	iFilledRectangle(SCREEN_WIDTH / 2 - 80, 310, 160, 45);
	iSetColor(0, 0, 0);
	iText(SCREEN_WIDTH / 2 - 60, 325, "HOW TO PLAY");

	iSetColor(255, 50, 50);
	iFilledRectangle(SCREEN_WIDTH / 2 - 80, 240, 160, 45);
	iSetColor(255, 255, 255);
	iText(SCREEN_WIDTH / 2 - 20, 255, "EXIT");
}

// ===================================================
// DRAW INSTRUCTIONS
// ===================================================

void drawInstructions() {

	iSetColor(20, 20, 20);
	iFilledRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	iSetColor(255, 255, 255);
	iText(SCREEN_WIDTH / 2 - 120, 600,
		"INSTRUCTIONS", GLUT_BITMAP_TIMES_ROMAN_24);

	iText(500, 500, "- Arrow Keys: Move");
	iText(500, 450, "- Space: Attack Enemy");
	iText(500, 400, "- Press S: Save Game");
	iText(500, 350, "- Collect 15 Resources to Win");

	iSetColor(255, 255, 0);
	iText(500, 200, "Press M for Main Menu");
}

// ===================================================
// DRAW FOREST ZONE
// ===================================================

void drawForestZone() {

	iShowImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bgImg);

	iShowImage(charX, charY, 70, 130, playerImg);

	if (enemyHealth > 0)
		iShowImage(enemyX, enemyY, 80, 130, enemyImg);

	for (int i = 0; i < RESOURCE_COUNT; i++) {
		if (!resources[i].collected)
			iShowImage(resources[i].x, resources[i].y,
			40, 40, resImgs[resources[i].type]);
	}

	// Health bar
	iSetColor(255, 0, 0);
	iFilledRectangle(20, SCREEN_HEIGHT - 50,
		playerHealth * 2.5, 25);

	iSetColor(255, 255, 255);
	iRectangle(20, SCREEN_HEIGHT - 50,
		MAX_HEALTH * 2.5, 25);

	char buf[200];
	sprintf_s(buf, "W:%d S:%d F:%d M:%d",
		inventoryWood, inventoryStone,
		inventoryFood, inventoryMetal);

	iText(SCREEN_WIDTH - 350, SCREEN_HEIGHT - 45,
		buf, GLUT_BITMAP_HELVETICA_18);

	if (inventoryWood + inventoryStone + inventoryFood + inventoryMetal >= 15)
		victory = true;
}

// ===================================================
// MAIN DRAW
// ===================================================

void iDraw() {

	iClear();

	if (currentState == MAIN_MENU)
		drawMainMenu();

	else if (currentState == INSTRUCTIONS)
		drawInstructions();

	else if (currentState == FOREST_ZONE) {

		if (victory) {
			iSetColor(0, 255, 0);
			iText(600, 400, "VICTORY! Press M",
				GLUT_BITMAP_TIMES_ROMAN_24);
		}
		else
			drawForestZone();
	}
}

// ===================================================
// KEYBOARD INPUT
// ===================================================

void iKeyboard(unsigned char key) {

	if (key == 'm' || key == 'M')
		currentState = MAIN_MENU;

	if (currentState == FOREST_ZONE) {

		if (key == 's' || key == 'S')
			saveGame();

		if (key == ' ') {
			if (abs(charX - enemyX) < 100)
				enemyHealth -= playerAttack;
		}
	}
}

// ===================================================
// SPECIAL KEYS
// ===================================================

void iSpecialKeyboard(unsigned char key) {

	if (currentState != FOREST_ZONE) return;

	if (key == GLUT_KEY_UP) charY += PLAYER_SPEED;
	if (key == GLUT_KEY_DOWN) charY -= PLAYER_SPEED;
	if (key == GLUT_KEY_LEFT) charX -= PLAYER_SPEED;
	if (key == GLUT_KEY_RIGHT) charX += PLAYER_SPEED;

	// Collision with resources
	for (int i = 0; i < RESOURCE_COUNT; i++) {

		if (!resources[i].collected) {

			if (charX < resources[i].x + 40 &&
				charX + 70 > resources[i].x &&
				charY < resources[i].y + 40 &&
				charY + 130 > resources[i].y) {

				updateInventory(resources[i]);
			}
		}
	}
}

// ===================================================
// MOUSE INPUT
// ===================================================

void iMouse(int button, int state, int mx, int my) {

	if (state == GLUT_DOWN && currentState == MAIN_MENU) {

		// START
		if (mx >= SCREEN_WIDTH / 2 - 80 &&
			mx <= SCREEN_WIDTH / 2 + 80 &&
			my >= 450 && my <= 495) {

			resetGame();
			currentState = FOREST_ZONE;
		}

		// LOAD
		if (mx >= SCREEN_WIDTH / 2 - 80 &&
			mx <= SCREEN_WIDTH / 2 + 80 &&
			my >= 380 && my <= 425) {

			loadGame();
		}

		// INSTRUCTIONS
		if (mx >= SCREEN_WIDTH / 2 - 80 &&
			mx <= SCREEN_WIDTH / 2 + 80 &&
			my >= 310 && my <= 355) {

			currentState = INSTRUCTIONS;
		}

		// EXIT
		if (mx >= SCREEN_WIDTH / 2 - 80 &&
			mx <= SCREEN_WIDTH / 2 + 80 &&
			my >= 240 && my <= 285) {

			exit(0);
		}
	}
}

void iMouseMove(int mx, int my) {}
void iPassiveMouseMove(int mx, int my) {}

// ===================================================
// TIMER
// ===================================================

void timers() {
	moveEnemy();
	respawnResources();
}

// ===================================================
// MAIN FUNCTION
// ===================================================

int main() {

	iInitialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Eternal Quest HD");

	loadAssets();
	spawnResources();

	iSetTimer(50, timers);

	iStart();
	return 0;
}
