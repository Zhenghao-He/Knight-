#pragma once
#ifndef __SECURITY_SCENE_H__
#define __SECURITY_SCENE_H__
#include "cocos2d.h"
//class
#include"ui/CocosGUI.h"
#include "cocos-ext.h"//使用按钮事件
#include "ui\UIScale9Sprite.h"
#include"cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace ui;
USING_NS_CC_EXT;
class security : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void func(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(security);
};
#endif // __SECURITY_SCENE_H__
