#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "BirdLayer.h"
#include "MainLayer.h"
#include "RankBirdLayer.h"
#include "Collision.h"

using namespace cocos2d;
using std::string;

bool BirdLayer::init() {
  if (!CCLayer::init())  // 调用父类的初始化
    return false;
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  if (random() % 2)
    backGround = Sprite::create("pic/morning.png");  // 创建一个背景精灵对象
  else
    backGround = Sprite::create("pic/night.png");  // 创建一个背景精灵对象
  backGround->setAnchorPoint(Vec2::ZERO);  // 设置背景精灵锚点
  backGround->setPosition(  // 设置背景精灵对象的位置
    Vec2(origin.x, origin.y + visibleSize.height - backGround->getContentSize().height));
  this->addChild(backGround, 0);  // 将背景精灵添加到布景中

  floor = Sprite::create("pic/floor.png");  // 创建地面精灵对象
  floor->setAnchorPoint(Vec2::ZERO);  //设置地面精灵锚点
  floor->setPosition(Vec2(origin.x, origin.y));  // 设置地面精灵对象位置
  this->addChild(floor);  // 将地面精灵添加到布景中
  Size floorSize = floor->getContentSize();  // 获取地面精灵尺寸大小
  floor->runAction(RepeatForever::create(  // 设置地面精灵循环滚动
    Sequence::create(MoveTo::create(0.5, Vec2(-120, 0)), MoveTo::create(0, Vec2::ZERO), NULL)));

  effectNode = NodeGrid::create();  // 创建一个网络节点对象
  this->addChild(effectNode, 1);  // 将网格节点添加到布景
  ready = Sprite::create("pic/readyBird.png");  // 游戏预备界面
  ready->setPosition(Vec2(visibleSize.width/2, visibleSize.height * 4 / 7));  // 设置精灵位置
  effectNode->addChild(ready, 10);  // 将精灵添加到网络节点中

  pLabelAtlas = LabelAtlas::create("0", "pic/number.png", 48, 64, '0');  // 创建一个文本标签对象
  pLabelAtlas->setPosition(Vec2(visibleSize.width/2, visibleSize.height/5*4));  // 设置文本标签位置
  this->addChild(pLabelAtlas, 4);  // 将文本标签添加到布景中

  pause = Sprite::create("button/pause.png");  // 创建暂停按钮精灵
  pause->setPosition(Vec2(32, 928));  // 设置精灵位置
  this->addChild(pause, 10);  // 将精灵添加到布景中

  auto listenerPause = EventListenerTouchOneByOne::create();  // 创建暂停精灵触摸监听
  listenerPause->setSwallowTouches(true);  // 设置下传触摸
  listenerPause->onTouchBegan = CC_CALLBACK_2(BirdLayer::onTouchPause, this);  // 开始触摸时回调onTouchBegan方法
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerPause, pause);  // 添加到监听器

  initBird();  // 初始化小鸟
  Size backGroundSize = backGround->getContentSize();  // 获取背景尺寸
  bird = Sprite::create();  // 创建小鸟精灵
  bird->setPosition(Vec2(140 + origin.x, origin.y + floorSize.height + backGroundSize.height/2));  // 设置精灵位置
  this->addChild(bird, 2);  // 将精灵添加到布景中
  bird->runAction(RepeatForever::create(animAc));  // 播放动画

  auto listener = EventListenerTouchOneByOne::create();  // 创建一个单点触摸监听
  listener->setSwallowTouches(true);  // 设置下传触摸
  listener->onTouchBegan = CC_CALLBACK_2(BirdLayer::onTouchBegan, this);  // 开始触摸时回调onTouchBegan方法
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);  // 添加到监听器

  return true;
}

int BirdLayer::random() {  // 生成随机数
  srand((unsigned)time(NULL));  // 用系统时间作为随机种子
  return rand() % 5 + 1;  // 将随机数返回
}

