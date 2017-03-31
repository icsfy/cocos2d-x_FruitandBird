#ifndef _GameLayer_H_
#define _GameLayer_H_

#include "cocos2d.h"
#include "GameSceneManager.h"

using namespace cocos2d;

class GameLayer : public Layer {
public:
  Sprite* backGround;  // 游戏背景
  NodeGrid* effectNode;  // 网络节点
  GameSceneManager* sceneManager;  // 场景管理器
public:
  virtual bool init();  // 初始化布景
  int random();  // 产生随机数
  void menuCallBack0(Ref* pSender);  // 切换到"进击的小鸟"游戏界面
  void menuCallBack1(Ref* pSender);  // 切换到"水果大逃亡"游戏界面
  void menuCallBack2(Ref* pSender);  // 切换到主菜单界面
  CREATE_FUNC(GameLayer);
};

#endif
