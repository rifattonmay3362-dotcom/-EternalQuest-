#include "iGraphics.h"
#include "globals.h"
#include "player.h"
#include "enemy.h"
#include "boss.h"
#include "resource.h"
#include "projectile.h"
#include "renderer.h"
#include "save.h"
#include <math.h>

/* ============================================================
GLOBAL VARIABLE DEFINITIONS
============================================================ */
int    gameState = STATE_MENU;
int    animTick = 0;
int    dayPhase = 0;
float  dayTimer = 0.0f;
bool   bossDefeated = false;

int keyLeft = 0, keyRight = 0, keyUp = 0, keyDown = 0;

/*=======================================Enemy Logic========================================================*/
Enemy enemies[MAX_ENEMIES];
Enemy::Enemy() { x = 0; y = 0; hp = ENEMY_MAX_HP; alive = true; dir = -1; attackTimer = 0; }
void Enemy::init(float px, float py) { x = px; y = py; hp = ENEMY_MAX_HP; alive = true; dir = -1; attackTimer = 0; }
void initEnemies() {
	float sx[MAX_ENEMIES] = { 400, 520, 620, 450, 580, 680 };
	float sy[MAX_ENEMIES] = { 200, 280, 180, 350, 130, 300 };
	for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].init(sx[i], sy[i]);
}
void Enemy::update(int nightBonus) {
	if (!alive) return;
	float spd = ENEMY_SPEED + nightBonus;
	if (x < player.x) { x += spd; dir = 1; }
	else { x -= spd; dir = -1; }
	if (y < player.y) y += spd * 0.5f; else y -= spd * 0.5f;
	attackTimer -= 0.016f;
	if (collides(x, y, ENEMY_W, ENEMY_H, player.x, player.y, PLAYER_W, PLAYER_H)) {
		if (attackTimer <= 0 && player.invincTimer <= 0) {
			player.hp -= 8; player.invincTimer = 1.0f; attackTimer = 1.2f;
			if (player.hp <= 0) { player.hp = 0; gameState = STATE_GAMEOVER; }
		}
	}
}
void Enemy::draw() {
	if (!alive) return;
	iSetColor(45, 125, 45); iFilledRectangle(x + 6, y + 8, 24, 22);
	iSetColor(65, 150, 65); iFilledCircle(x + 18, y + 37, 10);
	iSetColor(255, 35, 35); iFilledCircle(x + 13, y + 39, 2); iFilledCircle(x + 23, y + 39, 2);
	iSetColor(75, 0, 0); iFilledRectangle(x, y + 48, ENEMY_W, 4);
	iSetColor(180, 0, 0); iFilledRectangle(x, y + 48, (int)(ENEMY_W * hp / (float)ENEMY_MAX_HP), 4);
}

/*=======================================Boss Logic========================================================*/
Boss boss;
Boss::Boss() { reset(); }
void Boss::reset() { x = 680; y = 220; hp = BOSS_MAX_HP; alive = true; dir = -1; attackTimer = 0; phase = 1; }
void Boss::update(int nightBonus, bool allEnemiesDead) {
	if (!alive || !allEnemiesDead) return;
	float spd = BOSS_SPEED + nightBonus + (phase == 2 ? 1 : 0);
	if (x < player.x) { x += spd; dir = 1; }
	else { x -= spd; dir = -1; }
	if (y < player.y) y += spd * 0.4f; else y -= spd * 0.4f;
	if (hp <= BOSS_MAX_HP / 2) phase = 2;
	attackTimer -= 0.016f;
	if (collides(x, y, BOSS_W, BOSS_H, player.x, player.y, PLAYER_W, PLAYER_H)) {
		if (attackTimer <= 0 && player.invincTimer <= 0) {
			player.hp -= (phase == 2 ? 20 : 12); player.invincTimer = 0.8f; attackTimer = 1.0f;
			if (player.hp <= 0) { player.hp = 0; gameState = STATE_GAMEOVER; }
		}
	}
}
void Boss::draw() {
	if (!alive) return;
	int r = (phase == 2) ? 210 : 110;
	iSetColor(r, 80, 38); iFilledRectangle(x + 10, y + 15, 50, 45);
	iSetColor(r, 100, 50); iFilledCircle(x + 35, y + 73, 22);
	iSetColor(180, 0, 0); iFilledRectangle(x - 8, y + 118, (int)((BOSS_W + 16) * hp / (float)BOSS_MAX_HP), 10);
}

