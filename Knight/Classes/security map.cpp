#include "security map.h"


USING_NS_CC;


//class
Scene* security::createScene()
{
    return security::create();
}
bool security::init()
{
    if (!Scene::init())
    {
        return false;
    }
    return true;
}
void security::func(cocos2d::Ref* pSender)
{
    auto map = TMXTiledMap::create("safeMap.tmx");
    addChild(map);
}