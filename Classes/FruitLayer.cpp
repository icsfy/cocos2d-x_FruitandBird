#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Collision.h"
#include "FruitLayer.h"
#include "RankFruitLayer.h"
#include "MainLayer.h"
#include <time.h>

using namespace cocos2d;
using std::string;

bool FruitLayer::init() {
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
  this->addChild(floor, 4);  // 将地面精灵添加到布景中
  Size floorSize = floor->getContentSize();  // 获取地面精灵尺寸大小
  floor->runAction(RepeatForever::create(  // 设置地面精灵循环滚动
    Sequence::create(MoveTo::create(0.5, Vec2(-120, 0)), MoveTo::create(0, Vec2::ZERO), NULL)));

  effectNode = NodeGrid::create();  // 创建一个网络节点对象
  this->addChild(effectNode, 1);  // 将网格节点添加到布景
  ready = Sprite::create("pic/readyFruit.png");  // 游戏预备界面
  ready->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));  // 设置精灵位置
  effectNode->addChild(ready, 0);  // 将精灵添加到网络节点中

  pause = Sprite::create("button/pause.png");  // 创建暂停按钮精灵
  pause->setPosition(Vec2(32, 928));  // 设置精灵位置
  this->addChild(pause, 10);  // 将精灵添加到布景中

  auto listenerPause = EventListenerTouchOneByOne::create();  // 创建暂停精灵触摸监听
  listenerPause->setSwallowTouches(true);  // 设置下传触摸
  listenerPause->onTouchBegan = CC_CALLBACK_2(FruitLayer::onTouchPause, this);  // 开始触摸时回调onTouchBegan方法
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerPause, pause);  // 添加到监听器

  auto listener = EventListenerTouchOneByOne::create();  // 创建一个单点触摸监听
  listener->setSwallowTouches(true);  // 设置下传触摸
  listener->onTouchBegan = CC_CALLBACK_2(FruitLayer::onTouchBegan, this);  // 开始触摸时回调onTouchBegan方法
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);  // 添加到监听器

  return true;
}

int FruitLayer::random() {  // 生成随机数
  srand((unsigned)time(NULL));  // 用系统时间作为随机种子
  return rand() % 5;  // 将随机数返回
}

void FruitLayer::initColumn1() {
  auto visibleSize = Director::getInstance()->getVisibleSize();  // 获取可见区域尺寸
  int height = -(random() * 5);
  height1 = 800 - height;  // 计算柱子高度

  columnUnder1 = Sprite::create("pic/column1.png");  // 创建Sprite对象
  columnUnder1->setAnchorPoint(Vec2::ZERO);  // 设置锚点
  columnUnder1->setPosition(Vec2(visibleSize.width, height));  // 设置位置
  columnOn1 = Sprite::create("pic/column2.png");  // 创建Sprite对象
  columnOn1->setAnchorPoint(Vec2::ZERO);  // 设置锚点
  columnOn1->setPosition(Vec2(visibleSize.width, height1));  // 设置位置
  this->addChild(columnUnder1, 3);  // 添加到布景中
  this->addChild(columnOn1, 3);  // 添加到布景中
  int speed = count * 0.05 + 15;  // 根据游戏当前分数设置移动速度
  columnUnder1->runAction(RepeatForever::create(MoveBy::create(0.1, Vec2(-speed, 0))));  // 执行动作
  columnOn1->runAction(RepeatForever::create(MoveBy::create(0.1, Vec2(-speed, 0))));  // 执行动作
}

