#ifndef WORLD_H
#define WORLD_H

#include "utils.h"
#include "input.h"
#include "image.h"
#include "character.h"
#include "background.h"
#include <cmath>

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

struct sCell {
	eCellType type;
};

class GameMap {
public:
	int width;
	int height;
	sCell* data;
	bool show_monsters;

	GameMap()
	{
		width = height = 0;
		data = NULL;
	}

	GameMap(int w, int h)
	{
		width = w;
		height = h;
		data = new sCell[w * h];
		show_monsters = false;
	}

	sCell& getCell(int x, int y)
	{
		return data[x + y * width];
	}

};

struct sMapHeader {
	int w; //width of map
	int h; //height of map
	unsigned char bytes; //num bytes per cell
	unsigned char extra[7]; //filling bytes, not used
};


class World {
public:
	Image tileset;

	Player* player;
	Player* tutplayer;
	GameMap* map;
	Background* bg;
	Image font;
	Image minifont;
	std::vector<int> n_enemies;
	int cellsize = 16;
	int level = 0;
	int n_levels = 7;

	std::vector<std::vector<Vector2>> enemySpawnPositions;
	std::vector <std::vector<eCellType>> enemySpawnTypes;
	std::vector <std::vector<Enemy*>> enemies;

	World(){};
	void loadGameMap(const char* filename);
	void spawnEnemies();

	void renderBackground(Image& framebuffer);
	void renderPlayer(Image& framebuffer, float time);
	void renderEnemies(Image& framebuffer, float time);
	void renderShots(Image& framebuffer, float time);
	void renderLives(Image& framebuffer);

	void renderTutPlayer(Image& framebuffer, float time);
	void renderTutEnemies(Image& framebuffer, float time);
	void renderTutShots(Image& framebuffer, float time);
	void renderTutLives(Image& framebuffer);

	void moveEnemies(float& movementx);
	void moveTutEnemies(float& movementx);
	void moveShots(double seconds_elapsed);
	void moveTutShots(double seconds_elapsed);
	void moveWorld(Vector2 bg_movements[4], float& movementx);
	void changeEnemiesPos(Vector2 movement, float seconds_elapsed);
	void changeTutEnemiesPos(Vector2 movement, float seconds_elapsed);
	void checkEnemyMoving(Vector2 movement);
	void checkTutEnemyMoving(Vector2 movement);

	void addEnemyShots(float time);
	void deleteShots();
	void checkEnemyPassed();
	void checkTutEnemyPassed();
	void checkDamages();
	void checkTutDamages();

	bool hasEndedLevel();
	bool hasEndedGame();
	bool hasEndedTutorial();

};
bool isValidPos(Vector2 pos);
bool isHitShot(Vector2 shotpos, Vector2 objpos);



#endif
