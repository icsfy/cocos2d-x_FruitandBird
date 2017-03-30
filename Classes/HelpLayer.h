#ifndef _HelpLayer_H_
#define _HelpLayer_H_

#include "cocos2d.h"
#include "GameSceneManager.h"

using namespace cocos2d;

class HelpLayer : public Layer {
public:
  GameSceneManager* sceneManager;  // 场景管理器
  Sprite* sp[4];  // 用于显示帮助图片
  int index = 0;  // 索引
  Point beganPoint = Point(0, 0);  // 记录触摸开始的点
  Point endPoint = Point(0, 0);  // 记录触摸结束的点
  bool moveFlag = false;  // 判断是否正在滑动标志位
public:
  virtual bool init();  // 初始化布景
  void menuCallBack(Ref* pSender);  // 返回主菜单
  bool onTouchBegan(Touch* touch, Event* event);  // 开始单点触控回调方法
  void onTouchEnded(Touch* touch, Event* event);  // 单点触控结束回调方法
  void setmoveFlag();  // 设置移动标志位方法
  CREATE_FUNC(HelpLayer);
};

#endif