void FruitLayer::initColumn2() {
  int height = -(random() * 6);
  height2 = 800 - height;  // 计算柱子高度
  auto columnPosition = columnUnder1->getPosition();  // 获取柱子1位置
  columnUnder2 = Sprite::create("pic/column1.png");  // 创建Sprite对象
  columnUnder2->setAnchorPoint(Vec2::ZERO);  // 设置锚点
  columnUnder2->setPosition(Vec2(columnPosition.x + 320, height));  // 设置位置
  columnOn2 = Sprite::create("pic/column2.png");  // 创建Sprite对象
  columnOn2->setAnchorPoint(Vec2::ZERO);  // 设置锚点
  columnOn2->setPosition(Vec2(columnPosition.x + 320, height2));  // 设置位置
  this->addChild(columnUnder2, 3);  // 添加到布景中
  this->addChild(columnOn2, 3);  // 添加到布景中
  int speed = count * 0.05 + 15;  // 根据游戏当前分数设置移动速度
  columnUnder2->runAction(RepeatForever::create(MoveBy::create(0.1, Vec2(-speed, 0))));  // 执行动作
  columnOn2->runAction(RepeatForever::create(MoveBy::create(0.1, Vec2(-speed, 0))));  // 执行动作
}

void FruitLayer::update_fruit(float delta) {  // 定时产生水果
  if (gameFlag) {  // 判断游戏是否开始
    int i = random();  // 获取随机数
    float time = 54 / (i * 2 + 10);  // 计算精灵运动到右边的时间
    string number = StringUtils::format("%d", i);  // 将整型转为字符串
    string part = "pic/fruit" + number + ".png";  // 生成图片路径
    Sprite* ff = Sprite::create(part);  // 创建一个水果精灵
    ff->setPosition(Vec2(0, (700 - i * 40)));  // 设置位置
    this->addChild(ff, 2);  // 添加到布景中
    ff->runAction(MoveTo::create(time, Vec2(540, (700 - i * 40))));  // 执行动作
    int down = (700 -i * 40) - 450;  // 计算精灵上下移动幅度
    if ((700 - i * 40) < 600)
      down = -down;  // 设置某些精灵开始时的方向
    ff->runAction(RepeatForever::create(Sequence::create(  // 执行动作
      MoveBy::create(time/3, Vec2(0, -down)),
      MoveBy::create(time/3, Vec2(0, down)), NULL)));
    fruit.push_back(ff);  // 将精灵添加到向量中
  }
}

void FruitLayer::update_boss(float delta) {  // 定时产生水果boss
  if (gameFlag) {  // 判断游戏是否开始
    boss++;  // 出现的boss数目加1
    bossFlag = true;  // 将boss标志位设置为true
    string number = StringUtils::format("%d", random());  // 将整型转为字符串
    string partBoss = "pic/fruit" + number + ".png";  // 生成图片路径
    bossSprite = Sprite::create(partBoss);  // 创建一个水果boss精灵
    bossSprite->setScale(2);  // 设置精灵缩放比
    bossSprite->setPosition(Vec2(0, 600));  // 设置位置
    this->addChild(bossSprite, 2);  // 添加到布景中
    bossSprite->runAction(MoveTo::create(10, Vec2(540, 600)));  // 执行移动到右边框的动作
    slider = Slider::create();  // 创建一个拖拉条
    slider->loadBarTexture("pic/sliderTrack.png");  // 未被拖拉时加载的纹理
    slider->loadProgressBarTexture("pic/blood.png");  // 被拖拉后加载的纹理
    slider->setPercent(100);  // 设置拖拉条百分比
    slider->setPosition(Vec2(0, 700));  // 设置拖拉条位置
    this->addChild(slider, 2);  // 添加到布景中
    slider->runAction(MoveTo::create(10, Vec2(540, 700)));  // 执行移动到右边框的动作
  }
}

