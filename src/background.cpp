#include "background.h"

void Background::InitLayers() {
	layers[eLayer::GROUNDFRONT].layer.loadTGA("data/ground_front.tga"); //example to load an sprite)
	layers[eLayer::GROUND].layer.loadTGA("data/ground.tga"); //example to load an sprite)
	layers[eLayer::ROCKS].layer.loadTGA("data/rocks.tga"); //example to load an sprite)
	layers[eLayer::BG].layer.loadTGA("data/background.tga"); //example to load an sprite)

	layers[eLayer::GROUNDFRONT].moveSpeed = 20.0f;
	layers[eLayer::GROUND].moveSpeed = 17.0f;
	layers[eLayer::ROCKS].moveSpeed = 10.0f;
	layers[eLayer::BG].moveSpeed = 3.0f;

}

void sLayer::renderLayer(Image& framebuffer) {
	int startx = int(pos.x) % framebuffer.width;

	framebuffer.drawImage(this->layer, 0, 0, Area(startx, 0, framebuffer.width - startx, framebuffer.height));
	framebuffer.drawImage(this->layer, framebuffer.width - startx, 0, Area(0, 0, startx, framebuffer.height));
}

void Background::moveBackground(Vector2 movements[4]) {
	movements[eLayer::GROUNDFRONT].x += layers[eLayer::GROUNDFRONT].moveSpeed;
	movements[eLayer::GROUND].x += layers[eLayer::GROUND].moveSpeed;
	movements[eLayer::ROCKS].x += layers[eLayer::ROCKS].moveSpeed;
	movements[eLayer::BG].x += layers[eLayer::BG].moveSpeed;
}

void Background::changeBackgroundPos(Vector2 movements[4], float seconds_elapsed) {
	layers[eLayer::GROUNDFRONT].pos.x += movements[eLayer::GROUNDFRONT].x * seconds_elapsed;
	layers[eLayer::GROUND].pos.x += movements[eLayer::GROUND].x * seconds_elapsed;
	layers[eLayer::ROCKS].pos.x += movements[eLayer::ROCKS].x * seconds_elapsed;
	layers[eLayer::BG].pos.x += movements[eLayer::BG].x * seconds_elapsed;
}