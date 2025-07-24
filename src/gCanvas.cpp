/*
* gCanvas.cpp
*
*  Created on: May 6, 2020
*      Author: noyan
*/


#include "gCanvas.h"


gCanvas::gCanvas(gApp* root) : gBaseCanvas(root) {
	this->root = root;
}



gCanvas::~gCanvas() {

}

void gCanvas::setup() {
	platformx[PLAYER_HUMAN][PLATFORM_TOP] = 441;
	platformx[PLAYER_HUMAN][PLATFORM_BOTTOM] = 441;
	platformy[PLAYER_HUMAN][PLATFORM_TOP] = 126;
	platformy[PLAYER_HUMAN][PLATFORM_BOTTOM] = 635;
	map.loadImage("map.png");
	ship1.loadImage("gemiler/ship_1/ship_1_1.png");
	ship2.loadImage("gemiler/ship_2/ship_2_1.png");
	ship3.loadImage("gemiler/ship_3/ship_3_1.png");
	ship4.loadImage("gemiler/ship_4/ship_4_1.png");
	platform.loadImage("silahli_gemi1.png");
	std::vector<std::string> ship1paths;
	for(int i = 1; i <= 25; i++) {
		ship1paths.push_back("gemiler/ship_1/ship_1_" + gToStr(i) + ".png");
	}
	ship1frames = loadFrames(ship1paths);

	addShip(&ship1frames,PLAYER_HUMAN,PLATFORM_BOTTOM);
}

void gCanvas::update() {
	float deltaTime = appmanager->getElapsedTime();
	for(Ship* ship : activeships) {
		ship->animator.update(deltaTime);
	}
}

void gCanvas::draw() {
	map.draw(0,0);
	ship1.draw(60,20);
	ship2.draw(90,310);
	ship3.draw(47,550);
	ship4.draw(110,900);
	platform.draw(platformx[PLAYER_HUMAN][PLATFORM_TOP],platformy[PLAYER_HUMAN][PLATFORM_TOP],
			platform.getWidth(),platform.getHeight(),90);
	platform.draw(platformx[PLAYER_HUMAN][PLATFORM_BOTTOM],platformy[PLAYER_HUMAN][PLATFORM_BOTTOM],
			platform.getWidth(),platform.getHeight(),90);

	for(Ship* ship : activeships) {
		ship->animator.draw(ship->x, ship->y);
	}
}

void gCanvas::addShip(AnimationFrames* frames,
		PlayerType playertype, PlatformPos platformpos) {
	Ship* ship = new Ship();
	ship->x = platformx[playertype][platformpos];
	ship->y = platformy[playertype][platformpos];
	SpriteAnimation* anim = new SpriteAnimation(0, frames, 10);
	ship->animator.addAnimation(anim);
	activeships.push_back(ship);

}

void gCanvas::keyPressed(int key) {
//	gLogi("gCanvas") << "keyPressed:" << key;
}

void gCanvas::keyReleased(int key) {
//	gLogi("gCanvas") << "keyReleased:" << key;
}

void gCanvas::charPressed(unsigned int codepoint) {
//	gLogi("gCanvas") << "charPressed:" << gCodepointToStr(codepoint);
}

void gCanvas::mouseMoved(int x, int y) {
//	gLogi("gCanvas") << "mouseMoved" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseDragged(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseDragged" << ", x:" << x << ", y:" << y << ", b:" << button;
}

void gCanvas::mousePressed(int x, int y, int button) {
//	gLogi("gCanvas") << "mousePressed" << ", x:" << x << ", y:" << y << ", b:" << button;
}

void gCanvas::mouseReleased(int x, int y, int button) {
//	gLogi("gCanvas") << "mouseReleased" << ", button:" << button;
}

void gCanvas::mouseScrolled(int x, int y) {
//	gLogi("gCanvas") << "mouseScrolled" << ", x:" << x << ", y:" << y;
}

void gCanvas::mouseEntered() {

}

void gCanvas::mouseExited() {

}

void gCanvas::windowResized(int w, int h) {

}

void gCanvas::showNotify() {

}

void gCanvas::hideNotify() {

}

