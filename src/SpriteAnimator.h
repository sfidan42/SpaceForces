/*
 * SpriteAnimator.h
 *
 *  Created on: 2 Tem 2025
 *      Author: yzctr
 */

#ifndef SRC_SPRITEANIMATOR_H_
#define SRC_SPRITEANIMATOR_H_

#include "gImage.h"
#include <string>

struct AnimationFrames{
	AnimationFrames();
	~AnimationFrames();
	//disable copy constructors
	AnimationFrames(const AnimationFrames& other) = delete;
	//enable move constructor
	AnimationFrames(AnimationFrames&& other);
	AnimationFrames& operator=(AnimationFrames&& other);

	std::vector<gImage*> images;
};

/*
 loadFrames({"gemiler/ship_1/ship_1_1.png","gemiler/ship_2/ship_2_1.png",
 "gemiler/ship_3/ship_3_1.png","gemiler/ship_4/ship_4_1.png"})
 */
AnimationFrames loadFrames(std::vector<std::string> list);

class SpriteAnimation {
public:
	SpriteAnimation(int id,AnimationFrames* frames ,int fps = 6);

	gImage* getCurrentFrame();
	int getId() {
		return id;
	}

	void update(float deltaTime);

private:
	int id;
	AnimationFrames* frames;
	int currentframe;
	int fps;
	float timer;
};

class SpriteAnimator {
public:
	SpriteAnimator();
	~SpriteAnimator();

	void addAnimation(SpriteAnimation* anim);
	void changeAnimation(int id);

	void update(float deltaTime);
	void draw(int x, int y);

private:

	std::vector<SpriteAnimation*> animations;
	SpriteAnimation* currentanim;
};

#endif /* SRC_SPRITEANIMATOR_H_ */








