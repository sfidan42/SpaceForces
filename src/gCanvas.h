 /*
 * gCanvas.h
 *
 *  Created on: May 6, 2020
 *      Author: noyan
 */

#ifndef GCANVAS_H_
#define GCANVAS_H_

#include "gBaseCanvas.h"
#include "gApp.h"
#include "gImage.h"
#include "SpriteAnimator.h"

class Ship {
public:
	float x,y;
	float rotation;
	float speed;
	float shoottimer;
	float health;
	SpriteAnimator animator;
};

class Bullet {
public:
	float x,y;
	float rotation;
	float speed;
};

class gCanvas : public gBaseCanvas {
public:
	gCanvas(gApp* root);
	virtual ~gCanvas();

	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void charPressed(unsigned int codepoint);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseScrolled(int x, int y);
	void mouseEntered();
	void mouseExited();
	void windowResized(int w, int h);

	void showNotify();
	void hideNotify();


private:
	enum PlayerType{
		PLAYER_HUMAN,    //0
		PLAYER_AI,       //1
		MAX_PLAYER_NUM   //2
	};
	enum PlatformPos{
		PLATFORM_NONE = -1,
		PLATFORM_TOP,    //0
		PLATFORM_MIDDLE, //1
		PLATFORM_BOTTOM, //2
		MAX_PLATFORM_NUM //3
	};
	void addShip(AnimationFrames* frames, PlayerType playertype,
			PlatformPos platformpos);
	void addBullet(float x, float y, float speed, PlayerType playertype);
	void updateShip(Ship* ship, PlayerType playertype, float deltaTime);

	gApp* root;
	gImage map;
	gImage ship1, ship2, ship3, ship4;
	gImage platform;
	gImage humanbullet, aibullet;
	std::vector<Bullet *> humanbullets, aibullets;
	AnimationFrames ship1frames;
	AnimationFrames selectionframes;
	std::vector<Ship*> activehumanships;
	std::vector<Ship*> activeaiships;
	float humanshipxlimit, aishipxlimit;
	int platformx[MAX_PLAYER_NUM][MAX_PLATFORM_NUM];
	int platformy[MAX_PLAYER_NUM][MAX_PLATFORM_NUM];
	PlatformPos selectedplatform;
    SpriteAnimator selectionanimator;
};

#endif /* GCANVAS_H_ */
