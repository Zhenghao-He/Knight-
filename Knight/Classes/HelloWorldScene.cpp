#include "HelloWorldScene.h"
extern TMXTiledMap* myMap;
extern hero* Hero;
extern Sprite* _player;
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
    //////////////
    //music

    
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("text.mp3");
    SimpleAudioEngine::getInstance()->playBackgroundMusic("text.mp3");
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.3);
    auto slider = ControlSlider::create("background.png", "sliderProgress.png", "buttonBackground.png");//����������
    slider->setPosition(Vec2(100,100));//���û���������
    slider->setMaximumValue(100);//���û��������ֵ
    slider->setMinimumValue(0);//���û�������Сֵ
    slider->setValue(50);//���û�������ǰֵ
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(HelloWorld::sliderChange), Control::EventType::VALUE_CHANGED);
    this->addChild(slider);

    //////////////////////////////////////
    //button
    auto start = Label::createWithTTF("START", "fonts/Marker Felt.ttf", 48);
    start->enableGlow(Color4B::WHITE);

    auto buttonStart = MenuItemLabel::create(start, CC_CALLBACK_1(HelloWorld::safeMap, this));
  //auto buttonStart = MenuItemLabel::create(start, CC_CALLBACK_1(security::func, this));
    auto menu2 = Menu::create(buttonStart, NULL);
    this->addChild(menu2);



    return true;
}

void HelloWorld::safeMap(cocos2d::Ref* pSender)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
   
    removeAllChildren();
  
    Hero = hero::create("knight1.png");
    myMap = TMXTiledMap::create("safeMap2.tmx");
    addChild(myMap);
    Hero->init("knight1.png");
    addChild(Hero,4);

    auto weapon = Label::createWithTTF("weapon", "fonts/Marker Felt.ttf", 48);
    weapon->enableGlow(Color4B::WHITE);
    
    auto weaponInfo = MenuItemLabel::create(weapon, CC_CALLBACK_1(HelloWorld::WeaponAndEnemyInfo, this,"weaponAll.png"));
    auto menu3 = Menu::create(weaponInfo, NULL);
    menu3->setPosition(Vec2(origin.x + visibleSize.width / 4,
                            origin.y + visibleSize.height / 4));
    myMap->addChild(menu3, 1);

    auto enemy = Label::createWithTTF("enemy", "fonts/Marker Felt.ttf", 48);
    auto enemyInfo = MenuItemLabel::create(enemy, CC_CALLBACK_1(HelloWorld::WeaponAndEnemyInfo, this, "enemyAll.png"));
    auto menu4 = Menu::create(enemyInfo, NULL);
    menu4->setPosition(Vec2(origin.x + 3*visibleSize.width / 4,
        origin.y + 3*visibleSize.height / 4));

    myMap->addChild(menu4, 1);
    auto tansfer = Sprite::create("tansfer.png");
    tansfer->setPosition(Vec2(origin.x + visibleSize.width-100,
        origin.y + visibleSize.height/2));
    myMap->addChild(tansfer, 2);

    this->scheduleUpdate();//�����û�е�������

    


}

void HelloWorld::func(cocos2d::Ref* pSender)
{
    this->unscheduleUpdate();
    myMap->removeAllChildren();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    myMap = TMXTiledMap::create("win.tmx");
    addChild(myMap);
    /////////////////////////////////////////////////////////////////

    auto winSize = cocos2d::Size(480 * 2+100, 330 * 2);
    auto sprite = Sprite::create("PlayerBackgroundUI.png");   //�������ȿ�
    sprite->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.9)); //���ÿ��λ��
    sprite->setScale(0.5f);
    this->addChild(sprite);            //�ӵ�Ĭ��ͼ������ȥ

    auto PlayerHpUI = Sprite::create("PlayerHpUI.png");  //����Ѫ��
    ProgressTimer* progressHp = ProgressTimer::create(PlayerHpUI); //����progress����
    progressHp->setType(ProgressTimer::Type::BAR);        //���ͣ���״
    progressHp->setPosition(Vec2(winSize.width * 0.115, winSize.height * 0.955));
    //���ҵ������Ѫ��
    progressHp->setMidpoint(Point(0, 0.5));     //����Ǵ����ҵĻ����ĳ�(1,0.5)����
    progressHp->setBarChangeRate(Point(1, 0));
    progressHp->setPercentage(5 / PLAYER_HP * 100);
    progressHp->setTag(PLAYER_HP_BAR);       //��һ�����
    progressHp->setScale(0.5f);
    this->addChild(progressHp);

    auto PlayerDefendeseUI = Sprite::create("PlayerDefendeseUI.png");  //����dunpai
    ProgressTimer* progressDefendese = ProgressTimer::create(PlayerDefendeseUI); //����progress����
    progressDefendese->setType(ProgressTimer::Type::BAR);        //���ͣ���״
    progressDefendese->setPosition(Vec2(winSize.width * 0.115, winSize.height * 0.905));
    progressDefendese->setMidpoint(Point(0, 0.5));     //����Ǵ����ҵĻ����ĳ�(1,0.5)����
    progressDefendese->setBarChangeRate(Point(1, 0));
    progressDefendese->setPercentage(5 / PLAYER_DEFENDESE * 100);
    progressDefendese->setTag(PLAYER_DEFENDESE_BAR);       //��һ�����
    progressDefendese->setScale(0.5f);
    this->addChild(progressDefendese);


    auto PlayerMpUI = Sprite::create("PlayerMpUI.png");  //����Ѫ��
    ProgressTimer* progressMp = ProgressTimer::create(PlayerMpUI); //����progress����
    progressMp->setType(ProgressTimer::Type::BAR);        //���ͣ���״
    progressMp->setPosition(Vec2(winSize.width * 0.115, winSize.height * 0.85));
    //���ҵ������Ѫ��
    progressMp->setMidpoint(Point(0, 0.5));     //����Ǵ����ҵĻ����ĳ�(1,0.5)����
    progressMp->setBarChangeRate(Point(1, 0));
    progressMp->setPercentage(100 / PLAYER_MP * 100);
    progressMp->setTag(PLAYER_MP_BAR);       //��һ�����
    progressMp->setScale(0.5f);
    this->addChild(progressMp);
    //schedule(SEL_SCHEDULE(NA_Level1::scheduleBlood), 0.1f);  //ˢ�º�����ÿ��0.1��
    ///////////////////////////////////////////////////////////////
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}
void HelloWorld::WeaponAndEnemyInfo(cocos2d::Ref* pSender, const std::string& filename)
{
    this->unscheduleUpdate();
    Hero->setVisible(0);
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto sprite = Sprite::create(filename);
    sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    sprite->setScale(1.5f);
    myMap->addChild(sprite,3);
    auto back = MenuItemImage::create(
        "backUnpressed.png",
        "backIspressed.png",
        CC_CALLBACK_1(HelloWorld::safeMap, this));
    auto backButton = Menu::create(back, NULL);
    backButton->setPosition(Vec2(0,0));
    backButton->setScale(0.4f);
    this->addChild(backButton, 2);
  

}