void FruitLayer::update_column(float delta) {
  auto visibleSize = Director::getInstance()->getVisibleSize();  // 获取可见区域尺寸
  auto columnPosition1 = columnUnder1->getPosition();  // 获取柱子1的当前位置
  auto columnPosition2 = columnUnder2->getPosition();  // 获取柱子2的当前位置
  auto columnSize = columnUnder1->getContentSize();  // 获取柱子的尺寸大小
  if (columnPosition1.x <= -columnSize.width) {  // 判断柱子是否超出范围
    actionFlag = false;  // 将动作标志位设置为false
    removeChild(columnUnder1);  // 将下柱子从布景中移除
    removeChild(columnOn1);  // 将上柱子从布景中移除
    initColumn1();  // 初始化柱子
  }
  if (columnPosition2.x <= -columnSize.width) {  // 判断柱子是否超出范围
    actionFlag = false;  // 将动作标志位设置为false
    removeChild(columnUnder2);  // 将下柱子从布景中移除
    removeChild(columnOn2);  // 将上柱子从布景中移除
    initColumn2();  // 初始化柱子
  }
  for (auto iter = fruit.begin(); iter != fruit.end(); iter++) {
    auto fruitPosition = (*iter)->getPosition();  // 获取当前水果位置
    Size fruitSize = (*iter)->getContentSize();  // 获取当前水果的尺寸大小
    if (fruitPosition.x >= (visibleSize.width - fruitSize.width/2))  // 判断水果是否到达右边框
      gameOver();  // 游戏结束
  }
  if (bossFlag) {
    auto bossPosition = bossSprite->getPosition();  // 获取当前水果boss位置
    Size bossSize = bossSprite->getContentSize();  // 获取当前水果的尺寸大小
    if (bossPosition.x >= (visibleSize.width - bossSize.width/2))  // 判断水果boss是否到达右边框
      gameOver();  // 游戏结束
  }
}

void FruitLayer::startGame() {  // 开始游戏
  Director::getInstance()->setDepthTest(false);  // 深度检测设置为false
  effectNode->runAction(SplitRows::create(1.0f, 20));  // 执行特效
  Size visibleSize = Director::getInstance()->getVisibleSize();  // 获取可见区域尺寸
  initColumn1();  // 初始化第一根柱子
  initColumn2();  // 初始化第二根柱子
  gameFlag = true;  // 将游戏开始标志位设置为true

  pLabelAtlas = LabelAtlas::create("0", "pic/number.png", 48, 64, '0');  // 创建一个文本标签对象
  pLabelAtlas->setPosition(Vec2(visibleSize.width/5, visibleSize.height/5*4));  // 设置文本标签位置
  this->addChild(pLabelAtlas, 4);  // 将文本标签添加到布景中

  auto sched = Director::getInstance()->getScheduler();  // 获取调度器
  sched->schedule(  // 将update_fruit方法设置为1秒回调一次，用于产生水果
    schedule_selector(FruitLayer::update_fruit), this, 1, false);
  sched->schedule(  // 将update_boss方法设置为20秒回调一次，用于产生boss
    schedule_selector(FruitLayer::update_boss), this, 20, false);
  sched->schedule(  // 将update_column方法设置为0.1秒回调一次，用于更新柱子
    schedule_selector(FruitLayer::update_column), this, 0.1, false);

}

