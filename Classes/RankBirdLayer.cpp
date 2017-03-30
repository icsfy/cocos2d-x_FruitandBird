#include "cocos2d.h"
#include "RankBirdLayer.h"

using namespace cocos2d;
using namespace std;

bool RankBirdLayer::init() {  // 初始化
  if (!Layer::init())
    return false;
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  Sprite* backGround = Sprite::create("pic/night.png");  // 背景
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

  Sprite* rank = Sprite::create("pic/rankBackground.png");  // 创建排行榜背景
  rank->setPosition(Vec2(270, 530));  // 设置精灵位置
  this->addChild(rank, 1);  // 添加到布景中

  Sprite* title = Sprite::create("pic/rankBird.png");  // 创建标题
  title->setPosition(Vec2(270, 850));  // 设置精灵位置
  this->addChild(title, 1);  // 添加到布景中

  auto menuItem = MenuItemImage::create(  // 切换到主菜单
    "button/menu.png", "button/menu_off.png", CC_CALLBACK_1(RankBirdLayer::menuCallBack0, this));
  menuItem->setPosition(Vec2(80, 50));
  auto nextItem = MenuItemImage::create(  // 切换到下一个排行榜
    "button/next.png", "button/next_off.png", CC_CALLBACK_1(RankBirdLayer::menuCallBack1, this));
  nextItem->setPosition(Vec2(460, 50));

  auto menu = Menu::create(menuItem, nextItem, NULL);
  menu->setPosition(Vec2::ZERO);  // 设置菜单位置
  this->addChild(menu, 10);  // 将菜单项添加到布景中

  int *tempBird = new int[5];  // 存储分数
  load();  // 读取数据
  for (int i = 0; i < 5; i++) {
    string score;  // 创建一个字符串，用于获取分数
    string number = StringUtils::format("%d", i + 1);  // 创建一个字符串，用于获取排名
    tempBird[i] = atoi(scoreBird[i].c_str());  // 将字符串转为整型
    if (tempBird[i] == 0) {
      score = "-";  // 若分数为0，则显示"-"
    } else {
      score = scoreBird[i];
    }
    // 创建一个文本，用于显示排名
    labels = Label::createWithTTF(number, "fonts/FZKATJW.ttf", 60,
                    Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
    rank->addChild(labels, 2);  // 将文本添加到背景中
    labels->setPosition(Vec2(90, 280 - 50 * i));  // 设置文本位置
    labels->enableOutline(Color4B(187, 187, 187, 255), 2);  // 设置文本字体边框颜色
    // 创建一个文本，用于显示分数
    labels = Label::createWithTTF(score, "fonts/FZKATJW.ttf", 60,
                    Size::ZERO, TextHAlignment::LEFT, TextVAlignment::TOP);
    rank->addChild(labels, 2);  // 将文本添加到背景中
    labels->setPosition(Vec2(315, 280 - 50 * i));  // 设置文本位置
    labels->enableOutline(Color4B(187, 187, 187, 255), 2);  // 设置文本字体边框颜色
  }
  delete [] tempBird;

  return true;
}

void RankBirdLayer::save(int newScore) {  // 记录分数
  string score;  // 创建一个字符串，用于记录分数
  string oldScore;  // 创建一个字符串，用于记录旧分数
  int *tempBird = new int[5];  // 创建一个整型数组，用于获取分数
  load();  // 读取数据
  for (int i = 0; i < 5; i++)  // 提取各排名分数
    tempBird[i] = atoi(scoreBird[i].c_str());  // 将分数转为整型
  for (int i = 4; i >= 0; i--) {  // 最新分数与排名分数比较
    if (newScore >= tempBird[i]) {
      score = StringUtils::format("%d", newScore);  // 将整型转为字符串
      if (i != 4) {  // 判断记录分数是否为最后一个
        oldScore = StringUtils::format("%d", tempBird[i]);  // 将整型转为字符串
        UserDefault::getInstance()->setStringForKey(  // 重置用户数据中对应键的值
          StringUtils::format("b%d", i + 1).c_str(), oldScore);
      }
      UserDefault::getInstance()->setStringForKey(  // 更新用户数据中对应键的值
        StringUtils::format("b%d", i).c_str(), score);
    } else {
      break;  // 结束循环
    }
  }
  UserDefault::getInstance()->flush();  // 刷新
  delete [] tempBird;
}

void RankBirdLayer::load() {  // 读取分数
  for (int i = 0; i < 5; i++) {
    scoreBird[i] = UserDefault::getInstance()->
            getStringForKey(StringUtils::format("b%d", i).c_str(), "0");
  }
}

void RankBirdLayer::menuCallBack0(Ref* pSender) {  // 切换到主菜单场景的菜单项回调方法
  sceneManager->goToMainScene();  // 切换到主菜单场景
}

void RankBirdLayer::menuCallBack1(Ref* pSender) {  // 切换到下一个排行榜场景的菜单项回调方法
  sceneManager->goToRankFruitScene();  // 切换到"水果大逃亡"排行榜场景
}