void HelloWorld::update(float a)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    Vec2 des = -Vec2(origin.x + visibleSize.width - 100,
        origin.y + visibleSize.height / 2)/2;
    //if (myMap->getPosition()==des)
    if ((myMap->getPosition().x <= des.x + 100 && myMap->getPosition().x >= des.x - 100) &&
        (myMap->getPosition().y <= des.y + 200 && myMap->getPosition().y >= des.y - 200))
    {
        HelloWorld::func(this);

    }
    else {
        CCLOG("%f", myMap->getPosition().x - des.x);
        CCLOG("%f", myMap->getPosition().y - des.y);
    }
}
void HelloWorld::sliderChange(Ref* psender, Control::EventType event)
{
    /*
    ControlSlider* slider = (ControlSlider*)psender;
    String* valueStr = String::createWithFormat("%f", slider->getValue());
    log(valueStr->getCString());
    */

    ControlSlider* pslider = (ControlSlider*)psender;
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume((pslider->getValue()) / 50.0); // �ѻ����ֵ��
    //CCLOG("%f", pslider->getValue() * 5);
    //�����ʱ��������λ�ô����ֵ

}
bool HelloWorld::onTouchBegan(Touch* touch, Event* unused_event)
{
    // 1 - Just an example for how to get the player object
    // ˵��һ����Ϊ�ڶ����������ݸ�addEventListenerWithSceneGraphPriority(eventListener, _player)��_player���󱻷��ʵķ�ʽ��
    // auto node = unused_event->getcurrentTarget();
    static float existMp = 100;
    // 2.��ȡ����������꣬����������������_player��ƫ������


        Vec2 touchLocation = touch->getLocation();
        Vec2 offset = touchLocation - _player->getPosition();
 

        // 3.��������ڵ�λ�ô���һ�����ڣ�������ӵ������С�
        auto projectile = Sprite::create("Projectile.png");
        projectile->setPosition(_player->getPosition());

        // Add projectile's physicsBody
        auto physicsBody = PhysicsBody::createBox(projectile->getContentSize(), PhysicsMaterial(0.0f, 0.0f, 0.0f));
        physicsBody->setDynamic(false);
        physicsBody->setCategoryBitmask(5);
        physicsBody->setContactTestBitmask(2);
        projectile->setPhysicsBody(physicsBody);
        projectile->setTag(MY_PROJECTILE);

        myMap->addChild(projectile);

        // 4.��ƫ����ת��Ϊ��λ������������Ϊ1��������
        offset.normalize();
        // �������1000����ͻ����һ��ָ���û���������ĳ���Ϊ1000��������Ϊʲô��1000�أ���Ϊ����Ӧ�����Գ�����ǰ�ֱ�������Ļ�ı߽硣
        auto shootAmount = offset * 1000;
        // ����������ӵ����ڵ�λ����ȥ�������������һ��Ŀ��λ�á�
        auto realDest = shootAmount + projectile->getPosition();

        // 5.����һ����������������2�����ƶ���Ŀ��λ�ã�Ȼ�����ӳ������Ƴ���
        auto actionMove = MoveTo::create(2.0f, realDest);
        auto actionRemove = RemoveSelf::create();
        projectile->runAction(Sequence::create(actionMove, actionRemove, nullptr));
     /*   existMp--;
        auto progress = (ProgressTimer*)this->getChildByTag(PLAYER_MP_BAR);
        progress->setPercentage((((float)existMp) / PLAYER_MP) * 100);*/

    return true;
}