#ifndef _AboutLayer_H_
#define _AboutLayer_H_

#include "cocos2d.h"
#include "GameSceneManager.h"

class AboutLayer : public Layer {
public:
  GameSceneManager* sceneManager;  // 场景管理器
public:
  virtual bool init();  // 初始化布景
  void menuCallBack(Ref* pSender);  // 返回主菜单
  CREATE_FUNC(AboutLayer);
};

#endif
