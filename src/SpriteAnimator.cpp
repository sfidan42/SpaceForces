/*
 * SpriteAnimator.cpp
 *
 *  Created on: 2 Tem 2025
 *      Author: yzctr
 */

#include "SpriteAnimator.h"

AnimationFrames::AnimationFrames(){}
AnimationFrames::~AnimationFrames(){
	for(gImage* img : images){
		delete img;
	}
}
AnimationFrames::AnimationFrames(AnimationFrames&& other){
	images = other.images;
	other.images.clear();
}

AnimationFrames& AnimationFrames::operator=(AnimationFrames&& other){
	images = other.images;
	other.images.clear();
	return *this;
}

AnimationFrames loadFrames(std::vector<std::string> list){
	AnimationFrames aframes;
	aframes.images.reserve(list.size());
	for(const std::string& path : list){
		gImage* img = new gImage();
		img->loadImage(path);
		aframes.images.push_back(img);
	}
	return aframes;
}
SpriteAnimation::SpriteAnimation(int id, AnimationFrames* frames, int fps) {
	this->id = id;
	this->frames =frames;
	this->fps = fps;
	currentframe = 0;
	timer = 0;
}

gImage* SpriteAnimation::getCurrentFrame() {
	if(frames->images.empty()) {
		return nullptr;
	}
	return frames->images[currentframe % frames->images.size()];
}

void SpriteAnimation::update(float deltaTime) {
	if(frames->images.empty()) {
		return;
	}
	timer += deltaTime;
	float frameduration = 1.0f / fps;
	while(timer >= frameduration) {
		currentframe = (currentframe + 1) % frames->images.size();
		timer -= frameduration;
	}
}

SpriteAnimator::SpriteAnimator() {
	currentanim = nullptr;
}

SpriteAnimator::~SpriteAnimator() {
	for(int i = 0; i < animations.size(); i++) {
		delete animations[i];
	}
}

void SpriteAnimator::addAnimation(SpriteAnimation *anim) {
	if(!anim) {
		gLoge("SpriteAnimator") << "anim should not be null";
		return;
	}
	animations.push_back(anim);
	if(!currentanim) {
		currentanim = anim;
	}
}

void SpriteAnimator::changeAnimation(int id) {
	for(SpriteAnimation* anim : animations) {
		if(anim->getId() == id) {
			currentanim = anim;
			return;
		}
	}
	gLoge("SpriteAnimator") << id << " not found";
}

void SpriteAnimator::update(float deltaTime) {
	if(currentanim) {
		currentanim->update(deltaTime);
	}
}

void SpriteAnimator::draw(int x, int y) {
	if(currentanim) {
		gImage* currentframe = currentanim->getCurrentFrame();
		if(currentframe) {
			currentframe->draw(x - currentframe->getWidth() / 2, y - currentframe->getHeight() / 2);
		}

	}
}

void SpriteAnimator::draw(int x, int y, float rotation){
	if(currentanim) {
		gImage* currentframe = currentanim->getCurrentFrame();
		if(currentframe) {
			currentframe->draw(x - currentframe->getWidth() / 2, y - currentframe->getHeight() / 2,
					currentframe->getWidth(), currentframe->getHeight(), rotation);
		}

	}
}






