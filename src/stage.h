#ifndef STAGE_H
#define STAGE_H

#include "includes.h"
#include "utils.h"
#include "input.h"
#include "image.h"
#include "character.h"
#include "world.h"

#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4996)

enum STAGE_ID {
	INTRO = 0,
	TUTORIAL = 1,
	TUTMENU = 2,
	PLAYMENU = 3,
	PLAY = 4,
	END = 5,
	EXIT = -1
};

class Button
{
public:
	int x;
	int y;
	int w;
	int h;
	bool isActive;
	bool isSelected;
	STAGE_ID next_stage;
	std::string text;

	//CONSTRUCTORS
	Button();
	Button(int x, int y, int w, int h, STAGE_ID ns, std::string txt);

	//Util functions
	void drawButton(Image& framebuffer, Image font);
	void drawMiniButton(Image& framebuffer, Image font);
	void ActivateButton();
	void DeactivateButton();
	void selectButton();
	void deselectButton();
	void doAction(STAGE_ID& cs, bool& me);
};



class Stage
{
public:
	std::vector<Button*> buttons;
	int n_buttons = 0;
	virtual STAGE_ID GetId() = 0; //empty body
	virtual void InitButtons() = 0; //empty body
	virtual void activateButtons() = 0; //empty body
	virtual void deactivateButtons() = 0; //empty body
	virtual void nextButton() = 0; //empty body
	virtual void prevButton() = 0; //empty body
	virtual void render(Image& framebuffer, World* w, float time) = 0; //empty body
	virtual void update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me) = 0; //empty body
};

class IntroStage : public Stage
{
public:
	int n_buttons = 3;
	virtual STAGE_ID GetId() { return STAGE_ID::INTRO; };
	virtual void InitButtons();
	virtual void activateButtons();
	virtual void deactivateButtons();
	virtual void nextButton();
	virtual void prevButton();
	virtual void render(Image& framebuffer, World* w, float time);
	virtual void update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me);
};

class TutorialStage : public Stage
{
public:
	int n_buttons = 0;
	int n_clicks = 0;
	virtual STAGE_ID GetId() { return STAGE_ID::TUTORIAL; };
	virtual void InitButtons() {};
	virtual void activateButtons() {};
	virtual void deactivateButtons() {};
	virtual void nextButton(){};
	virtual void prevButton(){};
	virtual void render(Image& framebuffer, World* w, float time);
	virtual void update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me);
};

class TutMenuStage : public Stage
{
public:
	int n_buttons = 4;
	virtual STAGE_ID GetId() { return STAGE_ID::TUTMENU; };
	virtual void InitButtons();
	virtual void activateButtons();
	virtual void deactivateButtons();
	virtual void nextButton();
	virtual void prevButton();
	virtual void render(Image& framebuffer, World* w, float time);
	virtual void update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me);
};

class PlayMenuStage : public Stage
{
public:
	int n_buttons = 4;
	virtual STAGE_ID GetId() { return STAGE_ID::PLAYMENU; };
	virtual void InitButtons();
	virtual void activateButtons();
	virtual void deactivateButtons();
	virtual void nextButton();
	virtual void prevButton();
	virtual void render(Image& framebuffer, World* w, float time);
	virtual void update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me);
};

class PlayStage : public Stage
{
public:
	int n_buttons = 0;
	virtual STAGE_ID GetId() { return STAGE_ID::PLAY; };
	virtual void InitButtons(){};
	virtual void activateButtons(){};
	virtual void deactivateButtons(){};
	virtual void nextButton() {};
	virtual void prevButton() {};
	virtual void render(Image& framebuffer, World* w, float time);
	virtual void update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me);
};

class EndStage : public Stage
{
public:
	int n_buttons = 0;
	virtual STAGE_ID GetId() { return STAGE_ID::END; };
	virtual void InitButtons() {};
	virtual void activateButtons() {};
	virtual void deactivateButtons() {};
	virtual void nextButton() {};
	virtual void prevButton() {};
	virtual void render(Image& framebuffer, World* w, float time);
	virtual void update(double seconds_elapsed, World* w, std::vector<Stage*>& st, STAGE_ID& cs, bool& me);
};

void InitStages(std::vector<Stage*>& st);
Stage* GetStage(std::vector<Stage*> st, STAGE_ID id);
Stage* GetCurrentStage(std::vector<Stage*> st, STAGE_ID cs);
void SetStage(STAGE_ID& cs, STAGE_ID id);

#endif
