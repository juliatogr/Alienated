#include "utils.h"
#include "image.h"
#include "world.h"
#include "background.h"

void World::loadGameMap(const char* filename)
{
	FILE* file = fopen(filename, "rb");
	if (file == NULL) //file not found
		return;

	sMapHeader header; //read header and store it in the struct
	fread(&header, sizeof(sMapHeader), 1, file);
	assert(header.bytes == 1); //always control bad cases!!
	//allocate memory for the cells data and read it
	unsigned char* cells = new unsigned char[header.w * header.h];
	fread(cells, header.bytes, header.w * header.h, file);
	fclose(file); //always close open files
	//create the map where we will store it
	GameMap* map = new GameMap(header.w, header.h);

	for (int y = 0; y < map->height; y++) {
		this->n_enemies.push_back(0);
		std::vector<Vector2> pos;
		std::vector<eCellType> type;
		std::vector<Enemy*> enemy;
		this->enemySpawnPositions.push_back(pos);
		this->enemySpawnTypes.push_back(type);
		this->enemies.push_back(enemy);
	}

	for (int x = 0; x < map->width; x++) {
		for (int y = 0; y < map->height; y++) {
			map->getCell(x, y).type = (eCellType)cells[x + y * map->width];

			Vector2 pos;
			pos.x = x * 16;
			pos.y = 104;
			if (map->getCell(x, y).type % 16 == eCellType::OVNI) {
				pos.y = 80;
			}
			if ((int)map->getCell(x, y).type != (int)(eCellType)NULL) {
				this->enemySpawnPositions[y].push_back(pos);
				this->enemySpawnTypes[y].push_back(map->getCell(x, y).type);
				map->getCell(x, y).type = (eCellType)NULL;
				this->n_enemies[y]++;
			}
		}			
	}

	delete[] cells; //always free any memory allocated!

	this->map = map;
}

void World::spawnEnemies() {
	for (int i = 0; i < n_levels+1; i++) {
		enemies[i].reserve(n_enemies[i]);
		for (int j = 0; j < n_enemies[i]; j++) {
			Vector2 pos = enemySpawnPositions[i][j];
			eCellType type = enemySpawnTypes[i][j];
			Enemy* enemy = new Enemy(pos, type);
			enemies[i].push_back(enemy);
		
		}
	}
}

void World::moveEnemies(float& movementx) {
	for (int i = 0; i < n_enemies[level]; i++) {
		movementx += enemies[level][i]->moveSpeed;
	}
}
void World::moveTutEnemies(float& movementx) {
	for (int i = 0; i < n_enemies[7]; i++) {
		movementx += enemies[7][i]->moveSpeed;
	}
}

void World::moveWorld(Vector2 bg_movements[4], float& movementx) {
	bg->moveBackground(bg_movements);
	movementx += bg->layers[eLayer::GROUNDFRONT].moveSpeed;
}

void World::renderBackground(Image& framebuffer) {
	bg->layers[eLayer::BG].renderLayer(framebuffer);
	bg->layers[eLayer::ROCKS].renderLayer(framebuffer);
	bg->layers[eLayer::GROUND].renderLayer(framebuffer);
	bg->layers[eLayer::GROUNDFRONT].renderLayer(framebuffer);
}

void World::renderPlayer(Image& framebuffer, float time) {

	int startx = (int(time * player->animSpeed) % 2 + eCellType::ROBOT) * 16;
	int starty = int(player->type) * cellsize;

	int posx = player->pos.x - 4;
	int posy = player->pos.y - 8;

	framebuffer.drawImage(tileset, posx, posy, Area(startx, starty, cellsize, cellsize));
}

void World::renderTutPlayer(Image& framebuffer, float time) {

	int startx = int(eCellType::ROBOT) * cellsize;
	int starty = int(tutplayer->type) * cellsize;

	int posx = tutplayer->pos.x - 4;
	int posy = tutplayer->pos.y - 8;

	framebuffer.drawImage(tileset, posx, posy, Area(startx, starty, cellsize, cellsize));
}

void World::renderEnemies(Image& framebuffer, float time) {

	//this->addEnemyShots(time);
	for (int i = 0; i < n_enemies[level]; i++) {
		eCellType type = enemies[level][i]->type;
		int startx = (int(type) % 16 + enemies[level][i]->dir) * cellsize;
		int starty = enemies[level][i]->color * cellsize;

 		if (type %16 != eCellType::OVNI) {
			startx = enemies[level][i]->isMoving ? startx += (int(time * enemies[level][i]->animSpeed) % 2) * cellsize : startx;
		}
 		else if (type == int(eCellType::OVNI)+ OVNI_TYPE::BIG*cellsize) {
			starty = int(OVNI_TYPE::BIG) * cellsize;
		}

		float posx = enemies[level][i]->pos.x - 5;
		float posy = enemies[level][i]->pos.y - 8;

		framebuffer.drawImage(tileset, posx, posy, Area(startx, starty, cellsize, cellsize));
	}
}

