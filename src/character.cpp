#include "character.h"
#include "image.h"

Player::Player() {
	pos.x = 10;
	pos.y = max_y;
	type = PLAYER_TYPE::PRIGHT;
	isJumping = false;
	isFalling = false;
	isShooting = false; 
}

Player::Player(unsigned int x, unsigned int y) {
	this->pos.x = x;
	this->pos.y = y;
	isJumping = false; 
	isFalling = false;
	isShooting = false;
	type = PLAYER_TYPE::PRIGHT;
}
void Player::addShot() {

	eShot shot;

	shot.pos.x = this->pos.x+5;
	shot.pos.y = this->pos.y-2;

	if (this->type == PLAYER_TYPE::PDOWN) {
		shot.pos.y = this->pos.y + 4;
	}
	shot.dir = SHOT_DIR::TO_RIGHT;
	shot.moveSpeed = 60.0f;

	this->shots.push_back(shot);
}

void Player::renderShots(Image& framebuffer) {
	int n_shots = this->shots.size();
	for (int i = 0; i < n_shots; i++) {
		framebuffer.setPixel(this->shots[i].pos.x, this->shots[i].pos.y, Color::YELLOW);
	}
}

void Player::deleteShots() {
	this->shots.clear();
}

void Player::moveShots(double seconds_elapsed) {
	int n_shots = this->shots.size();
	for (int i = 0; i < n_shots; i++) {
		this->shots[i].pos.x += this->shots[i].moveSpeed * seconds_elapsed;
		if (this->shots[i].pos.x > 160) {
			this->shots.erase(this->shots.begin()+i);
			i--;
			n_shots--;
		}
	}
}
bool Player::hasWon() {
	if (n_lives > 0) {
		return true;
	}
	return false;
}

Enemy::Enemy() {
	pos.x = 10; pos.y = 104;
	this->type = eCellType::LITTLE_ALIEN;
	this->dir = ENEMY_DIR::EFRONT;
	this->color = ENEMY_COLOR::GREEN;
	isMoving = false;
	isShooting = false;
}
Enemy::Enemy(Vector2 pos, eCellType type) {

	this->pos.x = pos.x;
	this->pos.y = pos.y;
	isMoving = false;
	this->type = type;
	isShooting = false;

	if (type % 16 == eCellType::OVNI) {
		this->dir = ENEMY_DIR::EFRONT;
		this->damage_reductor = 125.0 / float(int(type) % 16 - eCellType::LITTLE_ALIEN);
		if (16 <= type && type < 32) {
			this->damage_reductor = this->damage_reductor/OVNI_TYPE::BIG;
		}
	}
	else {
		this->dir = ENEMY_DIR::ELEFT;
		if (0 <= type && type < 16) {
			this->color = ENEMY_COLOR::GREEN;
		}
		else if (16 <= type && type < 32) {
			this->color = ENEMY_COLOR::BLUE;
		}
		else if (32 <= type && type < 48) {
			this->color = ENEMY_COLOR::ORANGE;
		}
		else if (48 <= type && type < 64) {
			this->color = ENEMY_COLOR::RED;
		}
		this->damage_reductor = 125.0 / float(int(type) % 16 - eCellType::LITTLE_ALIEN + 1 + this->color);
	}
}

void Enemy::addShot() {

	if (this->type == eCellType::LITTLE_ALIEN) {
		return;
	}
	eShot shot;

	shot.pos.x = this->pos.x-10;
	shot.pos.y = this->pos.y - 2;

	if (this->type == eCellType::BIG_ALIEN) {
		shot.dir = SHOT_DIR::TO_LEFT;
		shot.moveSpeed = 50.0f;
	}

	else if (this->type == eCellType::OVNI + OVNI_TYPE::BIG*16) {
		shot.dir = SHOT_DIR::TO_DOWN;
		shot.moveSpeed = float(eCellType::OVNI);
	}

	this->shots.push_back(shot);
}

void Enemy::renderShots(Image& framebuffer, float time) {
	int n_shots = this->shots.size();
	for (int i = 0; i < n_shots; i++) {
		framebuffer.setPixel(this->shots[i].pos.x, this->shots[i].pos.y, Color::RED);
	}
}

void Enemy::moveShots(double seconds_elapsed) {
	int n_shots = this->shots.size();
	for (int i = 0; i < n_shots; i++) {
		if (this->shots[i].dir == SHOT_DIR::TO_LEFT) {
			this->shots[i].pos.x -= this->shots[i].moveSpeed * seconds_elapsed;
		}
		if (this->shots[i].dir == SHOT_DIR::TO_DOWN) {
			this->shots[i].pos.y += this->shots[i].moveSpeed * seconds_elapsed;
		}
	}
}

void Enemy::deleteShots() {
	this->shots.clear();
}



