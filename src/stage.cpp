#include "utils.h"
#include "input.h"
#include "image.h"
#include "character.h"
#include "stage.h"
#include "world.h"
#include "background.h"



// Button functions
Button::Button()
{
	this->x = 0;
	this->y = 0;
	this->w = 10;
	this->h = 10;
	this->isSelected = false;
	this->isActive = false;
}

Button::Button(int x, int y, int w, int h, STAGE_ID ns, std::string txt)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;

	this->next_stage = ns;
	this->isSelected = false;
	this->isActive = false;
	this->text = txt;
}


void Button::drawButton(Image& framebuffer, Image font) {
	if (this->isSelected) {
		framebuffer.drawRectangle(this->x - 4, this->y - 4, this->w + 8, this->h + 8, Color::YELLOW);
	}
	framebuffer.drawRectangle(this->x -1, this->y-1, this->w+2, this->h+2, Color::BLACK);
	framebuffer.drawText(this->text, this->x, this->y, font);
}

void Button::drawMiniButton(Image& framebuffer, Image minifont) {
	if (this->isSelected) {
		framebuffer.drawRectangle(this->x - 2, this->y - 2, this->w + 4, this->h + 4, Color::YELLOW);
	}
	framebuffer.drawRectangle(this->x-1, this->y-1, this->w+2, this->h+2, Color::BLACK);
	framebuffer.drawText(this->text, this->x, this->y, minifont, 4, 6);
}

void Button::ActivateButton() {
	this->isActive = true;
}

void Button::DeactivateButton() {
	this->isActive = false;
}

void Button::selectButton() {
	if (this->isActive) {
		this->isSelected = true;
	}
}
void Button::deselectButton() {
	this->isSelected = false;
}

void Button::doAction(STAGE_ID& cs, bool& me) {
	if (this->isSelected) {
		this->deselectButton();
		if (this->next_stage != (STAGE_ID) (-1)) {
			SetStage(cs, this->next_stage);
		}
		else {
			me = true;
		}
		
	}
}


// IntroStage functions
void IntroStage::InitButtons() {

	this->buttons.reserve(this->n_buttons);
	this->buttons.push_back(new Button(50, 40, 60, 10, STAGE_ID::PLAY, "NEW GAME"));
	this->buttons.push_back(new Button(50, 60, 60, 10, STAGE_ID::TUTORIAL, "TUTORIAL"));
	this->buttons.push_back(new Button(50, 80, 60, 10, STAGE_ID::EXIT,  "EXIT"));
};

void IntroStage::activateButtons() {
	for (int i = 0; i < this->n_buttons; i++) {
		this->buttons[i]->ActivateButton();
	}
}

void IntroStage::deactivateButtons() {
	for (int i = 0; i < this->n_buttons; i++) {
		this->buttons[i]->DeactivateButton();
	}
}

void IntroStage::nextButton() {
	for (int i = 0; i < this->n_buttons; i++) {
		if (this->buttons[i]->isSelected) {
			if (i == this->n_buttons - 1) {
				this->buttons[0]->selectButton();
			}
			else {
				this->buttons[i+1]->selectButton();
			}
			this->buttons[i]->deselectButton();
			return;
		}
	}
	this->buttons[0]->selectButton();
}

void IntroStage::prevButton() {
	for (int i = 0; i < this->n_buttons; i++) {
		if (this->buttons[i]->isSelected) {
			if (i == 0) {
				this->buttons[this->n_buttons-1]->selectButton();
			}
			else {
				this->buttons[i - 1]->selectButton();
			}
			this->buttons[i]->deselectButton();
			return;
		}
	}
	this->buttons[0]->selectButton();
}