void FruitLayer::check() {
  auto columnPosition1 = columnUnder1->getPosition();  // 获取柱子位置
  auto columnPosition2 = columnOn1->getPosition();  // 获取柱子位置
  auto columnPosition3 = columnUnder2->getPosition();  // 获取柱子位置
  auto columnPosition4 = columnOn2->getPosition();  // 获取柱子位置
  Size columnSize = columnUnder1->getContentSize();  // 获取柱子尺寸大小

  for(auto iter = fruit.begin(); iter != fruit.end();) {  // 获取向量中的水果精灵
    auto fruitPosition = (*iter)->getPosition();  // 获取当前水果位置
    Size fruitSize = (*iter)->getContentSize();  // 获取当前水果的尺寸大小
    bool check1 = collision((fruitPosition.x - fruitSize.width/2), (fruitPosition.y - fruitSize.height/2), (fruitPosition.x + fruitSize.width/2), (fruitPosition.y + fruitSize.height/2),
          columnPosition1.x, columnPosition1.y, (columnPosition1.x + columnSize.width), (columnPosition1.y + columnSize.height));
    bool check2 = collision((fruitPosition.x - fruitSize.width/2), (fruitPosition.y - fruitSize.height/2), (fruitPosition.x + fruitSize.width/2), (fruitPosition.y + fruitSize.height/2),
          columnPosition2.x, columnPosition2.y, (columnPosition2.x + columnSize.width), (columnPosition2.y + columnSize.height));
    bool check3 = collision((fruitPosition.x - fruitSize.width/2), (fruitPosition.y - fruitSize.height/2), (fruitPosition.x + fruitSize.width/2), (fruitPosition.y + fruitSize.height/2),
          columnPosition3.x, columnPosition3.y, (columnPosition3.x + columnSize.width), (columnPosition3.y + columnSize.height));
    bool check4 = collision((fruitPosition.x - fruitSize.width/2), (fruitPosition.y - fruitSize.height/2), (fruitPosition.x + fruitSize.width/2), (fruitPosition.y + fruitSize.height/2),
          columnPosition4.x, columnPosition4.y, (columnPosition4.x + columnSize.width), (columnPosition4.y + columnSize.height));
    if (check1 || check2 || check3 || check4) {  // 判断是否碰撞
      count++;  // 分数加1
      this->removeChild(*iter);  // 将精灵从布景中移除
      iter = fruit.erase(iter);  // 将精灵从向量中移除
      string num =StringUtils::toString(count);  // 将整型转为字符串
      pLabelAtlas->setString(num);  // 设置文本标签中的内容

      ParticleSystemQuad* psq = ParticleExplosion::create();  // 创建爆炸粒子系统效果
      psq->retain();  // 保持引用
      psq->setLife(0.5f);  // 设置生命周期
      this->addChild(psq, 1);  // 将粒子系统添加到精灵中
      psq->setTexture(  // 为粒子系统设置图片
        Director::getInstance()->getTextureCache()->addImage("pic/fire.png") );
      psq->setPosition(Vec2(fruitPosition.x, fruitPosition.y));  // 设置粒子系统的位置
      if (MainLayer::soundFlag)  // 判断游戏音效是否开启
        eatSound();  // 播放夹水果音效
    } else {
      if (MainLayer::soundFlag)  // 判断游戏音效是否开启
        wingSound();  // 播放夹空音效
      iter++;
    }
  }
  if (bossFlag) {
    auto bossPosition = bossSprite->getPosition();  // 获取当前水果位置
    Size bossSize = bossSprite->getContentSize();  // 获取当前水果的尺寸大小
    bool check1 = collision((bossPosition.x - bossSize.width/2), (bossPosition.y - bossSize.height/2), (bossPosition.x + bossSize.width/2), (bossPosition.y + bossSize.height/2),
          columnPosition1.x, columnPosition1.y, (columnPosition1.x + columnSize.width), (columnPosition1.y + columnSize.height));
    bool check2 = collision((bossPosition.x - bossSize.width/2), (bossPosition.y - bossSize.height/2), (bossPosition.x + bossSize.width/2), (bossPosition.y + bossSize.height/2),
          columnPosition2.x, columnPosition2.y, (columnPosition2.x + columnSize.width), (columnPosition2.y + columnSize.height));
    bool check3 = collision((bossPosition.x - bossSize.width/2), (bossPosition.y - bossSize.height/2), (bossPosition.x + bossSize.width/2), (bossPosition.y + bossSize.height/2),
          columnPosition3.x, columnPosition3.y, (columnPosition3.x + columnSize.width), (columnPosition3.y + columnSize.height));
    bool check4 = collision((bossPosition.x - bossSize.width/2), (bossPosition.y - bossSize.height/2), (bossPosition.x + bossSize.width/2), (bossPosition.y + bossSize.height/2),
          columnPosition4.x, columnPosition4.y, (columnPosition4.x + columnSize.width), (columnPosition4.y + columnSize.height));
    if (check1 || check2 || check3 || check4) {  // 判断是否碰撞
      int percent = slider->getPercent();  // 获取拖拉条当前百分比
      percent = percent - 10/boss;  // 计算每夹一次水果减小的百分比
      slider->setPercent(percent);  // 重新设置拖拉条的进度
      if (percent <= 0) {  // 判断拖拉条百分比是否小于0
        count = count + boss*50;  // 夹破boss更新分数
        this->removeChild(bossSprite);  // 将boss从布景中移除
        this->removeChild(slider);  // 将拖拉条从布景中移除
        string num =StringUtils::toString(count);  // 将整型转为字符串
        pLabelAtlas->setString(num);  // 设置文本标签内容
        bossFlag = false;  // 将bossFlag标志位设置为false
        if(MainLayer::soundFlag)  // 判断游戏音效是否开启
          eatSound();  // 播放夹水果音效
      }  // if (percent <= 0)
    }  // if (check1 || check2 || check3 || check4)
  }  // if (bossFlag)
}