void BirdLayer::initBird() {  // 初始化小鸟动画方法
  auto sfc = SpriteFrameCache::getInstance();  // 获取缓冲帧
  sfc->addSpriteFramesWithFile("pic/bird.plist", "pic/birdP.png");  // 加载小鸟图片和plist文件
  string animBird[3] = {"bird1.png", "bird2.png", "bird3.png"};  // 动画图片的名称
  Vector<SpriteFrame*> animFrames;  // 创建存放动画帧的向量
  for (int i = 0; i < 3; i++) {
    animFrames.pushBack(sfc->getSpriteFrameByName(animBird[i]));  // 获取动画帧
  }
  auto anim = Animation::createWithSpriteFrames(animFrames, 0.1f);  // 创建动画对象，从帧向量产生动画，间隔为0.1秒
  animAc = Animate::create(anim);  // 创建动画动作对象
  animAc->retain();  // 因为暂时不用，保持引用，防止被自动释放
}

void BirdLayer::birdRun() {  // 小鸟运动方法
  auto birdPosition = bird->getPosition();  // 小鸟位置
  Size floorSize = floor->getContentSize();  // 地面尺寸
  float time = (birdPosition.y - 240) / 135;  // 计算下落时间
  bird->runAction(Sequence::create(  // 小鸟下落
    MoveTo::create(time, Vec2(birdPosition.x, floorSize.height + 40)), NULL));
}

void BirdLayer::initColumn1() {
  columnFlag1 = true;  // 设置柱子1标志位为true
  auto visibleSize = Director::getInstance()->getVisibleSize();  // 获取可见区域尺寸
  auto backSize = backGround->getContentSize();  // 获取背景尺寸大小
  auto floorSize = floor->getContentSize();  // 获取地面尺寸大小
  int height1 = 400 / random();  // 计算上柱子高度
  int height2 = backSize.height - height1 - 196;  // 计算下柱子高度
  auto columnNode1 = Sprite::create("pic/column1.png");  // 创建Sprite对象
  auto columnNode2 = Sprite::create("pic/column2.png");  // 创建Sprite对象
  columnUnder1 = Scale9Sprite::create();  // 创建Scale9Sprite对象
  columnUnder1->updateWithSprite(columnNode1, Rect(0, 0, 96, 400), false, Rect(0, 30, 96, 400));  // 设置图像操作区域
  columnUnder1->setAnchorPoint(Vec2::ZERO);  // 设置锚点
  columnUnder1->setContentSize(Size(96, height1));  // 设置图像伸缩
  columnUnder1->setPosition(Vec2(visibleSize.width * (count > 0 ? 1 : 2), floorSize.height));  // 设置位置
  columnOn1 = Scale9Sprite::create();  // 创建Scale9Sprite对象
  columnOn1->updateWithSprite(columnNode2, Rect(0, 0, 96, 400), false, Rect(0, 0, 96, 370));  // 设置图像操作区域
  columnOn1->setAnchorPoint(Vec2::ZERO);  // 设置锚点
  columnOn1->setContentSize(Size(96, height2));  // 设置图像伸缩
  columnOn1->setPosition(Vec2(visibleSize.width * (count > 0 ? 1 : 2), visibleSize.height - height2));  // 设置位置
  this->addChild(columnUnder1, 0);  // 添加到布景中
  this->addChild(columnOn1, 0);  // 添加到布景中
  columnUnder1->runAction(RepeatForever::create(MoveBy::create(0.1, Vec2(-25, 0))));  // 执行向左移动动作
  columnOn1->runAction(RepeatForever::create(MoveBy::create(0.1, Vec2(-25, 0))));  // 执行向左移动动作
}

