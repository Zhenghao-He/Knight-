#pragma once
#ifndef __SPRITE_H__
#define __SPRITE_H__
#include "cocos2d.h"
#include"HelloWorldScene.h"

USING_NS_CC;
using namespace ui;
USING_NS_CC_EXT;

class character:public cocos2d::Sprite
{
private :
	int blood;
	int magic;
public:
//static cocos2d::Sprite* create()=0;
	virtual bool init()=0;
	virtual void Move(float a)=0;
	virtual void attack(int wapon)=0;
	//CREATE_FUNC(character);
};
class hero :public cocos2d::Sprite
{
private:
	int blood;
	int magic;
public:
	friend class HelloWorld;
	static hero* create(const std::string& filename);

	hero(int blood, int magic) :blood(blood), magic(magic) {};
	int getblood() { return blood; }
	int getmagic() { return magic; }
	virtual bool init(const std::string& filename);
	virtual void Move( float a);
	//virtual void attack(int wapon)=0;
	void onKeyPressed(EventKeyboard::KeyCode keycode, Event* event);

    void onKeyReleased(EventKeyboard::KeyCode keycode, Event* event);

	bool isCanReach(const std::string& filename, Vec2 dir);

	Vec2 tileCoordForPosition(const Vec2& position);

	bool onTouchBegan(Touch* touch, Event* unused_event);
	//CREATE_FUNC(Sprite);
};
#endif// __sprite_H__