/*=======================================Player Logic========================================================*/
Player player;
Player::Player() { reset(); }
void Player::reset() { x = 100; y = 200; hp = PLAYER_MAX_HP; dir = 1; wood = stone = food = 0; attacking = false; attackTimer = invincTimer = 0; }
void Player::update() {
	float nx = x, ny = y;
	if (keyLeft) nx -= PLAYER_SPEED; if (keyRight) nx += PLAYER_SPEED;
	if (keyUp) ny += PLAYER_SPEED; if (keyDown) ny -= PLAYER_SPEED;
	if (nx < 0) nx = 0; if (nx > SCREEN_W - PLAYER_W) nx = SCREEN_W - PLAYER_W;
	if (ny < 0) ny = 0; if (ny > SCREEN_H - PLAYER_H - 35) ny = SCREEN_H - PLAYER_H - 35;
	x = nx; y = ny;
	if (attacking) { attackTimer -= 0.016f; if (attackTimer <= 0) attacking = false; }
	if (invincTimer > 0) invincTimer -= 0.016f;
}
void Player::shoot() { if (!attacking) { attacking = true; attackTimer = 0.25f; spawnProjectile(x + (dir == 1 ? PLAYER_W : -PROJ_W), y + 22, dir); } }
void Player::draw() {
	if (invincTimer > 0 && (animTick / 4) % 2 == 0) return;
	iSetColor(60, 120, 200); iFilledRectangle(x + 10, y + 12, 20, 24);
	iSetColor(220, 170, 120); iFilledCircle(x + 20, y + 46, 11);
	iSetColor(0, 200, 0); iFilledRectangle(x, y + 62, (int)(PLAYER_W * hp / (float)PLAYER_MAX_HP), 6);
}

/*=======================================Refurbished Forest Zone==============================================*/
void drawRefurbishedForest() {
	if (dayPhase == 0) {
		for (int i = 0; i < 400; i++) { iSetColor(95 + i / 10, 180 + i / 20, 255); iLine(0, 200 + i, SCREEN_W, 200 + i); }
		iSetColor(255, 255, 255); int cloudOffset = (animTick / 2) % (SCREEN_W + 200);
		iFilledCircle(cloudOffset - 100, 500, 30); iFilledCircle(cloudOffset - 70, 505, 35);
	}
	else {
		for (int i = 0; i < 400; i++) { iSetColor(5, 5, 38 - i / 20 < 0 ? 0 : 38 - i / 20); iLine(0, 200 + i, SCREEN_W, 200 + i); }
		iSetColor(255, 255, 200); iFilledCircle(700, 550, 25); iSetColor(5, 5, 30); iFilledCircle(715, 555, 22);
	}
	iSetColor(20, 45, 25);
	for (int i = -50; i<SCREEN_W; i += 200) { double px[] = { (double)i, (double)i + 100, (double)i + 200 }, py[] = { 200, 380, 200 }; iFilledPolygon(px, py, 3); }
	iSetColor(34, 85, 20); iFilledRectangle(0, 0, SCREEN_W, 205);
	iSetColor(148, 116, 66); iFilledRectangle(0, 125, SCREEN_W, 45);
	for (int tx = 20; tx < SCREEN_W; tx += 150) {
		iSetColor(15, 45, 10); iFilledEllipse(tx + 50, 200, 40, 15);
		iSetColor(70, 40, 10); iFilledRectangle(tx + 42, 200, 16, 60);
		for (int layer = 0; layer<3; layer++) {
			iSetColor(20 + layer * 20, 100 + layer * 20, 20);
			double px[] = { (double)tx, (double)tx + 50, (double)tx + 100 }, py[] = { 250.0 + (double)layer*30.0, 320.0 + (double)layer*30.0, 250.0 + (double)layer*30.0 };
			iFilledPolygon(px, py, 3);
		}
	}
}

/*=======================================Standard Rendering====================================================*/
void drawMenu() { iSetColor(20, 20, 70); iFilledRectangle(0, 0, SCREEN_W, SCREEN_H); iSetColor(255, 255, 255); iText(320, 500, "ETERNAL QUEST", GLUT_BITMAP_TIMES_ROMAN_24); iSetColor(0, 150, 0); iFilledRectangle(300, 360, 200, 45); iSetColor(255, 255, 255); iText(350, 378, "START GAME", GLUT_BITMAP_HELVETICA_18); }
void drawHUD() { iSetColor(0, 0, 0); iFilledRectangle(0, 0, SCREEN_W, 35); char buf[64]; sprintf_s(buf, "HP: %d | Wood: %d | Stone: %d", player.hp, player.wood, player.stone); iSetColor(255, 255, 255); iText(15, 14, buf, GLUT_BITMAP_HELVETICA_12); }