void BirdLayer::initColumn2() {
  columnFlag2 = true;  // 设置柱子2标志位为true
  auto visibleSize = Director::getInstance()->getVisibleSize();  // 获取可见区域尺寸
  auto backSize = backGround->getContentSize();  // 获取背景尺寸大小
  auto floorSize = floor->getContentSize();  // 获取地面尺寸大小
  int height1 = 400 / random();  // 计算上柱子高度
  int height2 = backSize.height - height1 - 196;  // 计算下柱子高度
  auto columnNode1 = Sprite::create("pic/column1.png");  // 创建Sprite对象
  auto columnNode2 = Sprite::create("pic/column2.png");  // 创建Sprite对象
  auto columnPosition = columnUnder1->getPosition();  // 获取柱子1的位置
  columnUnder2 = Scale9Sprite::create();  // 创建Scale9Sprite对象
  columnUnder2->updateWithSprite(columnNode1, Rect(0, 0, 96, 400), false, Rect(0, 30, 96, 400));  // 设置图像操作区域
  columnUnder2->setAnchorPoint(Vec2::ZERO);  // 设置锚点
  columnUnder2->setContentSize(Size(96, height1));  // 设置图像伸缩
  columnUnder2->setPosition(Vec2(columnPosition.x + 320, floorSize.height));  // 设置位置
  columnOn2 = Scale9Sprite::create();  // 创建Scale9Sprite对象
  columnOn2->updateWithSprite(columnNode2, Rect(0, 0, 96, 400), false, Rect(0, 0, 96, 370));  // 设置图像操作区域
  columnOn2->setAnchorPoint(Vec2::ZERO);  // 设置锚点
  columnOn2->setContentSize(Size(96, height2));  // 设置图像伸缩
  columnOn2->setPosition(Vec2(columnPosition.x + 320, visibleSize.height - height2));  // 设置位置
  this->addChild(columnUnder2, 0);  // 添加到布景中
  this->addChild(columnOn2, 0);  // 添加到布景中
  columnUnder2->runAction(RepeatForever::create(MoveBy::create(0.1, Vec2(-25, 0))));  // 执行向左移动动作
  columnOn2->runAction(RepeatForever::create(MoveBy::create(0.1, Vec2(-25, 0))));  // 执行向左移动动作
}

void BirdLayer::update_column(float delta) {
  auto columnPosition1 = columnUnder1->getPosition();  // 获取柱子1的当前位置
  auto columnPosition2 = columnUnder2->getPosition();  // 获取柱子2的当前位置
  Size columnSize = columnUnder1->getContentSize();  // 获取柱子的尺寸大小
  if (columnPosition1.x <= -columnSize.width) {  // 判断柱子是否超出范围
    removeChild(columnUnder1);  // 将下柱子从布景中移除
    removeChild(columnOn1);  // 将上柱子从布景中移除
    initColumn1();  // 初始化柱子
  }
  if (columnPosition2.x <= -columnSize.width) {  // 判断柱子是否超出范围
    removeChild(columnUnder2);  // 将下柱子从布景中移除
    removeChild(columnOn2);  // 将上柱子从布景中移除
    initColumn2();  // 初始化柱子
  }
}