void World::renderTutEnemies(Image& framebuffer, float time) {

	//this->addEnemyShots(time);
	for (int i = 0; i < n_enemies[7]; i++) {
		eCellType type = enemies[7][i]->type;
		int startx = (int(type) % 16 + enemies[7][i]->dir) * cellsize;
		int starty = 0;

		if (type % 16 != eCellType::OVNI) {
			startx = enemies[7][i]->isMoving ? startx += (int(time * enemies[7][i]->animSpeed) % 2) * cellsize : startx;
		}
		else if (type == int(eCellType::OVNI) + OVNI_TYPE::BIG * cellsize) {
			starty = int(OVNI_TYPE::BIG) * cellsize;
		}
		float posx = enemies[7][i]->pos.x - 5;
		float posy = enemies[7][i]->pos.y - 8;

		framebuffer.drawImage(tileset, posx, posy, Area(startx, starty, cellsize, cellsize));
	}
}

void World::renderShots(Image& framebuffer, float time) {

	for (int i = 0; i < n_enemies[level]; i++) {
		enemies[level][i]->renderShots(framebuffer, time);
	}
	player->renderShots(framebuffer);
}
void World::renderTutShots(Image& framebuffer, float time) {

	for (int i = 0; i < n_enemies[7]; i++) {
		enemies[7][i]->renderShots(framebuffer, time);
	}
	tutplayer->renderShots(framebuffer);
}

void World::renderLives(Image& framebuffer) {
	int startx = int(eCellType::HEART) * cellsize;
	int starty = int(HEART_TYPE::FULL);

	for (int i = 0; i < 3; i++) {

		if (i >= player->n_lives) {
			starty = int(HEART_TYPE::EMPTY) * cellsize;
		}
		framebuffer.drawImage(tileset, 105 + i * 17, 5, Area(startx, starty, cellsize, cellsize));
	}
}

void World::renderTutLives(Image& framebuffer) {
	int startx = int(eCellType::HEART) * cellsize;
	int starty = int(HEART_TYPE::FULL);

	for (int i = 0; i < 3; i++) {

		if (i >= tutplayer->n_lives) {
			starty = int(HEART_TYPE::EMPTY) * cellsize;
		}
		framebuffer.drawImage(tileset, 105 + i * 17, 5, Area(startx, starty, cellsize, cellsize));
	}
}

void World::changeEnemiesPos(Vector2 movement, float seconds_elapsed) {

	for (int i = 0; i < n_enemies[level]; i++) {
		enemies[level][i]->pos.x -= movement.x*seconds_elapsed;
		enemies[level][i]->pos.y += movement.y * seconds_elapsed;

		int n_shots = enemies[level][i]->shots.size();
		if (n_shots > 0) {
			for (int j = 0; j < n_shots; j++) {
				enemies[level][i]->shots[j].pos.x -= movement.x * seconds_elapsed;
				enemies[level][i]->shots[j].pos.y += movement.y * seconds_elapsed;
			}
		}
	}
}
void World::changeTutEnemiesPos(Vector2 movement, float seconds_elapsed) {

	for (int i = 0; i < n_enemies[7]; i++) {
		enemies[7][i]->pos.x -= movement.x * seconds_elapsed;
		enemies[7][i]->pos.y += movement.y * seconds_elapsed;

		int n_shots = enemies[7][i]->shots.size();
		if (n_shots > 0) {
			for (int j = 0; j < n_shots; j++) {
				enemies[7][i]->shots[j].pos.x -= movement.x * seconds_elapsed;
				enemies[7][i]->shots[j].pos.y += movement.y * seconds_elapsed;
			}
		}
	}
}

void World::checkEnemyMoving(Vector2 movement) {

	for (int i = 0; i < n_enemies[level]; i++) {
		enemies[level][i]->isMoving = movement.x != 0.0f || movement.y != 0.0f;
	}
}

void World::checkTutEnemyMoving(Vector2 movement) {

	for (int i = 0; i < n_enemies[7]; i++) {
		enemies[7][i]->isMoving = movement.x != 0.0f || movement.y != 0.0f;
	}
}
void World::addEnemyShots(float time) {

	if (int(time) % 7 == 0) {
		for (int i = 0; i < n_enemies[level]; i++) {
			if (enemies[level][i]->type == eCellType::BIG_ALIEN && isValidPos(enemies[level][i]->pos)) {
				enemies[level][i]->addShot();
			}
		}
	}
}



