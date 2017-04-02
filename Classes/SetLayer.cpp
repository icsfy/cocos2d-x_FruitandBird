#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "MainLayer.h"
#include "SetLayer.h"

using namespace cocos2d;
using namespace std;
using namespace ui;

bool SetLayer::init() {
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

  Sprite* setBackground = Sprite::create("pic/setBack.png");  // 创建一个背景框精灵对象
  setBackground->setPosition(Vec2(270, 500));  // 设置背景框精灵位置
  this->addChild(setBackground, 10);  // 将背景框添加到布景中

  Sprite* music = Sprite::create("pic/music.png");  // 创建一个音乐精灵对象
  setBackground->addChild(music, 1);  // 将精灵添加到背景框精灵中
  music->setPosition(Vec2(100, 220));  // 设置精灵位置

  Sprite* sound = Sprite::create("pic/sound.png");  // 创建一个音效精灵对象
  setBackground->addChild(sound, 1);  // 将精灵添加到背景框精灵中
  sound->setPosition(Vec2(100, 140));  // 设置精灵位置

  CheckBox* checkMusic = CheckBox::create(  // 创建一个复选框对象
    "button/sound_on.png", "button/sound_off.png",  // 设置未选中和选中的背景图
    "button/sound_off.png", "button/sound_stop.png",  // 设置选中的小图标和被禁用的背景图
    "button/sound_stop.png", Widget::TextureResType::LOCAL  // 设置禁用时的小图标和纹理类型
  );
  setBackground->addChild(checkMusic, 1);  // 将复选框添加到背景框精灵中
  checkMusic->setPosition(Vec2(320, 220));  // 设置复选框位置
  checkMusic->setSelected(!MainLayer::musicFlag);  // 根据音乐标志位设置当前复选框状态
  checkMusic->addEventListener(CC_CALLBACK_2(SetLayer::selectedEvent0, this));  // 为复选框添加选择监听

  CheckBox* checkSound = CheckBox::create(  // 创建一个复选框对象
    "button/sound_on.png", "button/sound_off.png",  // 设置未选中和选中的背景图
    "button/sound_off.png", "button/sound_stop.png",  // 设置选中的小图标和被禁用的背景图
    "button/sound_stop.png", Widget::TextureResType::LOCAL  // 设置禁用时的小图标和纹理类型
  );
  setBackground->addChild(checkSound, 1);  // 将复选框添加到背景框精灵中
  checkSound->setPosition(Vec2(320, 140));  // 设置复选框位置
  checkSound->setSelected(!MainLayer::soundFlag);  // 根据音效标志位设置当前复选框状态
  checkSound->addEventListener(CC_CALLBACK_2(SetLayer::selectedEvent1, this));  // 为复选框添加选择监听

  auto menuItem = MenuItemImage::create(  // 返回主菜单
    "button/menu.png", "button/menu_off.png", CC_CALLBACK_1(SetLayer::menuCallBack, this));
  menuItem->setPosition(Vec2(200, 60));
  auto menu = Menu::create(menuItem, NULL);
  menu->setPosition(Vec2::ZERO);  // 设置菜单位置
  setBackground->addChild(menu, 1);  // 将菜单项添加到布景中

  NodeGrid* effectNode = NodeGrid::create();  // 创建一个网络节点对象
  this->addChild(effectNode, 10);  // 将网格节点添加到布景
  Sprite* title = Sprite::create("pic/title.png");  // 创建标题精灵
  title->setPosition(Vec2(270, 800));  // 设置标题精灵位置
  effectNode->addChild(title, 12);  // 将精灵添加到网络节点中
  Director::getInstance()->setDepthTest(false);  // 关闭深度检测
/* ?? doesn't work ??
  effectNode->runAction(RepeatForever::create(
    Ripple3D::create(2.0f, Size(32, 24), Vec2(270, 800), 360, 2, 5)  // 3D水波涟漪特效
  ));
*/
  return true;
}

void SetLayer::menuCallBack(Ref* pSender) {
  sceneManager->goToMainScene();  // 切换到主菜单场景
}

void SetLayer::selectedEvent0(Ref* pSender, CheckBox::EventType type) {
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

void SetLayer::selectedEvent1(Ref* pSender, CheckBox::EventType type) {
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
