#include "cocos2d.h"
#include "HelpLayer.h"

using namespace cocos2d;

bool HelpLayer::init() {
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

  Sprite* helpBackground = Sprite::create("pic/helpBack.png");  // // 创建帮助背景精灵对象
  helpBackground->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 - 5));  // 设置精灵位置
  this->addChild(helpBackground, 1);  // 将精灵添加到布景中

  auto menuItem = MenuItemImage::create(  // 返回主菜单
    "button/menu.png", "button/menu_off.png", CC_CALLBACK_1(HelpLayer::menuCallBack, this));
  menuItem->setPosition(Vec2(270, 50));
  auto menu = Menu::create(menuItem, NULL);
  menu->setPosition(Vec2::ZERO);  // 设置菜单位置
  this->addChild(menu, 12);  // 将菜单项添加到布景中

  for (int i = 0; i < 4; i++) {
    sp[i] = Sprite::create(StringUtils::format("pic/help%d.png", i));
    sp[i]->setPosition(Vec2(180 + i * 360, 320));
    if (i != 0) {
      sp[i]->runAction(FadeOut::create(0));
    }
  }

  Point pp[4];  // 用于绘制多边形
  pp[0] = Point(0, 0);  pp[1] = Point(360, 0);
  pp[2] = Point(360, 640);  pp[3] = Point(0, 640);
  Color4F green(0, 1, 0, 0);
  Color4F red(1, 0, 0, 0);
  DrawNode* shape = DrawNode::create();  // 创建剪裁用DrawNode
  shape->drawPolygon(pp, 4, green, 1, red);  // 绘制多边形
  ClippingNode* clipper = ClippingNode::create();  // 创建裁剪节点
  clipper->setStencil(shape);  // 设置裁剪模板
  for (int i = 0; i < 4; i++) {
    clipper->addChild(sp[i], 0);  // 将精灵添加到裁剪节点中
  }
  clipper->setPosition(Vec2(90, 160));  // 设置裁剪节点位置
  this->addChild(clipper, 10);  // 将被剪裁节点放置到布景中

  EventListenerTouchOneByOne* listenerTouch = EventListenerTouchOneByOne::create();  // 创建一个单点触摸监听
  listenerTouch->setSwallowTouches(true);  // 设置下传触摸
  listenerTouch->onTouchBegan = CC_CALLBACK_2(HelpLayer::onTouchBegan, this);  // 开始触摸时回调onTouchBegan方法
  listenerTouch->onTouchEnded = CC_CALLBACK_2(HelpLayer::onTouchEnded, this);  // 触摸结束时回调onTouchEnded方法
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerTouch, clipper);  // 添加到监听器

  return true;
}

void HelpLayer::menuCallBack(Ref* pSender) {
  sceneManager->goToMainScene();  // 切换到主菜单场景
}

bool HelpLayer::onTouchBegan(Touch* touch, Event* event) {
  if (moveFlag) {  // 判断当前图片是否正在滑动
    return false;
  }
  auto target = static_cast<Sprite*>(event->getCurrentTarget());  // 获取当前触摸对象，并转化为精灵类型
  auto location = target->convertToNodeSpace(touch->getLocation());  // 获取当前坐标
  beganPoint = location;  // 记录开始坐标
  return true;
}

void HelpLayer::onTouchEnded(Touch* touch, Event* event) {
  if (!moveFlag) {  // 判断当前图片是否正在滑动
    auto target = static_cast<Sprite*>(event->getCurrentTarget());  // 获取的当前触摸的对象，并转化为精灵类型
    auto location = target->convertToNodeSpace(touch->getLocation());  // 获取当前坐标
    endPoint = location;  // 记录结束坐标

    if (beganPoint.x - endPoint.x >= 30) {  // 图片向左滑动
      moveFlag = true;  // 设置图片滑动标志位为true
      index = (index + 1) % 4;  // 索引值更新为下一张图片
      sp[index]->setPosition(Vec2(540, 320));
      for (int i = 0; i < 3; i++) {
        if (i == index) {
          sp[i]->runAction(Spawn::create(  // 当前索引图片淡入
            MoveBy::create(1, Vec2(-360, 0)), FadeIn::create(1), NULL));
        }
        if (i == index - 1 || i == index + 1) {
          sp[i]->runAction(Spawn::create(  // 相邻索引图片淡出
            MoveBy::create(1, Vec2(-360, 0)), FadeOut::create(1), NULL));
        }
      }
      sp[3]->runAction(Sequence::create(
        Spawn::create(MoveBy::create(1, Vec2(-360, 0)), FadeIn::create(1), NULL),
        CallFunc::create(CC_CALLBACK_0(HelpLayer::setmoveFlag, this)), NULL));
    } else if (endPoint.x - beganPoint.x >= 30) {  // 图片向右滑动
      moveFlag = true;  // 设置图片滑动标志位为true
      index = (index + 3) % 4;  // 索引值更新为上一张图片
      sp[index]->setPosition(Vec2(-180, 320));
      for (int i = 3; i > 0; i--) {
        if (i == index) {
          sp[i]->runAction(Spawn::create(  // 当前索引图片淡入
            MoveBy::create(1, Vec2(360, 0)), FadeIn::create(1), NULL));
        }
        if (i == index + 1 || i == index -1) {
          sp[i]->runAction(Spawn::create(  // 相邻索引图片淡出
            MoveBy::create(1, Vec2(360, 0)), FadeOut::create(1), NULL));
        }
      }
      sp[0]->runAction(Sequence::create(
        Spawn::create(MoveBy::create(1, Vec2(360, 0)), FadeIn::create(1), NULL),
        CallFunc::create(CC_CALLBACK_0(HelpLayer::setmoveFlag, this)), NULL));
    }
  }
}

void HelpLayer::setmoveFlag() {
  moveFlag = false;  // 设置图片滑动标志位为false

  if (beganPoint.x - endPoint.x >= 30)  // 图片向左滑动
    sp[(index + 1) % 4]->setPosition(Vec2(540, 320));  // 设置下一个索引精灵位置

  if (endPoint.x - beganPoint.x >= 30)  // 图片向右滑动
    sp[(index + 3) % 4]->setPosition(Vec2(-180, 320));  // 设置上一个精灵索引位置
}
