#pragma once
#include "image.h"


enum eLayer {
	GROUNDFRONT,
	GROUND,
	ROCKS,
	BG
};
struct sLayer {
	Image layer;
	Vector2 pos;
	float moveSpeed;

	void renderLayer(Image& framebuffer);
};

class Background {
public:
	sLayer layers[4];

	void InitLayers();
	void moveBackground(Vector2 movements[4]);
	void changeBackgroundPos(Vector2 movements[4], float seconds_elapsed);
	
};