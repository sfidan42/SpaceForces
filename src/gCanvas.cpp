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
	appmanager->setTargetFramerate(165);
	selectedplatform = PLATFORM_NONE;

	map.loadImage("map.png");
	ship1.loadImage("gemiler/ship_1/ship_1_1.png");
	ship2.loadImage("gemiler/ship_2/ship_2_1.png");
	ship3.loadImage("gemiler/ship_3/ship_3_1.png");
	ship4.loadImage("gemiler/ship_4/ship_4_1.png");
	platform.loadImage("silahli_gemi1.png");
	humanbullet.loadImage("human_bullet.png");
	aibullet.loadImage("ai_bullet.png");
	std::vector<std::string> ship1paths;
	for(int i = 1; i <= 25; i++) {
		ship1paths.push_back("gemiler/ship_1/ship_1_" + gToStr(i) + ".png");
	}
	ship1frames = loadFrames(ship1paths);
	platformx[PLAYER_HUMAN][PLATFORM_TOP] = 577;
	platformx[PLAYER_HUMAN][PLATFORM_BOTTOM] = 577;
	platformy[PLAYER_HUMAN][PLATFORM_TOP] = 280;
	platformy[PLAYER_HUMAN][PLATFORM_BOTTOM] = 790;

	platformx[PLAYER_AI][PLATFORM_TOP] = 1600;
    platformx[PLAYER_AI][PLATFORM_BOTTOM] = 1600;
	platformy[PLAYER_AI][PLATFORM_TOP] = 280;
	platformy[PLAYER_AI][PLATFORM_BOTTOM] = 790;

	addShip(&ship1frames,PLAYER_HUMAN,PLATFORM_BOTTOM);
	addShip(&ship1frames,PLAYER_AI,PLATFORM_BOTTOM);

	std::vector<std::string> selectionpaths;
	selectionpaths.reserve(24);
	for (int i = 1; i < 25; i ++) {
		std::string path = "platform_anim/platform_" + gToStr(i) + ".png";
		selectionpaths.push_back(path);
	}
	selectionframes = loadFrames(selectionpaths);
	SpriteAnimation* selectionanimation = new SpriteAnimation(100, &selectionframes, 10);
	selectionanimator.addAnimation(selectionanimation);

	humanshipxlimit = 1600.0f - platform.getWidth() / 2.0f;
	aishipxlimit = 577.0f + platform.getWidth() / 2.0f;
}

void gCanvas::update() {
	float deltaTime = appmanager->getElapsedTime();
	float nearestaishipx = humanshipxlimit;
	for(Ship* ship : activeaiships) {
		float aishipx = ship->x - platform.getWidth() / 2.0f;
		if (aishipx < nearestaishipx) {
			nearestaishipx = aishipx;
		}
	}
	float nearesthumanshipx = aishipxlimit;
	for(Ship* ship : activehumanships) {
		float humanshipx = ship->x + platform.getWidth() / 2.0f;
		if (humanshipx > nearesthumanshipx) {
			nearesthumanshipx = humanshipx;
		}
	}
	// Human ships move towards AI ships
	for(Ship* ship : activehumanships) {
		ship->x = gClamp(ship->x + ship->speed * deltaTime, 0.0f, nearestaishipx);
		updateShip(ship, PLAYER_HUMAN, deltaTime);
	}
	// AI ships move towards human ships
	for(Ship* ship : activeaiships) {
		ship->x = gClamp(ship->x - ship->speed * deltaTime, nearesthumanshipx, (float)getWidth());
		updateShip(ship, PLAYER_AI, deltaTime);
	}
	for (Bullet* bullet : humanbullets) {
		bullet->x += bullet->speed * deltaTime;
	}
	for (Bullet* bullet : aibullets) {
		bullet->x -= bullet->speed * deltaTime;
	}
	selectionanimator.update(deltaTime);
}