void IntroStage::render(Image& framebuffer, World* w, float time) {
	w->renderBackground(framebuffer);
	framebuffer.drawImage(w->tileset, 20, 50, Area(16, 0, 16, 16));			//draws a scaled image
	framebuffer.drawText("Alienated", 0, 0, w->font);				//draws some text using a bitmap font in an image (assuming every char is 7x9)
	
	for (int i = 0; i < this->n_buttons; i++) {
		this->buttons[i]->drawButton(framebuffer, w->font);
	}

};
void IntroStage::update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me)
{	
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN))
	{
		this->nextButton();
	}
	else if (Input::gamepads[0].direction & PAD_DOWN) {
		this->nextButton();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_UP))
	{
		this->prevButton();
	}
	else if (Input::gamepads[0].direction & PAD_UP) {
		this->prevButton();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_Z))
	{
		for (int i = 0; i < this->n_buttons; i++) {
			if (this->buttons[i]->isSelected) {
				st[cs]->deactivateButtons();
				this->buttons[i]->doAction(cs, me);
				st[cs]->activateButtons();
			}
		}
	}
	else if (Input::gamepads[0].wasButtonPressed(A_BUTTON))
	{
		for (int i = 0; i < this->n_buttons; i++) {
			if (this->buttons[i]->isSelected) {
				st[cs]->deactivateButtons();
				this->buttons[i]->doAction(cs, me);
				st[cs]->activateButtons();
			}
		}
	}
};


// TutorialStage functions
void TutorialStage::render(Image& framebuffer, World* w, float time) {

	w->renderBackground(framebuffer);
	if (n_clicks == 0) {
		framebuffer.drawText("Hi, human.", 5, 30, w->minifont, 4, 6);	
		framebuffer.drawText("The earth is getting destroyed", 5, 50, w->minifont, 4, 6);
		framebuffer.drawText("thanks to your specie.", 5, 60, w->minifont, 4, 6);
	}
	if (n_clicks == 1) {
		framebuffer.drawText("Now, you have discovered that", 5, 30, w->minifont, 4, 6);
		framebuffer.drawText("Mercury is livable...", 5, 40, w->minifont, 4, 6);
		framebuffer.drawText("But you are cowards and don't", 5, 60, w->minifont, 4, 6);
		framebuffer.drawText("want to fight against aliens.", 5, 70, w->minifont, 4, 6);
	}
	if (n_clicks == 2) {
		framebuffer.drawImage(w->tileset, 76, 70, Area(16, 0, 16, 16));
		framebuffer.drawText("Humans created a robot to do this job.", 5, 30, w->minifont, 4, 6);
		framebuffer.drawText("However, somebody has to guide him,", 5, 40, w->minifont, 4, 6);
		framebuffer.drawText("and this will be you.", 5, 50, w->minifont, 4, 6);
	}

	if (n_clicks == 3) {
		framebuffer.drawText("Click up arrow to jump", 5, 30, w->minifont, 4, 6);
		framebuffer.drawText("Click down arrow to become little", 5, 40, w->minifont, 4, 6);
		framebuffer.drawText("Click z to shot", 5, 50, w->minifont, 4, 6);
		framebuffer.drawImage(w->tileset, w->tutplayer->pos.x - 4, w->tutplayer->pos.y - 8, Area(16, int(w->tutplayer->type) * 16, 16, 16));
		w->tutplayer->renderShots(framebuffer);
	}

	if (n_clicks == 4) {
		framebuffer.drawText("These are the types of enemy", 5, 30, w->minifont, 4, 6);
		framebuffer.drawText("that you can find.", 5, 40, w->minifont, 4, 6);

		framebuffer.drawText("They will appear from the right.", 5, 60, w->minifont, 4, 6);
		framebuffer.drawImage(w->tileset, 45, 75, Area(48, 0, 16, 16));
		framebuffer.drawImage(w->tileset, 63, 75, Area(96, 0, 16, 16));
		framebuffer.drawImage(w->tileset, 81, 75, Area(144, 0, 16, 16));
		framebuffer.drawImage(w->tileset, 99, 75, Area(144, 16, 16, 16));
	}

	if (n_clicks == 5) {
		framebuffer.drawText("If you do not kill the enemies", 5, 40, w->minifont, 4, 6);
		framebuffer.drawText("before they cross the screen", 5, 50, w->minifont, 4, 6);
		framebuffer.drawText("you will lose a life.", 5, 60, w->minifont, 4, 6);;

		framebuffer.drawText("You have 3 lives.", 5, 80, w->minifont, 4, 6);;

		for (int i = 0; i < 3; i++) {
			framebuffer.drawImage(w->tileset, 105 + i * 17, 5, Area(160, 0, 16, 16));
		}
	}

	if (n_clicks == 6) {
		framebuffer.drawText("Let's try it", 5, 40, w->minifont, 4, 6);
	}
	if (n_clicks == 7) {
		framebuffer.drawText("Let's try it", 5, 40, w->minifont, 4, 6);
		w->renderTutPlayer(framebuffer, time);
		w->renderTutEnemies(framebuffer, time);
		w->renderTutShots(framebuffer, time);
		w->renderTutLives(framebuffer);
	}

	if (n_clicks == 8) {
		framebuffer.drawText("You are ready to play the game.", 5, 40, w->minifont, 4, 6);
		framebuffer.drawText("But be careful. On the game the", 5, 50, w->minifont, 4, 6);
		framebuffer.drawText("world is moving and goes faster.", 5, 60, w->minifont, 4, 6);
	}
	framebuffer.drawText("Click right cursor to continue", 30, 110, w->minifont, 4, 6);
}

