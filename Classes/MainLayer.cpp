#include "cocos2d.h"
#include "MainLayer.h"
#include "SimpleAudioEngine.h"
#include <time.h>

using namespace cocos2d;

bool MainLayer::musicFlag = true;  // 初始化音乐播放
bool MainLayer::soundFlag = true;  // 初始化音效播放

bool MainLayer::init() {
  if (!CCLayer::init())  // 调用父类的初始化
    return false;
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  int i = random();
  if (i % 2 == 0)
    backGround = Sprite::create("pic/morning.png");  // 创建一个背景精灵对象
  else
    backGround = Sprite::create("pic/night.png");  // 创建一个背景精灵对象
  backGround->setAnchorPoint(Point(0, 0));  // 设置背景精灵锚点
  backGround->setPosition(
    Vec2(origin.x, origin.y + visibleSize.height - backGround->getContentSize().height)
  );  // 设置背景精灵对象的位置
  this->addChild(backGround, 0);  // 将背景精灵添加到布景中
  if(musicFlag)  // 判断背景音乐是否开启
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/playscene.mp3", true);  // 播放背景音乐
  Sprite* floor = Sprite::create("pic/floor.png");  // 创建地面精灵对象
  floor->setAnchorPoint(Vec2::ZERO);  //设置地面精灵锚点
  floor->setPosition(Vec2(origin.x, origin.y));  // 设置地面精灵对象位置
  this->addChild(floor);  // 将地面精灵添加到布景中
  floor->runAction(RepeatForever::create(  // 设置地面精灵循环滚动
    Sequence::create(MoveTo::create(0.5, Vec2(-120, 0)), MoveTo::create(0, Vec2::ZERO), NULL)
  ));

  auto gameItem = MenuItemImage::create(  // 游戏开始
    "button/start.png", "button/start_off.png", CC_CALLBACK_1(MainLayer::menuCallBack0, this));
  gameItem->setPosition(Vec2(150, 600));
  auto rankItem = MenuItemImage::create(  // 排行榜
    "button/rank.png", "button/rank_off.png", CC_CALLBACK_1(MainLayer::menuCallBack1, this));
  rankItem->setPosition(Vec2(390, 600));
  auto setItem = MenuItemImage::create(  // 设置
    "button/set.png", "button/set_off.png", CC_CALLBACK_1(MainLayer::menuCallBack2, this));
  setItem->setPosition(Vec2(150, 450));
  auto helpItem = MenuItemImage::create(  // 帮助
    "button/help.png", "button/help_off.png", CC_CALLBACK_1(MainLayer::menuCallBack3, this));
  helpItem->setPosition(Vec2(390, 450));
  auto aboutItem = MenuItemImage::create(  // 帮助
    "button/about.png", "button/about_off.png", CC_CALLBACK_1(MainLayer::menuCallBack4, this));
  aboutItem->setPosition(Vec2(150, 300));
  auto exitItem = MenuItemImage::create(  // 退出
    "button/exit.png", "button/exit_off.png", CC_CALLBACK_1(MainLayer::menuCallBack5, this));
  exitItem->setPosition(Vec2(390, 300));

  auto menu = Menu::create(gameItem, rankItem, setItem, helpItem, aboutItem, exitItem, NULL);
  menu->setPosition(Vec2::ZERO);  // 设置菜单位置
  this->addChild(menu, 10);  // 将菜单项添加到布景中

  effectNode = NodeGrid::create();  // 创建一个网络节点对象
  this->addChild(effectNode, 10);  // 将网格节点添加到布景
  Sprite* title = Sprite::create("pic/title.png");  // 创建标题精灵
  title->setPosition(Vec2(270, 800));  // 设置标题精灵位置
  effectNode->addChild(title, 12);  // 将精灵添加到网络节点中
  Director::getInstance()->setDepthTest(false);  // 关闭深度检测
  effectNode->runAction(RepeatForever::create(
    Ripple3D::create(2.0f, Size(32, 24), Vec2(270, 800), 360, 2, 5)  // 3D水波涟漪特效
  ));

  return true;
}

int MainLayer::random() {  // 生成随机数
  srand((unsigned)time(NULL));  // 用系统时间作为随机种子
  int number = rand() % 4;  // 随机产生0~3的数
  return number;  // 将随机数返回
}

void MainLayer::initTitle() {  // 更新标题
  Sprite* title = Sprite::create("pic/title.png");  // 创建一个标题精灵对象
  title->setPosition(Vec2(270, 800));  // 设置标题精灵位置
  this->addChild(title, 10);  // 将标题精灵添加到布景中
  this->removeChild(effectNode);  // 移除网络节点对象
}

void MainLayer::menuCallBack0(Ref* pSender) {  // 切换到选择游戏场景的菜单项回调方法
  initTitle();  // 更新标题
  sceneManager->goToGameScene();  // 切换到选择游戏场景
}

void MainLayer::menuCallBack1(Ref* pSender) {  // 切换到排行榜场景的菜单项回调方法
  initTitle();  // 更新标题
  sceneManager->goToRankBirdScene();  // 切换到排行榜场景
}

void MainLayer::menuCallBack2(Ref* pSender) {  // 切换到游戏设置场景的菜单项回调方法
  initTitle();  // 更新标题
  sceneManager->goToSetScene();  // 切换到游戏设置场景
}

void MainLayer::menuCallBack3(Ref* pSender) {  // 切换到游戏帮助场景的菜单项回调方法
  initTitle();  // 更新标题
  sceneManager->goToHelpScene();  // 切换到游戏帮助场景
}

void MainLayer::menuCallBack4(Ref* pSender) {  // 切换到游戏关于场景的菜单项回调方法
  initTitle();  // 更新标题
  sceneManager->goToAboutScene();  // 切换到游戏关于场景
}

void MainLayer::menuCallBack5(Ref* pSender) {  // 退出游戏的菜单项回调方法
  initTitle();  // 更新标题
  Director::getInstance()->end();  // 退出游戏
}