void World::deleteShots() {

	for (int i = 0; i < n_enemies[this->level]; i++) {
		this->enemies[this->level][i]->deleteShots();
	}
	this->player->deleteShots();
}

void World::moveShots(double seconds_elapsed) {

	for (int i = 0; i < n_enemies[this->level]; i++) {
		this->enemies[this->level][i]->moveShots(seconds_elapsed);
		
	}
	this->player->moveShots(seconds_elapsed);
}

void World::moveTutShots(double seconds_elapsed) {

	for (int i = 0; i < n_enemies[7]; i++) {
		this->enemies[7][i]->moveShots(seconds_elapsed);

	}
	this->tutplayer->moveShots(seconds_elapsed);
}

void World::checkDamages() {
	int n_shots = this->player->shots.size();

	if (n_shots > 0) {
		for (int i = 0; i < n_enemies[this->level]; i++) {
			for (int j = 0; j < n_shots; j++) {
				if (this->player->shots.size() > 0) {
					if (isHitShot(this->player->shots[j].pos, this->enemies[this->level][i]->pos)) {
						this->enemies[this->level][i]->live_percentage -= this->enemies[this->level][i]->damage_reductor;
						this->player->shots.erase(this->player->shots.begin());
						n_shots--;

						if (this->enemies[this->level][i]->live_percentage <= 0) {
							this->enemySpawnPositions[this->level].erase(this->enemySpawnPositions[this->level].begin());
							this->enemySpawnTypes[this->level].erase(this->enemySpawnTypes[this->level].begin());
							this->enemies[this->level].erase(this->enemies[this->level].begin() + i);
							n_enemies[this->level]--;
						}
						break;
					}
				}

			}
		}

	}
}

void World::checkTutDamages() {
	int n_shots = tutplayer->shots.size();

	if (n_shots > 0) {
		for (int i = 0; i < n_enemies[7]; i++) {
			for (int j = 0; j < n_shots; j++) {
				if (tutplayer->shots.size() > 0) {
					if (isHitShot(tutplayer->shots[j].pos, this->enemies[7][i]->pos)) {
						this->enemies[7][i]->live_percentage -= this->enemies[7][i]->damage_reductor;
						tutplayer->shots.erase(tutplayer->shots.begin());
						n_shots--;

						if (this->enemies[7][i]->live_percentage <= 0) {
							this->enemySpawnPositions[7].erase(this->enemySpawnPositions[7].begin());
							this->enemySpawnTypes[7].erase(this->enemySpawnTypes[7].begin());
							this->enemies[7].erase(this->enemies[7].begin() + i);
							n_enemies[7]--;
						}
						break;
					}
				}
			}
		}
	}
}

bool isValidPos(Vector2 pos) {
	if (pos.x > 0 && pos.y > 0 && pos.x < 160 && pos.y < 120) {
		return true;
	}
	return false;
}

bool isHitShot(Vector2 shotpos, Vector2 objpos) {
	if (isValidPos(objpos)) {
		if (shotpos.x > objpos.x - 5 && shotpos.x < objpos.x + 3 && shotpos.y > objpos.y - 8 && shotpos.y < objpos.y + 8) {
			return true;
		}
	}
	return false;
}

bool World::hasEndedLevel() {
	if (n_enemies[this->level] == 0) {
		return true;
	}
	return false;
}

bool World::hasEndedGame() {
	if (this->level >= this->n_levels || this->player->n_lives == 0) {
		return true;
	}
	return false;
}

bool World::hasEndedTutorial() {
	int n_tutenemies = enemies[7].size();
	if (n_tutenemies == 0 || this->player->n_lives == 0) {
		return true;
	}
	return false;
}

void World::checkEnemyPassed() {

	for (int i = 0; i < n_enemies[this->level]; i++) {
		if (enemies[this->level][0]->pos.x < 0) {
			this->enemies[this->level].erase(this->enemies[this->level].begin());
			n_enemies[this->level]--;
			this->player->n_lives--;
		}
	}
}

void World::checkTutEnemyPassed() {

	for (int i = 0; i < n_enemies[7]; i++) {
		if (enemies[7][0]->pos.x < 0) {
			this->enemies[7].erase(this->enemies[7].begin());
			n_enemies[7]--;
			this->tutplayer->n_lives--;
		}
	}
}
