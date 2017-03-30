#ifndef _RankFruitLayer_H_
#define _RankFruitLayer_H_

#include "cocos2d.h"
#include "GameSceneManager.h"

using namespace cocos2d;

class RankFruitLayer : public Layer {
public:
  std::string scoreFruit[5];  // 记录分数的字符串数组
  Label* labels;  // 显示分数的文本
  GameSceneManager* sceneManager;  // 场景管理器
public:
  virtual bool init();  // 初始化布景
  void save(int);  // 记录分数
  void load();  // 读取分数
  void menuCallBack0(Ref* pSender);  // 切换到主菜单场景
  void menuCallBack1(Ref* pSender);  // 切换到下一个排行榜场景
  CREATE_FUNC(RankFruitLayer);
};
#endif
