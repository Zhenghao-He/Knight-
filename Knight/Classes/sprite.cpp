#include"sprite.h"
std::map<cocos2d::EventKeyboard::KeyCode, bool> keyMap;
extern TMXTiledMap* myMap;
USING_NS_CC;
hero* hero::create(const std::string& filename) {
    hero* myhero = new hero(10,10);
    if( myhero->init(filename)) {
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
    //����һ���¼����������������¼�
    keyListener->onKeyPressed = CC_CALLBACK_2(hero::onKeyPressed, this);
    //���̱�����ʱ��Ӧ

    keyListener->onKeyReleased = CC_CALLBACK_2(hero::onKeyReleased, this);

    //���̰���������ʱ��Ӧ

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
    // ˵��һ����Ϊ�ڶ����������ݸ�addEventListenerWithSceneGraphPriority(eventListener, _player)��_player���󱻷��ʵķ�ʽ��
    // auto node = unused_event->getcurrentTarget();
    static float existMp = 100;
    // 2.��ȡ����������꣬����������������_player��ƫ������
    if (existMp > 0)
    {
        CCLOG("5678");
        Vec2 touchLocation = touch->getLocation();
        Vec2 offset = touchLocation - Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2);
        // 3.��������ڵ�λ�ô���һ�����ڣ�������ӵ������С�
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

        // 4.��ƫ����ת��Ϊ��λ������������Ϊ1��������
        offset.normalize();
        // �������1000����ͻ����һ��ָ���û���������ĳ���Ϊ1000��������Ϊʲô��1000�أ���Ϊ����Ӧ�����Գ�����ǰ�ֱ�������Ļ�ı߽硣
        auto shootAmount = offset * 10000;
        // ����������ӵ����ڵ�λ����ȥ�������������һ��Ŀ��λ�á�
        auto realDest = shootAmount + projectile->getPosition();

        // 5.����һ����������������2�����ƶ���Ŀ��λ�ã�Ȼ�����ӳ������Ƴ���
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
/*��cocos��ͼ����ת��Ϊ��Ƭ��ͼ������*/
Vec2 hero::tileCoordForPosition(const Vec2& position)
{
    Size mapSize = myMap->getMapSize();      // ��ȡ��tiles����Ϊ��λ�ĵ�ͼ�ߴ�
    Size tileSize = myMap->getTileSize();    // ��ȡ�����ص�Ϊ��λ��tile�ߴ�����
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
    // ��ȡ��Ƭ��Gidֵ
    auto barrier = myMap->getLayer(filename);
    int tiledGid = barrier->getTileGIDAt(tileCoordForPosition(position));//��gidֵ
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