void TutorialStage::update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_RIGHT))
	{
		n_clicks++;
	}
	else if (Input::gamepads[0].direction & PAD_RIGHT)
	{
		n_clicks++;
	}
	if (n_clicks == 3 || n_clicks == 7) {
		Vector2 player_movement;
		w->tutplayer->type = PLAYER_TYPE::PRIGHT;

		if (Input::wasKeyPressed(SDL_SCANCODE_UP))
		{
			if (w->tutplayer->isFalling) {
				w->tutplayer->isJumping = false;
			}
			else {
				w->tutplayer->isJumping = true;
			}

		}
		else if (Input::gamepads[0].direction & PAD_UP)
		{
			w->tutplayer->isJumping = true;
		}

		else if (Input::isKeyPressed(SDL_SCANCODE_DOWN))
		{
			w->tutplayer->type = PLAYER_TYPE::PDOWN;
		}
		else if (Input::gamepads[0].direction & PAD_DOWN)
		{
			w->tutplayer->type = PLAYER_TYPE::PDOWN;
		}
		if (Input::wasKeyPressed(SDL_SCANCODE_Z))
		{
			w->tutplayer->addShot();
		}
		else if (Input::gamepads[0].wasButtonPressed(A_BUTTON))
		{
			w->tutplayer->addShot();
		}

		if (w->tutplayer->isJumping) {
			player_movement.y += w->tutplayer->jumpSpeed * 2.5;
			w->tutplayer->jumpSpeed -= seconds_elapsed;

			if (w->tutplayer->pos.y <= w->tutplayer->min_y) {
				w->tutplayer->isJumping = false;
				w->tutplayer->isFalling = true;
			}
		}

		else if (w->tutplayer->isFalling) {
			player_movement.y -= w->tutplayer->jumpSpeed * 2.5;
			w->tutplayer->jumpSpeed += seconds_elapsed;
			if (w->tutplayer->pos.y >= w->tutplayer->max_y) {
				w->tutplayer->isFalling = false;
			}
		}
		w->tutplayer->pos.y -= player_movement.y * seconds_elapsed;
		w->moveTutShots(seconds_elapsed);

		if (n_clicks == 7) {
			Vector2 movements[4];
			Vector2 enemy_movement;
			w->moveTutEnemies(enemy_movement.x);
			w->changeTutEnemiesPos(enemy_movement, seconds_elapsed);
			w->checkTutEnemyMoving(enemy_movement);
			w->checkTutDamages();
			w->checkTutEnemyPassed();
		}

		if (w->hasEndedTutorial()) {
			n_clicks++;
		}
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_X))
	{
		SetStage(cs, STAGE_ID::TUTMENU);
		st[cs]->activateButtons();
	}
	else if (Input::gamepads[0].wasButtonPressed(B_BUTTON))
	{
		SetStage(cs, STAGE_ID::TUTMENU);
		st[cs]->activateButtons();
	}

	if (n_clicks == 9) {
		SetStage(cs, STAGE_ID::PLAY);
		st[cs]->activateButtons();
	}
}

