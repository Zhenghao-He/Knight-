#include"sprite.h"
extern TMXTiledMap* myMap;
USING_NS_CC;
std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
int n = 0;
hero* hero::create() {
    hero* myhero = new hero(10,10);
    if( myhero->init()) {
        myhero->autorelease();//�г������sprite���������ڣ�
    }
    else
    {
        delete myhero;
        myhero = NULL;
        return NULL;
    }
    return myhero;
}
bool hero::init()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if (!Sprite::initWithFile("knight1.png")) {
        return false;
    }
    setScale(0.1f);
    setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));

    auto* dispatcher = Director::getInstance()->getEventDispatcher();
    auto* keyListener = EventListenerKeyboard::create();
    //����һ���¼����������������¼�
    keyListener->onKeyPressed = CC_CALLBACK_2(hero::onKeyPressed, this);
    //���̱�����ʱ��Ӧ

    keyListener->onKeyReleased = CC_CALLBACK_2(hero::onKeyReleased, this);

    //���̰���������ʱ��Ӧ

    dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);


    return true;
}
void hero::Move(float a)
{
    auto left = cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW;
    auto up = cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW;
    auto right = cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW;
    auto down = cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW;
    float offsetX = 0, offsetY = 0;
    float v = 5;
    if (keyMap[left]) {
        offsetX = v;
    }
    else if (keyMap[right]) {
        offsetX = -v;
    }
    else if (keyMap[up]) {
        offsetY = -v;
    }
    else if (keyMap[down]) {
        offsetY = v;
    }
    Vec2 dir = Vec2(myMap->getPosition().x + offsetX, myMap->getPosition().y + offsetY);
    if (isCanReach(myMap, dir)) {
        auto move = MoveTo::create(0.2f, dir);
        myMap->runAction(move);
        //CCLOG("%d", 0);
    }
    else {
      /*  Vec2 dir1 = Vec2(myMap->getPosition().x - offsetX/5, myMap->getPosition().y - offsetY/5);
        auto move = MoveTo::create(0.2f, dir1);*/
        myMap->stopAllActions();
        //n++;
        //CCLOG("%d",n);
    }
}
void hero::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
    keyMap[keycode] = true;
    schedule(CC_SCHEDULE_SELECTOR(hero::Move), 0.0f);
}

void hero::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    //this->unschedule(CC_SCHEDULE_SELECTOR(hero::Move));
    keyMap[keycode] = false;

}
/*��cocos��ͼ����ת��Ϊ��Ƭ��ͼ������*/
Vec2 hero::tileCoordForPosition(const Vec2& position, TMXTiledMap* mymap)
{
    Size mapSize = mymap->getMapSize();      // ��ȡ��tiles����Ϊ��λ�ĵ�ͼ�ߴ�
    Size tileSize = mymap->getTileSize();    // ��ȡ�����ص�Ϊ��λ��tile�ߴ�����
    float x = position.x / tileSize.width;
    float y = (mapSize.height * tileSize.height - position.y) / tileSize.height;
    return Vec2(x, y);

   /* Size mapSize = mymap->getMapSize();
    Size tileSize = mymap->getTileSize();
    int x = position.x * tileSize.width + tileSize.width / 2;
    int y = (mapSize.height - position.y) * tileSize.height - tileSize.height / 2;
    return Vec2(x, y);*/
}
bool hero::isCanReach(TMXTiledMap* mymap,Vec2 dir)
{
    bool result;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 position = Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2)-dir;
    // ��ȡ��Ƭ��Gidֵ
    auto barrier = mymap->getLayer("barrier");
    int tiledGid = barrier->getTileGIDAt(tileCoordForPosition(position,mymap));//��gidֵ
    if (tiledGid )
    {
        result = false;
    }
    else
    {
        result = true;
    }
    return result;
}