/*=======================================Projectiles & Resources===============================================*/
Projectile projs[MAX_PROJ];
Projectile::Projectile() { x = 0; y = 0; vx = 0; active = false; } // Linker Fix
void initProjectiles() { for (int i = 0; i < MAX_PROJ; i++) projs[i].active = false; }
void spawnProjectile(float x, float y, int dir) { for (int i = 0; i < MAX_PROJ; i++) if (!projs[i].active) { projs[i].x = x; projs[i].y = y; projs[i].vx = (float)dir * PROJ_SPEED; projs[i].active = true; break; } }
void updateProjectiles() {
	for (int i = 0; i < MAX_PROJ; i++) {
		if (!projs[i].active) continue; projs[i].x += projs[i].vx;
		if (projs[i].x < -20 || projs[i].x > SCREEN_W + 20) projs[i].active = false;
		for (int j = 0; j < MAX_ENEMIES; j++) if (enemies[j].alive && projs[i].active && collides(projs[i].x, projs[i].y, PROJ_W, PROJ_H, enemies[j].x, enemies[j].y, ENEMY_W, ENEMY_H)) { enemies[j].hp -= 20; projs[i].active = false; if (enemies[j].hp <= 0) enemies[j].alive = false; }
	}
}
void drawProjectiles() { iSetColor(255, 255, 0); for (int i = 0; i < MAX_PROJ; i++) if (projs[i].active) iFilledRectangle(projs[i].x, projs[i].y, PROJ_W, PROJ_H); }

Resource resources[MAX_RESOURCES];
Resource::Resource() { x = 0; y = 0; type = 0; collected = false; } // Linker Fix
void initResources() { for (int i = 0; i < MAX_RESOURCES; i++) { resources[i].x = 100 + i * 80; resources[i].y = 150 + (i % 2) * 50; resources[i].collected = false; } }
void checkResourcePickup() { for (int i = 0; i < MAX_RESOURCES; i++) if (!resources[i].collected && collides(player.x, player.y, PLAYER_W, PLAYER_H, resources[i].x, resources[i].y, RES_W, RES_H)) { resources[i].collected = true; player.wood++; } }

/*=======================================System Functions======================================================*/
bool collides(float ax, float ay, int aw, int ah, float bx, float by, int bw, int bh) { return (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by); }
void initLevel() { bossDefeated = false; dayTimer = 0; dayPhase = 0; animTick = 0; player.reset(); initEnemies(); boss.reset(); initResources(); initProjectiles(); }
void iDraw() {
	iClear(); animTick++;
	switch (gameState) {
	case STATE_MENU: drawMenu(); break;
	case STATE_PLAYING: drawRefurbishedForest(); for (int i = 0; i < MAX_RESOURCES; i++) if (!resources[i].collected) { iSetColor(200, 150, 0); iFilledCircle(resources[i].x, resources[i].y, 8); } drawProjectiles(); for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].draw(); boss.draw(); player.draw(); drawHUD(); break;
	case STATE_GAMEOVER: iSetColor(255, 0, 0); iText(350, 300, "GAME OVER", GLUT_BITMAP_TIMES_ROMAN_24); break;
	}
}
void updateGame() {
	if (gameState != STATE_PLAYING) return;
	dayTimer += 0.016f; if (dayPhase == 0 && dayTimer > 30.0f) { dayPhase = 1; dayTimer = 0; } if (dayPhase == 1 && dayTimer > 20.0f) { dayPhase = 0; dayTimer = 0; }
	player.update(); updateProjectiles(); checkResourcePickup();
	for (int i = 0; i < MAX_ENEMIES; i++) enemies[i].update(dayPhase == 1 ? 1 : 0);
	bool allDead = true; for (int i = 0; i < MAX_ENEMIES; i++) if (enemies[i].alive) allDead = false;
	boss.update(dayPhase == 1 ? 1 : 0, allDead);
}

void iMouseMove(int mx, int my) {}
void iPassiveMouseMove(int mx, int my) {}
void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (gameState == STATE_MENU && mx >= 300 && mx <= 500 && my >= 360 && my <= 405) { initLevel(); gameState = STATE_PLAYING; }
	}
}
void iKeyboard(unsigned char key) { if (key == ' ' && gameState == STATE_PLAYING) player.shoot(); if ((key == 'm' || key == 'M')) gameState = STATE_MENU; }
void iSpecialKeyboard(unsigned char key) { if (key == GLUT_KEY_LEFT) keyLeft = 1; if (key == GLUT_KEY_RIGHT) keyRight = 1; if (key == GLUT_KEY_UP) keyUp = 1; if (key == GLUT_KEY_DOWN) keyDown = 1; }
void iSpecialKeyboardUp(unsigned char key) { if (key == GLUT_KEY_LEFT) keyLeft = 0; if (key == GLUT_KEY_RIGHT) keyRight = 0; if (key == GLUT_KEY_UP) keyUp = 0; if (key == GLUT_KEY_DOWN) keyDown = 0; }
void fixedUpdate() {}

int main() {
	iInitialize(SCREEN_W, SCREEN_H, "Eternal Quest");
	iSetTimer(16, updateGame);
	iStart();
	return 0;
}