// TutMenuStage functions
void TutMenuStage::InitButtons() {
	this->buttons.reserve(this->n_buttons);
	this->buttons.push_back(new Button(50, 30, 60, 10, STAGE_ID::INTRO, "INTRO"));
	this->buttons.push_back(new Button(50, 50, 60, 10, STAGE_ID::PLAY, "NEW GAME"));
	this->buttons.push_back(new Button(50, 70, 60, 10, STAGE_ID::TUTORIAL, "RESUME"));
	this->buttons.push_back(new Button(50, 90, 60, 10, STAGE_ID::EXIT, "EXIT"));
};
void TutMenuStage::activateButtons() {
	for (int i = 0; i < this->n_buttons; i++) {
		this->buttons[i]->ActivateButton();
	}
}

void TutMenuStage::deactivateButtons() {
	for (int i = 0; i < this->n_buttons; i++) {
		this->buttons[i]->DeactivateButton();
	}
}
void TutMenuStage::nextButton() {
	for (int i = 0; i < this->n_buttons; i++) {
		if (this->buttons[i]->isSelected) {
			if (i == this->n_buttons - 1) {
				this->buttons[0]->selectButton();
			}
			else {
				this->buttons[i + 1]->selectButton();
			}
			this->buttons[i]->deselectButton();
			return;
		}
	}
	this->buttons[0]->selectButton();
}

void TutMenuStage::prevButton() {
	for (int i = 0; i < this->n_buttons; i++) {
		if (this->buttons[i]->isSelected) {
			if (i == 0) {
				this->buttons[this->n_buttons - 1]->selectButton();
			}
			else {
				this->buttons[i - 1]->selectButton();
			}
			this->buttons[i]->deselectButton();
			return;
		}
	}
	this->buttons[0]->selectButton();
}

void TutMenuStage::render(Image& framebuffer, World* w, float time) {
	w->renderBackground(framebuffer);
	framebuffer.drawText("Menu", 0, 0, w->font);				//draws some text using a bitmap font in an image (assuming every char is 7x9)
	
	for (int i = 0; i < this->n_buttons; i++) {
		this->buttons[i]->drawButton(framebuffer, w->font);
	}
}

void TutMenuStage::update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me)
{	
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN))
	{
		this->nextButton();
	}
	else if (Input::gamepads[0].direction & PAD_DOWN)
	{
		this->nextButton();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_UP))
	{
		this->prevButton();
	}
	else if (Input::gamepads[0].direction & PAD_UP)
	{
		this->prevButton();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_Z))
	{
		for (int i = 0; i < this->n_buttons; i++) {
			if (this->buttons[i]->isSelected) {
				st[cs]->deactivateButtons();
				this->buttons[i]->doAction(cs, me);
				st[cs]->activateButtons();
			}
		}
	}
	else if (Input::gamepads[0].wasButtonPressed(A_BUTTON))
	{
		for (int i = 0; i < this->n_buttons; i++) {
			if (this->buttons[i]->isSelected) {
				st[cs]->deactivateButtons();
				this->buttons[i]->doAction(cs, me);
				st[cs]->activateButtons();
			}
		}
	}
}

// PlayMenuStage functions
void PlayMenuStage::InitButtons() {
	this->buttons.reserve(this->n_buttons);
	this->buttons.push_back(new Button(50, 30, 60, 10, STAGE_ID::INTRO, "INTRO"));
	this->buttons.push_back(new Button(50, 50, 60, 10, STAGE_ID::TUTORIAL, "TUTORIAL"));
	this->buttons.push_back(new Button(50, 70, 60, 10, STAGE_ID::PLAY, "RESUME"));
	this->buttons.push_back(new Button(50, 90, 60, 10, STAGE_ID::EXIT, "EXIT"));
};
void PlayMenuStage::activateButtons() {
	for (int i = 0; i < this->n_buttons; i++) {
		this->buttons[i]->ActivateButton();
	}
}

void PlayMenuStage::deactivateButtons() {
	for (int i = 0; i < this->n_buttons; i++) {
		this->buttons[i]->DeactivateButton();
	}
}
void PlayMenuStage::nextButton() {
	for (int i = 0; i < this->n_buttons; i++) {
		if (this->buttons[i]->isSelected) {
			if (i == this->n_buttons - 1) {
				this->buttons[0]->selectButton();
			}
			else {
				this->buttons[i + 1]->selectButton();
			}
			this->buttons[i]->deselectButton();
			return;
		}
	}
	this->buttons[0]->selectButton();
}