void FruitLayer::setFlag() {  // 设置柱子运动标志位
  actionFlag = false;  // 将柱子运动标志位设置为false
}

void FruitLayer::gameOver() {  // 游戏结束
  gameFlag = false;  // 设置游戏标志位为false

  columnUnder1->stopAllActions();  // 停止柱子动作
  columnOn1->stopAllActions();  // 停止柱子动作
  columnUnder2->stopAllActions();  // 停止柱子动作
  columnOn2->stopAllActions();  // 停止柱子动作
  floor->stopAllActions();  // 停止地面动作

  if (bossFlag) {  // 判断场景中是否存在boss精灵
    bossSprite->stopAllActions();  // 停止boss动作
    slider->stopAllActions();  // 停止拖拉条动作
  }

  for (auto iter = fruit.begin(); iter != fruit.end(); iter++)  // 遍历水果精灵
    (*iter)->stopAllActions();  // 停止水果精灵动作

  if (!overFlag)  // 显示重玩界面
    replay();
}

void FruitLayer::replay() {
  overFlag = true;  // 将游戏结束标志位设置为true
  string medal;  // 奖牌图片路径
  removeChild(pLabelAtlas);  // 删除文本标签
  Sprite* over = Sprite::create("pic/gameOver.png");  // 游戏结束精灵
  over->setPosition(Vec2(270, 1200));
  this->addChild(over, 10);

  overRank = Sprite::create("pic/scoreOver.png");  // 记分板精灵
  overRank->setPosition(Vec2(270, -100));
  this->addChild(overRank, 10);

  replayItem = MenuItemImage::create(  // 重新开始
    "button/replay.png", "button/replay_off.png", CC_CALLBACK_1(FruitLayer::menuCallbackRestart, this));
  replayItem->setPosition(Vec2(150, -250));

  menuItem = MenuItemImage::create(  // 返回主菜单
    "button/menu.png", "button/menu_off.png", CC_CALLBACK_1(FruitLayer::menuCallbackToMain, this));
  menuItem->setPosition(Vec2(380, -250));

  Menu* menu = Menu::create(replayItem, menuItem, NULL);
  menu->setPosition(Vec2::ZERO);  // 设置菜单位置
  this->addChild(menu,10);

  RankFruitLayer* rank = RankFruitLayer::create();  // 排行榜
  rank->load();  // 加载分数
  rank->save(count);  // 存储当前得分
  int topScore = atoi(rank->scoreFruit[0].c_str());  // 将最高分数转为整型
  string tempScore = StringUtils::format("%d", count);  // 将当前分数转为字符串
  Label* newScore = Label::createWithTTF(  // 最新分数
    tempScore, "fonts/FZKATJW.ttf", 50, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
  newScore->setColor(Color3B::YELLOW);
  newScore->enableOutline(Color4B(187, 187, 187, 255),2);
  overRank->addChild(newScore);
  newScore->setPosition(Vec2(320, 130));

  Label* ss = Label::createWithTTF((topScore < count ? tempScore : rank->scoreFruit[0]),  // 最高分数
    "fonts/FZKATJW.ttf", 50, Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
  ss->setColor(Color3B::YELLOW);
  ss->enableOutline(Color4B(187, 187, 187, 255),2);
  overRank->addChild(ss);
  ss->setPosition(Vec2(320, 50));

  if (count >= atoi(rank->scoreFruit[2].c_str())) {
    medal = "pic/third.png";  // 铜牌
    if (count >= atoi(rank->scoreFruit[1].c_str())) {
      medal = "pic/slider.png";  // 银牌
      if (count >= atoi(rank->scoreFruit[0].c_str())) {
        medal = "pic/gold.png";  // 金牌
      }
    }
    Sprite* gold = Sprite::create(medal);  // 奖牌精灵
    overRank->addChild(gold, 5);
    gold->setPosition(Vec2(90, 88));
  }
  over->runAction(Sequence::create(  // 执行动作
        MoveTo::create(0.2, Vec2(270, 750)),
        CallFunc::create(CC_CALLBACK_0(FruitLayer::moveObject, this)),  NULL));
}

void FruitLayer::moveObject() {  // 游戏结束运动方法
  overRank->runAction(MoveTo::create(0.6, Vec2(270, 550)));  // 将记分板移动到指定位置
  replayItem->runAction(MoveTo::create(0.6, Vec2(150, 380)));  // 将重玩菜单项移动到指定位置
  menuItem->runAction(MoveTo::create(0.6, Vec2(380, 380)));  // 将主菜单项移动到指定位置
}

bool FruitLayer::onTouchBegan(Touch* touch, Event* event) {
  if (actionFlag)  // 判断柱子是否在运动
    return false;
  if (readyFlag) {  // 判断是否处于准备状态
    startGame();  // 开始游戏
    readyFlag = false;  // 将准备标志位设为false
  }
  auto columnPosition1 = columnUnder1->getPosition();  // 获取柱子1的当前位置
  auto columnPosition2 = columnUnder2->getPosition();  // 获取柱子2的当前位置
  auto touchPosition = touch->getLocation();  // 获取当前触控位置
  Size visibleSize = Director::getInstance()->getVisibleSize();  // 获取可见区域尺寸
  int compression1 = height1 - 600;  // 获取柱子1的位移
  int compression2 = height2 - 600;  // 获取柱子2的位移
  Action* action1 = Sequence::create(  // 设置动作
    MoveBy::create(0.1, Vec2(0, compression1)),  // 向上移动
    CallFunc::create(CC_CALLBACK_0(FruitLayer::check, this)),  // 碰撞检测
    MoveBy::create(0.1, Vec2(0, -compression1)),  // 向下移动
    CallFunc::create(CC_CALLBACK_0(FruitLayer::setFlag, this)), NULL);  // 改变标志位
  Action* action2 = Sequence::create(  // 设置动作
    MoveBy::create(0.1, Vec2(0, -compression1)),  // 向下移动
    CallFunc::create(CC_CALLBACK_0(FruitLayer::check, this)),  // 碰撞检测
    MoveBy::create(0.1, Vec2(0, compression1)), NULL);  // 向上移动
  Action* action3 = Sequence::create(  // 设置动作
    MoveBy::create(0.1, Vec2(0, compression2)),  // 向上移动
    CallFunc::create(CC_CALLBACK_0(FruitLayer::check, this)),  // 碰撞检测
    MoveBy::create(0.1, Vec2(0, -compression2)),  // 向下移动
    CallFunc::create(CC_CALLBACK_0(FruitLayer::setFlag, this)), NULL);  // 改变标志位
  Action* action4 = Sequence::create(  // 设置动作
    MoveBy::create(0.1, Vec2(0, -compression2)),  // 向下移动
    CallFunc::create(CC_CALLBACK_0(FruitLayer::check, this)),  // 碰撞检测
    MoveBy::create(0.1, Vec2(0, compression2)), NULL);  // 向上移动

  if (gameFlag) {  // 判断游戏是否开始
    Size columnSize = columnUnder1->getContentSize();  // 获取下柱子尺寸大小
    bool columnCheck1 = collision(  // 碰撞检测，用于判断柱子是否在屏幕在侧
      columnPosition1.x, columnPosition1.y,
      columnPosition1.x + columnSize.width, columnPosition1.y + columnSize.height,
      0, 0, visibleSize.width/2, visibleSize.height);
    bool columnCheck2 = collision(  // 碰撞检测，用于判断柱子是否在屏幕在侧
      columnPosition2.x, columnPosition2.y,
      columnPosition2.x + columnSize.width, columnPosition2.y + columnSize.height,
      0, 0, visibleSize.width/2, visibleSize.height);
    if (touchPosition.x < visibleSize.width/2) {  // 判断当前触摸点是否在屏幕左侧
      if (columnCheck1) {  // 判断柱子1是否在左侧
        actionFlag = true;  // 将运动标志位设置为true
        columnUnder1->runAction(action1);  // 执行动作
        columnOn1->runAction(action2);  // 执行动作
      } else if (columnCheck2) {  // 判断柱子2是否在左侧
        actionFlag = true;  // 将运动标志位设置为true
        columnUnder2->runAction(action3);  // 执行动作
        columnOn2->runAction(action4);  // 执行动作
      }
    } else if (touchPosition.x >= visibleSize.width/2) {  // 判断当前触摸点是否在屏幕右侧
      if (!columnCheck1) {  // 判断柱子1是否在右侧
        actionFlag = true;  // 将运动标志位设置为true
        columnUnder1->runAction(action1);  // 执行动作
        columnOn1->runAction(action2);  // 执行动作
      } else if (!columnCheck2) {  // 判断柱子2是否在左侧
        actionFlag = true;  // 将运动标志位设置为true
        columnUnder2->runAction(action3);  // 执行动作
        columnOn2->runAction(action4);  // 执行动作
      }
    }
  }
  return true;
}

bool FruitLayer::onTouchPause(Touch* touch, Event* event) {
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
    checkMusic->addEventListener(CC_CALLBACK_2(FruitLayer::selectedEvent0, this));

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
    checkSound->addEventListener(CC_CALLBACK_2(FruitLayer::selectedEvent1, this));

    MenuItemImage* conItem = MenuItemImage::create(  // 继续游戏
        "button/continue.png", "button/continue_off.png",
        CC_CALLBACK_1(FruitLayer::menuCallbackResume, this));
    conItem->setPosition(Vec2(80, 80));

    MenuItemImage* backItem = MenuItemImage::create(  // 返回主菜单
      "button/menu.png", "button/menu_off.png",
       CC_CALLBACK_1(FruitLayer::menuCallbackPauseToMain, this));
    backItem->setPosition(Vec2(225, 80));

    MenuItemImage* againItem = MenuItemImage::create(
      "button/replay.png", "button/replay_off.png",  // 重新开始
       CC_CALLBACK_1(FruitLayer::menuCallbackPauseRestart, this));
    againItem->setPosition(Vec2(370, 80));

    Menu* menu = Menu::create(againItem, conItem, backItem, NULL);
    menu->setPosition(Vec2::ZERO);  // 设置菜单位置
    pauseBackground->addChild(menu,1);

    return true;
  }

  return false;
}

void FruitLayer::menuCallbackRestart(Ref* pSender) {  // 重新开始游戏
  sceneManager->goToSquishyScene(1);  // 重新切换到游戏场景
}

void FruitLayer::menuCallbackToMain(Ref* pSender) {  // 切换到主菜单场景
  sceneManager->goToMainScene();  // 切换到主菜单场景
}

void FruitLayer::menuCallbackPauseToMain(Ref* pSender) {  // 暂停时切换到主菜单场景
  Director::getInstance()->resume();  // 恢复游戏
  sceneManager->goToMainScene();  // 切换到主菜单场景
}

void FruitLayer::menuCallbackResume(Ref* pSender) {  // 继续游戏
  stopFlag = false;  // 将暂停标志位设置为false
  this->removeChild(pauseBackground);  // 将暂停背景板从布景中移除
  pause->setTexture("button/pause.png");  // 替换暂停精灵图片
  Director::getInstance()->resume();  // 恢复游戏
}

void FruitLayer::menuCallbackPauseRestart(Ref* pSender) {  // 暂停时重新开始游戏
  Director::getInstance()->resume();  // 恢复游戏
  sceneManager->goToSquishyScene(1);  // 重新切换到游戏场景
}

void FruitLayer::selectedEvent0(Ref* pSender, CheckBox::EventType type) {  // 音乐
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

void FruitLayer::selectedEvent1(Ref* pSender, CheckBox::EventType type) {  // 音效
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

void FruitLayer::wingSound() {  // 播放夹空音效
  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/sfx_wing.ogg");
}

void FruitLayer::eatSound() {  // 播放夹爆水果音效
  CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/eat.wav");
}
