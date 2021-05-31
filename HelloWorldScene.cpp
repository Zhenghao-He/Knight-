#include "HelloWorldScene.h"
#include"security map.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }
    
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    ///////////////////////////////////////////
    //title
    auto label = Label::createWithTTF("Vigour Knight", "fonts/Marker Felt.ttf",72);
    //label->setOpacity(100);
    label->enableShadow(Color4B::RED, Size(3, 3));
    label->enableGlow(Color4B::RED);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    auto start=Label::createWithTTF("START", "fonts/Marker Felt.ttf", 48);
    start->enableGlow(Color4B::WHITE);
   
    auto sprite = Sprite::create("begin.png");
    sprite->setScale(2);
    if (sprite == nullptr)
    {
        problemLoading("'begin.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    //////////////////////////////////////
    //button

    auto buttonStart = MenuItemLabel::create(start, CC_CALLBACK_1(HelloWorld::func, this));
   //auto buttonStart = MenuItemLabel::create(start, CC_CALLBACK_1(security::func, this));
    auto menu2 = Menu::create(buttonStart, NULL);
    this->addChild(menu2);

    return true;
}

void HelloWorld::func(cocos2d::Ref* pSender)
{
    auto map = TMXTiledMap::create("safeMap.tmx");
    addChild(map, 2);

}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