void PlayMenuStage::prevButton() {
	for (int i = 0; i < this->n_buttons; i++) {
		if (this->buttons[i]->isSelected) {
			if (i == 0) {
				this->buttons[this->n_buttons - 1]->selectButton();
			}
			else {
				this->buttons[i - 1]->selectButton();
			}
			this->buttons[i]->deselectButton();
			return;
		}
	}
	this->buttons[0]->selectButton();
}

void PlayMenuStage::render(Image& framebuffer, World* w, float time) {
	w->renderBackground(framebuffer);
	framebuffer.drawText("Menu", 0, 0, w->font);				//draws some text using a bitmap font in an image (assuming every char is 7x9)

	for (int i = 0; i < this->n_buttons; i++) {
		this->buttons[i]->drawButton(framebuffer, w->font);
	}
}

void PlayMenuStage::update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me)
{	
	if (Input::wasKeyPressed(SDL_SCANCODE_DOWN))
	{
		this->nextButton();
	}
	else if (Input::gamepads[0].direction & PAD_DOWN)
	{
		this->nextButton();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_UP))
	{
		this->prevButton();
	}
	else if (Input::gamepads[0].direction & PAD_UP)
	{
		this->prevButton();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_Z))
	{
		for (int i = 0; i < this->n_buttons; i++) {
			if (this->buttons[i]->isSelected) {
				st[cs]->deactivateButtons();
				this->buttons[i]->doAction(cs, me);
				st[cs]->activateButtons();
			}
		}
	}
	else if (Input::gamepads[0].wasButtonPressed(A_BUTTON))
	{
		for (int i = 0; i < this->n_buttons; i++) {
			if (this->buttons[i]->isSelected) {
				st[cs]->deactivateButtons();
				this->buttons[i]->doAction(cs, me);
				st[cs]->activateButtons();
			}
		}
	}
}

// PlayStage functions

void PlayStage::render(Image& framebuffer, World* w, float time) {
	
	w->renderBackground(framebuffer);
	framebuffer.drawText("Alienated", 5, 10, w->font);
	w->renderPlayer(framebuffer, time);
	w->renderEnemies(framebuffer, time);
	w->renderShots(framebuffer, time);
	w->renderLives(framebuffer);

	framebuffer.drawText("level "+std::to_string(w->level), 75, 5, w->minifont, 4, 6);
}

void PlayStage::update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me)
{	
	Vector2 movements[4];
	Vector2 enemy_movement;
	Vector2 player_movement;
	w->player->type = PLAYER_TYPE::PRIGHT;
	int old_player_shots = w->player->shots.size();

	if (Input::wasKeyPressed(SDL_SCANCODE_UP))
	{
		if (w->player->isFalling) {
			w->player->isJumping = false;
		}
		else {
			w->player->isJumping = true;
		}
		
	}
	else if (Input::gamepads[0].direction & PAD_UP)
	{
		w->player->isJumping = true;

	}
	else if (Input::isKeyPressed(SDL_SCANCODE_DOWN))
	{
		w->player->type = PLAYER_TYPE::PDOWN;
	}
	else if (Input::gamepads[0].direction & PAD_DOWN)
	{
		w->player->type = PLAYER_TYPE::PDOWN;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_Z))
	{
		w->player->addShot();
	}
	else if (Input::gamepads[0].wasButtonPressed(A_BUTTON))
	{
		w->player->addShot();
	}

	if (Input::wasKeyPressed(SDL_SCANCODE_X))
	{
		SetStage(cs, STAGE_ID::PLAYMENU);
		st[cs]->activateButtons();
	}
	else if (Input::gamepads[0].wasButtonPressed(B_BUTTON))
	{
		SetStage(cs, STAGE_ID::PLAYMENU);
		st[cs]->activateButtons();
	}

	if (w->player->isJumping) {
		player_movement.y += w->player->jumpSpeed*2.5;
		w->player->jumpSpeed -= seconds_elapsed;

		if (w->player->pos.y <= w->player->min_y) {
			w->player->isJumping = false;
			w->player->isFalling = true;
		}
	}

	else if (w->player->isFalling) {
		player_movement.y -= w->player->jumpSpeed * 2.5;
		w->player->jumpSpeed += seconds_elapsed;
		if (w->player->pos.y >= w->player->max_y) {
			w->player->isFalling = false;
		}
	}

	w->player->pos.y -= player_movement.y * seconds_elapsed;

	int new_player_shots = w->player->shots.size();

	w->moveWorld(movements, enemy_movement.x);
	w->moveEnemies(enemy_movement.x);
	w->moveShots(seconds_elapsed);
	w->changeEnemiesPos(enemy_movement, seconds_elapsed);
	w->bg->changeBackgroundPos(movements, seconds_elapsed);
	w->player->isShooting = new_player_shots != old_player_shots;
	w->checkEnemyMoving(enemy_movement);
	w->checkEnemyPassed();
	w->checkDamages();

	if (w->hasEndedLevel()) {
		w->deleteShots();
		w->level++;
	}

	if (w->hasEndedGame()) {
		SetStage(cs, STAGE_ID::END);
		st[cs]->activateButtons();
	}
}

