#include"sprite.h"
std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
extern TMXTiledMap* myMap;
USING_NS_CC;
hero* hero::create(const std::string& filename) {
    hero* myhero = new hero(10,10);
    if( myhero->init(filename)) {
        myhero->autorelease();//有程序控制sprite的生命周期，
    }
    else
    {
        delete myhero;
        myhero = NULL;
        return NULL;
    }
    return myhero;
}
bool hero::init(const std::string& filename)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    if (!Sprite::initWithFile(filename)) {
        return false;
    }
    setScale(0.1f);
    setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));

    auto* dispatcher = Director::getInstance()->getEventDispatcher();
    auto* keyListener = EventListenerKeyboard::create();
    //创建一个事件监听器监听键盘事件
    keyListener->onKeyPressed = CC_CALLBACK_2(hero::onKeyPressed, this);
    //键盘被按下时响应

    keyListener->onKeyReleased = CC_CALLBACK_2(hero::onKeyReleased, this);

    //键盘按键被弹回时响应

    dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);


    auto eventListener = EventListenerTouchOneByOne::create();

    eventListener->onTouchBegan = CC_CALLBACK_2(hero::onTouchBegan, this);

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

    return true;
}

bool hero::onTouchBegan(Touch* touch, Event* unused_event)
{
    CCLOG("1234");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    // 1 - Just an example for how to get the player object
    // 说明一下作为第二个参数传递给addEventListenerWithSceneGraphPriority(eventListener, _player)的_player对象被访问的方式。
    // auto node = unused_event->getcurrentTarget();
    static float existMp = 100;
    // 2.获取触摸点的坐标，并计算这个点相对于_player的偏移量。
    if (existMp > 0)
    {
        CCLOG("5678");
        Vec2 touchLocation = touch->getLocation();
        Vec2 offset = touchLocation - Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2);
        // 3.在玩家所在的位置创建一个飞镖，将其添加到场景中。
        auto projectile = Sprite::create("Projectile.png");
        projectile->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2));

        // Add projectile's physicsBody
        auto physicsBody = PhysicsBody::createBox(projectile->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
        //physicsBody->setDynamic(false);
        //physicsBody->setCategoryBitmask(5);
       // physicsBody->setContactTestBitmask(2);
        //projectile->setPhysicsBody(physicsBody);
        projectile->setTag(MY_PROJECTILE);
        projectile->setScale(10);
        this->addChild(projectile);

        // 4.将偏移量转化为单位向量，即长度为1的向量。
        offset.normalize();
        // 将其乘以1000，你就获得了一个指向用户触屏方向的长度为1000的向量。为什么是1000呢？因为长度应当足以超过当前分辨率下屏幕的边界。
        auto shootAmount = offset * 10000;
        // 将此向量添加到飞镖的位置上去，这样你就有了一个目标位置。
        auto realDest = shootAmount + projectile->getPosition();

        // 5.创建一个动作，将飞镖在2秒内移动到目标位置，然后将它从场景中移除。
        auto actionMove = MoveTo::create(2.0f, realDest);
        auto actionRemove = RemoveSelf::create();
        projectile->runAction(Sequence::create(actionMove, actionRemove, nullptr));
       // existMp--;
      //  auto progress = (ProgressTimer*)this->getChildByTag(PLAYER_MP_BAR);
        //progress->setPercentage((((float)existMp) / PLAYER_MP) * 100);

    }
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
    if (isCanReach("barrier", dir)) {
        auto move = MoveTo::create(0.2f, dir);
        myMap->runAction(move);
    }
    else {
        myMap->stopAllActions();
    }
}
void hero::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
    keyMap[keycode] = true;
    schedule(CC_SCHEDULE_SELECTOR(hero::Move), 0.0f);
}

void hero::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    keyMap[keycode] = false;
}
/*把cocos地图坐标转换为瓦片地图的坐标*/
Vec2 hero::tileCoordForPosition(const Vec2& position)
{
    Size mapSize = myMap->getMapSize();      // 获取以tiles数量为单位的地图尺寸
    Size tileSize = myMap->getTileSize();    // 获取以像素点为单位的tile尺寸属性
    float x = (position.x) / (tileSize.width-2);
    float y = (mapSize.height)  - (position.y) / (tileSize.height-2);
    return Vec2(x,y);
}
bool hero::isCanReach(const std::string& filename,Vec2 dir)
{
    //myMap->setAnchorPoint(myMap->getPosition());
    bool result;
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 position =Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2)-dir;
    // 获取瓦片的Gid值
    auto barrier = myMap->getLayer(filename);
    int tiledGid = barrier->getTileGIDAt(tileCoordForPosition(position));//中gid值
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