void BirdLayer::update_bird(float delta) {
  if (gameFlag && !runFlag)  // 判断小鸟是否正在运动以及游戏是否开始
    birdRun();  // 调用小鸟下落方法
  auto birdPosition = bird->getPosition();  // 获取小鸟当前位置
  Size birdSize = bird->getContentSize();  // 获取小鸟尺寸大小
  Size floorSize = floor->getContentSize();  // 获取地面尺寸大小
  auto columnPosition1 = columnUnder1->getPosition();  // 获取下柱子1位置
  auto columnPosition2 = columnOn1->getPosition();  // 获取上柱子1位置
  auto columnPosition3 = columnUnder2->getPosition();  // 获取下柱子2位置
  auto columnPosition4 = columnOn2->getPosition();  // 获取上柱子2位置
  Size columnSize1 = columnUnder1->getContentSize();  // 获取下柱子1尺寸大小
  Size columnSize2 = columnOn1->getContentSize();  // 获取上柱子1尺寸大小
  Size columnSize3 = columnUnder2->getContentSize();  // 获取下柱子2尺寸大小
  Size columnSize4 = columnOn2->getContentSize();  // 获取上柱子2尺寸大小
  if (birdPosition.x > columnPosition1.x && columnFlag1) {  // 判断小鸟是否通过柱子
    count++;  // 分数加1
    string num = StringUtils::toString(count);  // 将整型转化为字符型
    pLabelAtlas->setString(num);  // 改变文本标签内容
    if (MainLayer::soundFlag)  // 判断音效标志位
      pointSound();  // 播放得分音效
    columnFlag1 = false;  // 设置柱子标志位为false，表示小鸟已通过
  }
  if (birdPosition.x > columnPosition3.x && columnFlag2) {  // 判断小鸟是否通过柱子
    count++;  // 分数加1
    string num = StringUtils::toString(count);  // 将整型转化为字符型
    pLabelAtlas->setString(num);  // 改变文本标签内容
    if (MainLayer::soundFlag)  // 判断音效标志位
      pointSound();  // 播放得分音效
    columnFlag2 = false;  // 设置柱子标志位为false，表示小鸟已通过
  }
  //检测碰撞
  bool check = collision((birdPosition.x - birdSize.width), (birdPosition.y - birdSize.height), (birdPosition.x + birdSize.width), (birdPosition.y + birdSize.height),
      0, 0, floorSize.width, floorSize.height);
  bool check1 = collision((birdPosition.x - birdSize.width/3), (birdPosition.y - birdSize.height/3), (birdPosition.x + birdSize.width/3), (birdPosition.y + birdSize.height/3),
      columnPosition1.x, columnPosition1.y, (columnPosition1.x + columnSize1.width), (columnPosition1.y + columnSize1.height));
  bool check2 = collision((birdPosition.x - birdSize.width/3), (birdPosition.y - birdSize.height/3), (birdPosition.x + birdSize.width/3), (birdPosition.y + birdSize.height/3),
      columnPosition2.x, columnPosition2.y, (columnPosition2.x + columnSize2.width), (columnPosition2.y + columnSize2.height));
  bool check3 = collision((birdPosition.x - birdSize.width/3), (birdPosition.y - birdSize.height/3), (birdPosition.x + birdSize.width/3), (birdPosition.y + birdSize.height/3),
      columnPosition3.x, columnPosition3.y, (columnPosition3.x + columnSize3.width), (columnPosition3.y + columnSize3.height));
  bool check4 = collision((birdPosition.x - birdSize.width/3), (birdPosition.y - birdSize.height/3), (birdPosition.x + birdSize.width/3), (birdPosition.y + birdSize.height/3),
      columnPosition4.x, columnPosition4.y, (columnPosition4.x + columnSize4.width), (columnPosition4.y + columnSize4.height));
  if (check || check1 || check2 || check3 || check4) {  // 碰撞有效，游戏结束
    if(gameFlag) {
      if(MainLayer::soundFlag)  // 判断音效标志位
        hitSound();  // 播放碰撞音效
      if(check1 || check2 || check3 || check4) {  // 判断是否撞到柱子
        if(MainLayer::soundFlag)  // 判断音效标志位
          dieSound();  // 播放下落死亡音效
      }
    }
    gameOver();  // 调用游戏结束方法
  }
}

void BirdLayer::startGame() {  // 开始游戏
  Director::getInstance()->setDepthTest(false);  // 深度检测设置为false
  effectNode->runAction(SplitRows::create(0.5f, 30));
  initColumn1();  // 初始化第一根柱子
  initColumn2();  // 初始化第二根柱子
  birdRun();  // 调用小鸟运动方法
  auto sched = Director::getInstance()->getScheduler();  // 获取调度器
  sched->schedule(  // 将update_column方法设置为0.05秒回调一次，用于检测及初始化柱子
    schedule_selector(BirdLayer::update_column), this, 0.05, false);
  sched->schedule(  // 将update_bird方法设置为0.05秒回调一次，用于检测小鸟碰撞
    schedule_selector(BirdLayer::update_bird), this, 0.05, false);
}

