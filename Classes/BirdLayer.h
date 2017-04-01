#ifndef _BirdLayer_H_
#define _BirdLayer_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameSceneManager.h"

using namespace cocos2d;
using namespace ui;

class BirdLayer : public Layer {
public:
  int count = 0;  // 记录分数
  bool stopFlag = false;  // 暂停标志位
  bool runFlag = false;  // 小鸟运动标志位
  bool columnFlag1 = false;  // 穿过的柱子标志位1
  bool columnFlag2 = false;  // 穿过的柱子标志位2
  bool gameFlag = true;  // 游戏开始标志位
  bool readyFlag = true;  // 游戏准备标志位
  bool overFlag = false;  // 游戏结束标志位
  NodeGrid* effectNode;  // 声明网格
  LabelAtlas* pLabelAtlas;  // 文本标签
  MenuItemImage* replayItem;  // 重新开始
  MenuItemImage* menuItem;  // 返回主菜单
  Sprite* overRank;  // 记分板
  Sprite* pause;  // 暂停
  Sprite* backGround;  // 背景
  Sprite* floor;  // 地面
  Sprite* bird;  // 小鸟
  Sprite* ready;  // 准备
  Sprite* pauseBackground;  // 暂停背景
  Scale9Sprite* columnUnder1;  // 下柱子1
  Scale9Sprite* columnOn1;  // 上柱子1
  Scale9Sprite* columnUnder2;  // 下柱子2
  Scale9Sprite* columnOn2;  // 上柱子2
  Animate* animAc;  // 动画
  GameSceneManager* sceneManager;  // 场景管理器
public:
  virtual bool init();  // 初始化布景
  void initBird();  // 初始化小鸟
  void startGame();  // 开始游戏
  void gameOver();  // 游戏结束
  void birdRun();  // 小鸟运动
  void wingSound();  // 翅膀挥动音效
  void pointSound();  // 得分音效
  void hitSound();  // 撞击音效
  void dieSound();  // 死亡音效
  void moveObject();  // 游戏结束界面运动
  void initColumn1();  // 初始化柱子1
  void initColumn2();  // 初始化柱子2
  void setRunFlag1();  // 设置小鸟运动标志位1
  void setRunFlag2();  // 设置小鸟运动标志位2
  void replay();  // 重玩界面
  int random();  // 生成随机数
  void update_column(float delta);  // 更新柱子
  void update_bird(float delta);  // 更新小鸟碰撞检测
  void menuCallbackRestart(Ref* pSender);  // 重新开始游戏
  void menuCallbackToMain(Ref* pSender);  // 切换到主菜单场景
  void menuCallbackPauseToMain(Ref* pSender);  // 暂停时切换到主菜单场景
  void menuCallbackResume(Ref* pSender);  // 继续游戏
  void menuCallbackPauseRestart(Ref* pSender);  // 暂停时重新开始游戏
  void selectedEvent0(Ref* pSender, CheckBox::EventType type);  // 复选框事件回调方法
  void selectedEvent1(Ref* pSender, CheckBox::EventType type);  // 复选框事件回调方法
  bool onTouchBegan(Touch* touch, Event* event);  // 开始触控事件
  bool onTouchPause(Touch* touch, Event* event);  // 暂停按钮触控事件
  CREATE_FUNC(BirdLayer);
};

#endif