void gCanvas::draw() {
	map.draw(0,0);
	ship1.draw(60,20);
	ship2.draw(90,310);
	ship3.draw(47,550);
	ship4.draw(110,900);
	// Oyuncunun Sol Üst Platformu
	platform.draw(platformx[PLAYER_HUMAN][PLATFORM_TOP] - platform.getWidth() / 2,
			platformy[PLAYER_HUMAN][PLATFORM_TOP] - platform.getHeight() / 2,
			platform.getWidth(),platform.getHeight(),90);
	// Oyuncunun Sol Alt Platformu
	platform.draw(platformx[PLAYER_HUMAN][PLATFORM_BOTTOM] - platform.getWidth() / 2,
			platformy[PLAYER_HUMAN][PLATFORM_BOTTOM] - platform.getHeight() /2,
			platform.getWidth(),platform.getHeight(),90);
    // Düþmanýn Sað Üst Platformu
	platform.draw(platformx[PLAYER_AI][PLATFORM_TOP] - platform.getWidth() / 2,
			platformy[PLAYER_AI][PLATFORM_TOP] - platform.getHeight() / 2,
			platform.getWidth(),platform.getHeight(),-90);
	// Düþmanýn Sað Alt Platformu
	platform.draw(platformx[PLAYER_AI][PLATFORM_BOTTOM] - platform.getWidth() / 2,
			platformy[PLAYER_AI][PLATFORM_BOTTOM] - platform.getHeight() / 2,
			platform.getWidth(),platform.getHeight(),-90);
	if (selectedplatform != PLATFORM_NONE) {
		int x = platformx[PLAYER_HUMAN][selectedplatform];
		int y = platformy[PLAYER_HUMAN][selectedplatform];
		selectionanimator.draw(x, y, -30);
	}
	for (Bullet* bullet : humanbullets) {
		humanbullet.draw(
			bullet->x - humanbullet.getWidth() / 2.0f,
			bullet->y - humanbullet.getHeight() / 2.0f,
			humanbullet.getWidth(),
			humanbullet.getHeight(),
			bullet->rotation
		);
	}
	for (Bullet* bullet : aibullets) {
		aibullet.draw(
			bullet->x - aibullet.getWidth() / 2.0f,
			bullet->y - aibullet.getHeight() / 2.0f,
			aibullet.getWidth(),
			aibullet.getHeight(),
			bullet->rotation
		);
	}
	for(Ship* ship : activehumanships) {
		ship->animator.draw(ship->x, ship->y, ship->rotation);
	}
	for(Ship* ship : activeaiships) {
		ship->animator.draw(ship->x, ship->y, ship->rotation);
	}
}

void gCanvas::addShip(AnimationFrames* frames,
		PlayerType playertype, PlatformPos platformpos) {
	Ship* ship = new Ship();
	ship->x = platformx[playertype][platformpos];
	ship->y = platformy[playertype][platformpos];
	ship->speed = 120.0f;
	ship->shoottimer = 0.0f;
	ship->health = 100.0f;
	SpriteAnimation* anim = new SpriteAnimation(0, frames, 10);
	ship->animator.addAnimation(anim);
	switch(playertype) {
	case PLAYER_HUMAN:
		ship->rotation = -90.0f;
		activehumanships.push_back(ship);
		break;
	case PLAYER_AI:
		ship->rotation = 90.0f;
		activeaiships.push_back(ship);
		break;
	default:
		ship->rotation = 0.0f;
		break;
	}
}

void gCanvas::addBullet(float x, float y, float speed, PlayerType playertype) {
	Bullet* bullet = new Bullet();
	bullet->x = x;
	bullet->y = y;
	bullet->speed = speed;
	bullet->rotation = 0.0f;
	switch(playertype) {
	case PLAYER_HUMAN:
		humanbullets.push_back(bullet);
		break;
	case PLAYER_AI:
		aibullets.push_back(bullet);
		break;
	default:
		break;
	}
}

void gCanvas::updateShip(Ship* ship, PlayerType playertype, float deltaTime) {
	ship->animator.update(deltaTime);
	ship->shoottimer += deltaTime;
	float limit = 1.0f / 2.0f; // 500 ms
	while (ship->shoottimer >= limit) {
		ship->shoottimer -= limit;
		addBullet(
			ship->x + 50,
			ship->y,
			ship->speed * 10.0f,
			playertype
		);
	}
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

void gCanvas::mouseReleased(int mousex, int mousey, int button) {
//	gLogi("gCanvas") << "mouseReleased" << ", button:" << button;
	int halfwidth = platform.getWidth() / 2;
	int halfheight = platform.getHeight() / 2;
	for(int pos = 0; pos < MAX_PLATFORM_NUM; pos++) {
		int x = platformx[PLAYER_HUMAN][pos];
		int y = platformy[PLAYER_HUMAN][pos];
		if (x - halfwidth <= mousex && y - halfheight <= mousey && x + halfwidth >= mousex && y + halfheight >= mousey) {
			selectedplatform = (PlatformPos)pos;
			break;
		}
	}
	gLogi("gCanvas") << "pos: " << selectedplatform;
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
