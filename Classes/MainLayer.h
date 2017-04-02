#ifndef _MainLayer_H_
#define _MainLayer_H_

#include "cocos2d.h"
#include "GameSceneManager.h"

using namespace cocos2d;

class MainLayer : public Layer {
public:
  Sprite* backGround;  // 游戏背景
  NodeGrid* effectNode;  // 网络节点
  static bool musicFlag;  // 背景音乐标志位
  static bool soundFlag;  // 即时音效标志位
  GameSceneManager* sceneManager;  // 场景管理器
public:
  virtual bool init();  // 初始化布景
  int random();  // 产生随机数
  void menuCallBack0(Ref* pSender);  // 切换到选择游戏场景
  void menuCallBack1(Ref* pSender);  // 切换到排行榜场景
  void menuCallBack2(Ref* pSender);  // 切换到游戏设置场景
  void menuCallBack3(Ref* pSender);  // 切换到游戏帮助场景
  void menuCallBack4(Ref* pSender);  // 切换到游戏关于场景
  void menuCallBack5(Ref* pSender);  // 退出游戏
  //void initTitle();  // 更新标题
  CREATE_FUNC(MainLayer);
};

#endif