void BirdLayer::gameOver() {  // 游戏结束
  gameFlag = false;  // 设置游戏标志位为false
  auto birdPosition = bird->getPosition();  // 获取小鸟当前位置
  Size birdSize = bird->getContentSize();  // 获取小鸟尺寸大小
  Size floorSize = floor->getContentSize();  // 获取地面尺寸大小

  // 停止动作
  floor->stopAllActions();
  columnUnder1->stopAllActions();
  columnOn1->stopAllActions();
  columnUnder2->stopAllActions();
  columnOn2->stopAllActions();
  bird->stopAllActions();

  bird->runAction(Sequence::create(Spawn::createWithTwoActions(  // 小鸟下落
      MoveTo::create(0.2, Vec2(birdPosition.x, floorSize.height+birdSize.width/2)), RotateTo::create(0.2, 90)), NULL));
  if (!overFlag)  // 显示重玩界面
    replay();
}

void BirdLayer::replay() {
  overFlag = true;  // 将游戏结束标志位设置为true
  string medal;  // 奖牌图片路径
  removeChild(pLabelAtlas);  // 删除分数
  Sprite* over = Sprite::create("pic/gameOver.png");  // 游戏结束精灵
  over->setPosition(Vec2(270, 1200));
  this->addChild(over, 10);

  overRank = Sprite::create("pic/scoreOver.png");  // 记分板精灵
  overRank->setPosition(Vec2(270, -100));
  this->addChild(overRank, 10);

  replayItem = MenuItemImage::create(  // 重新开始
    "button/replay.png", "button/replay_off.png", CC_CALLBACK_1(BirdLayer::menuCallbackItem0, this));
  replayItem->setPosition(Vec2(150, -250));

  menuItem = MenuItemImage::create(  // 返回主菜单
    "button/menu.png", "button/menu_off.png", CC_CALLBACK_1(BirdLayer::menuCallbackItem1, this));
  menuItem->setPosition(Vec2(380, -250));

  Menu* menu = Menu::create(replayItem, menuItem, NULL);
  menu->setPosition(Vec2::ZERO);  // 设置菜单位置
  this->addChild(menu,10);

  RankBirdLayer* rank = RankBirdLayer::create();  // 排行榜
  rank->load();  // 加载分数
  rank->save(count);  // 存储当前得分
  int topScore = atoi(rank->scoreBird[0].c_str());  // 将最高分数转为整型
  string tempScore = StringUtils::format("%d", count);  // 将当前分数转为字符串
  Label* newScore = Label::createWithTTF(  // 最新分数
    tempScore, "fonts/FZKATJW.ttf", 50, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
  newScore->setColor(Color3B::YELLOW);
  newScore->enableOutline(Color4B(187, 187, 187, 255),2);
  overRank->addChild(newScore);
  newScore->setPosition(Vec2(320, 130));

  Label* ss = Label::createWithTTF((topScore < count ? tempScore : rank->scoreBird[0]),  // 最高分数
    "fonts/FZKATJW.ttf", 50, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
  ss->setColor(Color3B::YELLOW);
  ss->enableOutline(Color4B(187, 187, 187, 255),2);
  overRank->addChild(ss);
  ss->setPosition(Vec2(320, 50));

  if (count >= atoi(rank->scoreBird[2].c_str())) {
    medal = "pic/third.png";  // 铜牌
    if (count >= atoi(rank->scoreBird[1].c_str())) {
      medal = "pic/slider.png";  // 银牌
      if (count >= atoi(rank->scoreBird[0].c_str())) {
        medal = "pic/gold.png";  // 金牌
      }
    }
    Sprite* gold = Sprite::create(medal);  // 奖牌精灵
    overRank->addChild(gold, 5);
    gold->setPosition(Vec2(90, 88));
  }
  over->runAction(Sequence::create(  // 执行动作
        MoveTo::create(0.2, Vec2(270, 750)),
        CallFunc::create(CC_CALLBACK_0(BirdLayer::moveObject, this)),  NULL));
}

void BirdLayer::moveObject() {  // 游戏结束运动方法
  overRank->runAction(MoveTo::create(0.6, Vec2(270, 550)));  // 将记分板移动到指定位置
  replayItem->runAction(MoveTo::create(0.6, Vec2(150, 380)));  // 将重玩菜单项移动到指定位置
  menuItem->runAction(MoveTo::create(0.6, Vec2(380, 380)));  // 将主菜单项移动到指定位置
}

bool BirdLayer::onTouchBegan(Touch* touch, Event* event) {
  auto birdPosition = bird->getPosition();  // 获取小鸟当前坐标
  if (gameFlag) {  // 判断游戏是否开始
    if (readyFlag) {  // 判断是否处于准备状态
      startGame();  // 开始游戏
      readyFlag = false;  // 将准备标志位设为false
    }
    if (MainLayer::soundFlag)  // 判断音效是否开启
      wingSound();  // 小鸟上升音效
    auto action = Spawn::createWithTwoActions(  // 小鸟动作
      MoveTo::create(0.2, Vec2(birdPosition.x, birdPosition.y + 105)), RotateTo::create(0, -30));
    bird->stopAllActions();  // 停止小鸟动作
    bird->runAction(RepeatForever::create(animAc));  // 重新执行小鸟动画
    bird->runAction(Sequence::create(  // 执行小鸟上升动作
            CallFunc::create(CC_CALLBACK_0(BirdLayer::setRunFlag1, this)),
            action, DelayTime::create(0.05 ),  // 间隔时间
            CallFunc::create(CC_CALLBACK_0(BirdLayer::setRunFlag2, this)),
            RotateTo::create(2.0, 90), NULL));  // 转向角度
  }
  return true;
}

void BirdLayer::setRunFlag1() {
  runFlag = true;  // 将运动标志位设为true
}

void BirdLayer::setRunFlag2() {
  runFlag = false;  // 将运动标志位设为false
}

bool BirdLayer::onTouchPause(Touch* touch, Event* event) {
  if (!gameFlag)  // 判断游戏是否结束
    return false;
  auto target = static_cast<Sprite*>(event->getCurrentTarget());  // 获取的当前触摸的对象，并转化为精灵类型
  auto location = target->convertToNodeSpace(touch->getLocation());  // 获取当前坐标
  auto size = target->getContentSize();  // 获取精灵的大小
  auto rect = Rect(0, 0, size.width, size.height);  // 创建一个矩形对象，其大小与精灵相同
  if(!stopFlag && rect.containsPoint(location))  {  // 判断触摸点是否在目标的范围内
    stopFlag = true;  // 将暂停标志位设为true
    pause->setTexture("button/continue_pause.png");  // 替换暂停精灵图片
    Director::getInstance()->pause();  // 暂停场景

    pauseBackground = Sprite::create("pic/setBackground.png");  // 背景框
    pauseBackground->setPosition(Vec2(270, 600));
    this->addChild(pauseBackground, 10);

    Sprite* music = Sprite::create("pic/music.png");  // 音乐
    pauseBackground->addChild(music, 1);
    music->setPosition(Vec2(120, 300));

    CheckBox* checkMusic = CheckBox::create(
        "button/sound_on.png", "button/sound_off.png",
        "button/sound_off.png", "button/sound_stop.png",
        "button/sound_stop.png");
    pauseBackground->addChild(checkMusic, 1);
    checkMusic->setPosition(Vec2(350, 300));
    checkMusic->setSelected(!MainLayer::musicFlag);
    checkMusic->addEventListener(CC_CALLBACK_2(BirdLayer::selectedEvent0, this));

    Sprite* sound = Sprite::create("pic/sound.png");  // 音效
    pauseBackground->addChild(sound, 1);
    sound->setPosition(Vec2(120, 200));

    CheckBox* checkSound = CheckBox::create(
        "button/sound_on.png", "button/sound_off.png",
        "button/sound_off.png", "button/sound_stop.png",
        "button/sound_stop.png");
    pauseBackground->addChild(checkSound, 1);
    checkSound->setPosition(Vec2(350, 200));
    checkSound->setSelected(!MainLayer::soundFlag);
    checkSound->addEventListener(CC_CALLBACK_2(BirdLayer::selectedEvent1, this));

    MenuItemImage* conItem = MenuItemImage::create(  // 继续游戏
        "button/continue.png", "button/continue_off.png",
        CC_CALLBACK_1(BirdLayer::menuCallbackItem3, this));
    conItem->setPosition(Vec2(80, 80));

    MenuItemImage* backItem = MenuItemImage::create(  // 返回主菜单
      "button/menu.png", "button/menu_off.png",
       CC_CALLBACK_1(BirdLayer::menuCallbackItem2, this));
    backItem->setPosition(Vec2(225, 80));

    MenuItemImage* againItem = MenuItemImage::create(
      "button/replay.png", "button/replay_off.png",  // 重新开始
       CC_CALLBACK_1(BirdLayer::menuCallbackItem4, this));
    againItem->setPosition(Vec2(370, 80));

    Menu* menu = Menu::create(againItem, conItem, backItem, NULL);
    menu->setPosition(Vec2::ZERO);  // 设置菜单位置
    pauseBackground->addChild(menu,1);

    return true;
  }

  return false;
}

void BirdLayer::menuCallbackItem0(Ref* pSender) {  // 重新开始游戏
  sceneManager->goToFlappyScene(1);  // 重新切换到游戏场景
}

void BirdLayer::menuCallbackItem1(Ref* pSender) {  // 切换到主菜单场景
  sceneManager->goToMainScene();  // 切换到主菜单场景
}

void BirdLayer::menuCallbackItem2(Ref* pSender) {  // 暂停时切换到主菜单场景
  Director::getInstance()->resume();  // 恢复游戏
  sceneManager->goToMainScene();  // 切换到主菜单场景
}
//继续游戏按钮执行方法
void BirdLayer::menuCallbackItem3(Ref* pSender) {  // 继续游戏
  stopFlag = false;  // 将暂停标志位设置为false
  this->removeChild(pauseBackground);  // 将暂停背景板从布景中移除
  pause->setTexture("button/pause.png");  // 替换暂停精灵图片
  Director::getInstance()->resume();  // 恢复游戏
}
//重新开始游戏按钮执行方法
void BirdLayer::menuCallbackItem4(Ref* pSender) {  // 暂停时重新开始游戏
  Director::getInstance()->resume();  // 恢复游戏
  sceneManager->goToFlappyScene(1);  // 重新切换到游戏场景
}

void BirdLayer::selectedEvent0(Ref* pSender, CheckBox::EventType type) {  // 音乐
  switch (type) {
    case CheckBox::EventType::SELECTED:  // 复选框被选中
      CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();  // 暂停播放背景音乐
      MainLayer::musicFlag = false;  // 将音乐标志位设置为false
      break;
    case CheckBox::EventType::UNSELECTED:  // 复选框没选中
      CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();  // 继续播放背景音乐
      MainLayer::musicFlag = true;  // 将音乐标志位设置为true
      break;
    default:
      break;
  }
}

void BirdLayer::selectedEvent1(Ref* pSender, CheckBox::EventType type) {  // 音效
  switch (type) {
    case CheckBox::EventType::SELECTED:  // 复选框被选中
      MainLayer::soundFlag = false;  // 将音效标志位设置为false
      break;
    case CheckBox::EventType::UNSELECTED:  // 复选框没被选中
      MainLayer::soundFlag = true;  // 将音效标志位设置为true
      break;
    default:
      break;
  }
}

void BirdLayer::wingSound() {  // 播放小鸟跳跃音效
  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_wing.ogg");
}

void BirdLayer::pointSound() {  // 播放得分音效
  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_point.ogg");
}

void BirdLayer::hitSound() {  // 播放小鸟撞柱子音效
  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_hit.ogg");
}

void BirdLayer::dieSound() {  // 播放小鸟下落音效
  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_die.ogg");
}
