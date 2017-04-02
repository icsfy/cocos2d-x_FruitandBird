#include "cocos2d.h"
#include "GameLayer.h"
#include <time.h>

using namespace cocos2d;

bool GameLayer::init() {
  if (!CCLayer::init())  // 调用父类的初始化
    return false;
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  if (random())
    backGround = Sprite::create("pic/morning.png");  // 创建一个背景精灵对象
  else
    backGround = Sprite::create("pic/night.png");  // 创建一个背景精灵对象
  backGround->setAnchorPoint(Point(0, 0));  // 设置背景精灵锚点
  backGround->setPosition(
    Vec2(origin.x, origin.y + visibleSize.height - backGround->getContentSize().height)
  );  // 设置背景精灵对象的位置
  this->addChild(backGround, 0);  // 将背景精灵添加到布景中

  Sprite* floor = Sprite::create("pic/floor.png");  // 创建地面精灵对象
  floor->setAnchorPoint(Vec2::ZERO);  //设置地面精灵锚点
  floor->setPosition(Vec2(origin.x, origin.y));  // 设置地面精灵对象位置
  this->addChild(floor);  // 将地面精灵添加到布景中
  floor->runAction(RepeatForever::create(  // 设置地面精灵循环滚动
    Sequence::create(MoveTo::create(0.5, Vec2(-120, 0)), MoveTo::create(0, Vec2::ZERO), NULL)
  ));

  effectNode = NodeGrid::create();  // 创建一个网络节点对象
  this->addChild(effectNode, 10);  // 将网格节点添加到布景
  Sprite* title = Sprite::create("pic/title.png");  // 创建标题精灵
  title->setPosition(Vec2(270, 800));  // 设置标题精灵位置
  effectNode->addChild(title, 12);  // 将精灵添加到网络节点中
  Director::getInstance()->setDepthTest(false);  // 关闭深度检测
/* ?? doesn't work ??
  effectNode->runAction(RepeatForever::create(
    Ripple3D::create(2.0f, Size(32, 24), Vec2(270, 800), 360, 2, 10)  // 3D水波涟漪特效
  ));
*/
  auto birdItem = MenuItemImage::create(  // "进击的小鸟"菜单项
    "button/bird.png", "button/bird_off.png", CC_CALLBACK_1(GameLayer::menuCallBack0, this));
  birdItem->setPosition(Vec2(visibleSize.width/2 - 50, visibleSize.height/2 + 100));

  auto fruitItem = MenuItemImage::create(  // "水果大逃亡"菜单项
    "button/fruit.png", "button/fruit_off.png", CC_CALLBACK_1(GameLayer::menuCallBack1, this));
  fruitItem->setPosition(Vec2(visibleSize.width/2 + 50, visibleSize.height/2 - 100));

  auto menuItem = MenuItemImage::create(  // 返回主菜单
    "button/menu.png", "button/menu_off.png", CC_CALLBACK_1(GameLayer::menuCallBack2, this));
  menuItem->setPosition(Vec2(80, 50));

  auto menu = Menu::create(menuItem, birdItem, fruitItem, NULL);
  menu->setPosition(Vec2::ZERO);  // 设置菜单位置
  this->addChild(menu, 12);  // 将菜单项添加到布景中

  return true;
}

int GameLayer::random() {  // 生成随机数
  srand((unsigned)time(NULL));  // 用系统时间作为随机种子
  return rand() % 2;  // 将随机数返回，值为0或1
}

void GameLayer::menuCallBack0(Ref* pSender) {
  sceneManager->goToFlappyScene(0);  // 切换到"进击的小鸟"游戏界面
}

void GameLayer::menuCallBack1(Ref* pSender) {
  sceneManager->goToSquishyScene(0);  // 切换到"水果大逃亡"游戏界面
}

void GameLayer::menuCallBack2(Ref* pSender) {
  sceneManager->goToMainScene();  // 切换到主菜单界面
}
