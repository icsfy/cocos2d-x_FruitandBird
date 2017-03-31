#include "cocos2d.h"
#include "AboutLayer.h"

using namespace cocos2d;

bool AboutLayer::init() {
  if (!CCLayer::init())  // 调用父类的初始化
    return false;
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  Sprite* backGround = Sprite::create("pic/morning.png");  // 创建一个背景精灵对象
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

  Sprite* about = Sprite::create("pic/about.png");  // 创建关于精灵
  about->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 100));  // 设置精灵位置
  this->addChild(about);  // 将精灵添加到布景中

  auto menuItem = MenuItemImage::create(  // 返回主菜单
    "button/menu.png", "button/menu_off.png", CC_CALLBACK_1(AboutLayer::menuCallBack, this));
  menuItem->setPosition(Vec2(80, 50));
  auto menu = Menu::create(menuItem, NULL);
  menu->setPosition(Vec2::ZERO);  // 设置菜单位置
  this->addChild(menu, 10);  // 将菜单项添加到布景中

  return true;
}

void AboutLayer::menuCallBack(Ref* pSender) {
  sceneManager->goToMainScene();  // 切换到主菜单场景
}
