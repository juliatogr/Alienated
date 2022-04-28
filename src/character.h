
#ifndef CHARACTER_H
#define CHARACTER_H

# include "utils.h"
#include "image.h"

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

enum SHOT_DIR {
	TO_RIGHT = 0,
	TO_LEFT = 1,
	TO_DOWN = 2

};

struct eShot {
	Vector2 pos;
	float moveSpeed;
	SHOT_DIR dir;
};

enum PLAYER_TYPE {
	PFRONT = 0,
	PRIGHT = 1,
	PDOWN = 2,
};

class Player
{
public:
	Vector2 pos;
	PLAYER_TYPE type;
	
	int n_lives = 3;
	float moveSpeed = 50.0f;
	float animSpeed = 4.0f;
	float jumpSpeed = 40.0f;

	int min_y = 73;
	int max_y = 104;

	bool isJumping;
	bool isFalling;
	bool isShooting;
	std::vector<eShot> shots;

	// Constructors
	Player();
	Player(unsigned int x, unsigned int y);
	void addShot();
	void renderShots(Image& framebuffer);
	void deleteShots();
	bool hasWon();
	void moveShots(double seconds_elapsed);
};

enum eCellType : uint8 {
	ROBOT = 1,
	LITTLE_ALIEN = 3,
	BIG_ALIEN = 6,
	OVNI = 9,
	HEART = 10,
};

enum ENEMY_DIR {
	EFRONT = 0,
	ELEFT = 1
};

enum ENEMY_COLOR {
	GREEN = 0,
	BLUE = 1,
	ORANGE = 2,
	RED = 3
};

enum OVNI_TYPE {
	LITTLE = 0,
	BIG = 1
};

enum HEART_TYPE {
	FULL = 0,
	EMPTY = 1
};
class Enemy
{
public:
	Vector2 pos;
	eCellType type;
	ENEMY_DIR dir;
	ENEMY_COLOR color;

	float live_percentage = 100;
	float moveSpeed = 1.0f;
	float animSpeed = 4.0f;

	bool isMoving;
	bool isShooting;

	float damage_reductor;

	std::vector<eShot> shots;

	// Constructors
	Enemy();
	Enemy(Vector2 pos, eCellType type);
	void addShot();
	void renderShots(Image& framebuffer, float time);
	void deleteShots();
	void moveShots(double seconds_elapsed);
};
#endif