// EndStage functions


void EndStage::render(Image& framebuffer, World* w, float time) {
	framebuffer.fill(Color::BLACK);

	int startx;
	if (w->player->hasWon()) {
		startx = eCellType::ROBOT * w->cellsize;
		framebuffer.drawImage(w->tileset, 10, 95, Area(startx, 0, w->cellsize, w->cellsize));
		framebuffer.drawImage(w->tileset, 50, 35, Area(startx, 0, w->cellsize, w->cellsize));
		framebuffer.drawImage(w->tileset, 110, 10, Area(startx, 0, w->cellsize, w->cellsize));
		framebuffer.drawImage(w->tileset, 140, 80, Area(startx, 0, w->cellsize, w->cellsize));
		startx = eCellType::HEART * w->cellsize;
	}
	else {
		startx = eCellType::LITTLE_ALIEN * w->cellsize;
	}
	framebuffer.drawImage(w->tileset, 18, 20, Area(startx, 0, w->cellsize, w->cellsize));
	framebuffer.drawImage(w->tileset, 90, 100, Area(startx, 0, w->cellsize, w->cellsize));
	framebuffer.drawImage(w->tileset, 130, 60, Area(startx, 0, w->cellsize, w->cellsize));
	framebuffer.drawImage(w->tileset, 102, 43, Area(startx, 0, w->cellsize, w->cellsize));
	framebuffer.drawImage(w->tileset, 30, 80, Area(startx, 0, w->cellsize, w->cellsize));
	framebuffer.drawImage(w->tileset, 56, 10, Area(startx, 0, w->cellsize, w->cellsize));
	framebuffer.drawImage(w->tileset, 140, 15, Area(startx, 0, w->cellsize, w->cellsize));
	framebuffer.drawText("END", framebuffer.width/2-8, framebuffer.height / 2 -5, w->font);
}

void EndStage::update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me){

	if (Input::wasKeyPressed(SDL_SCANCODE_X))
	{
		SetStage(cs, STAGE_ID::PLAYMENU);
		st[cs]->activateButtons();
	}
	else if (Input::gamepads[0].wasButtonPressed(B_BUTTON))
	{
		SetStage(cs, STAGE_ID::PLAYMENU);
		st[cs]->activateButtons();
	}
}

// General functions
void InitStages(std::vector<Stage*>& st) {

	st.reserve(6);
	st.push_back(new IntroStage());
	st.push_back(new TutorialStage());
	st.push_back(new TutMenuStage());
	st.push_back(new PlayMenuStage());
	st.push_back(new PlayStage());
	st.push_back(new EndStage());

	st[STAGE_ID::INTRO]->InitButtons();
	st[STAGE_ID::TUTMENU]->InitButtons();
	st[STAGE_ID::PLAYMENU]->InitButtons();
}

Stage* GetStage(std::vector<Stage*> st, STAGE_ID id) {
	return st[id];
}

Stage* GetCurrentStage(std::vector<Stage*> st, STAGE_ID cs) {
	return GetStage(st, cs);
}

void SetStage(STAGE_ID& cs, STAGE_ID id) { 
	
	cs = id; 

};



