#ifndef _FruitLayer_H_
#define _FruitLayer_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameSceneManager.h"

using namespace cocos2d;
using namespace ui;

class FruitLayer : public Layer {
public:
  int count = 0;  // 记录分数
  int boss = 0;  // boss出现次数
  int height1;  // 柱子高度
  int height2;  // 柱子高度
  bool stopFlag = false;  // 暂停标志位
  bool bossFlag = false;  // boss标志位
  bool gameFlag = true;  // 游戏开始标志位
  bool readyFlag = true;  // 游戏准备标志位
  bool overFlag = false;  // 游戏结束标志位
  bool actionFlag = false;  // 动作标志位
  NodeGrid* effectNode;  // 网格节点
  Slider* slider;  // 用于显示boss血条
  LabelAtlas* pLabelAtlas;  // 用于显示分数
  MenuItemImage* replayItem;  // 重新开始
  MenuItemImage* menuItem;  // 返回主菜单
  Sprite* bossSprite;  // boss精灵
  Sprite* overRank;  // 记分板
  Sprite* pause;  // 暂停
  Sprite* backGround;  // 背景
  Sprite* floor;  // 地面
  Sprite* ready;  // 准备
  Sprite* pauseBackground;  // 暂停背景
  Sprite* columnUnder1;  // 下柱子1
  Sprite* columnOn1;  // 上柱子1
  Sprite* columnUnder2;  // 下柱子2
  Sprite* columnOn2;  // 上柱子2
  GameSceneManager* sceneManager;  // 场景管理器
  std::vector<Sprite*> fruit;  // 存储水果精灵
public:
  virtual bool init();  // 初始化布景
  void startGame();  // 开始游戏
  int random();  // 生成随机数
  void gameOver();  // 游戏结束
  void wingSound();  // 夹空音效
  void eatSound();  // 夹破水果音效
  void replay();  // 重玩界面
  void check();  // 碰撞检测
  void moveObject();  // 游戏结束界面运动
  void initColumn1();  // 初始化柱子1
  void initColumn2();  // 初始化柱子2
  void setFlag();  // 设置动作标志位
  void update_column(float delta);  // 更新柱子
  void update_boss(float delta);  // 更新boss
  void update_fruit(float delta);  // 更新水果
  void menuCallbackRestart(Ref* pSender);  // 重新开始游戏
  void menuCallbackToMain(Ref* pSender);  // 切换到主菜单场景
  void menuCallbackPauseToMain(Ref* pSender);  // 暂停时切换到主菜单场景
  void menuCallbackResume(Ref* pSender);  // 继续游戏
  void menuCallbackPauseRestart(Ref* pSender);  // 暂停时重新开始游戏
  void selectedEvent0(Ref* pSender, CheckBox::EventType type);  // 复选框事件回调方法
  void selectedEvent1(Ref* pSender, CheckBox::EventType type);  // 复选框事件回调方法
  bool onTouchBegan(Touch* touch, Event* event);  // 开始触控事件
  bool onTouchPause(Touch* touch, Event* event);  // 暂停按钮触控事件
  CREATE_FUNC(FruitLayer);
};

#endif
