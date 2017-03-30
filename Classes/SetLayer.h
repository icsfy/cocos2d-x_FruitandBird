#ifndef _SetLayer_H_
#define _SetLayer_H_

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameSceneManager.h"

using namespace cocos2d;
using namespace ui;

class SetLayer : public Layer {
public:
  GameSceneManager* sceneManager;  // 场景管理器
public:
  virtual bool init();  // 初始化布景
  void menuCallBack(Ref* pSender);  // 返回主菜单
  void selectedEvent0(Ref* pSender, CheckBox::EventType type);  // 复选框事件回调方法
  void selectedEvent1(Ref* pSender, CheckBox::EventType type);  // 复选框事件回调方法
  CREATE_FUNC(SetLayer);
};

#endif
