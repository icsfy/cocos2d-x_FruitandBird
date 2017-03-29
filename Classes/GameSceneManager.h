#ifndef __GameSceneManager_H__
#define __GameSceneManager_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameSceneManager {
public:
  Scene* mainScene;  // 主界面场景
  Scene* gameScene;  // 游戏选择界面场景
  Scene* birdScene;  // "进击的小鸟"游戏场景
  Scene* fruitScene;  // "水果大逃亡"游戏场景
  Scene* musicScene;  // 游戏设置场景
  Scene* rankScene;  // 游戏排行榜场景
  Scene* aboutScene;  // 关于场景
  Scene* helpScene;  // 帮助场景
public:
  void createMainScene();  // 创建第一次进入游戏的场景
  void goToFlappyScene(int);  // 切换到"进击的小鸟"游戏界面
  void goToSquishyScene(int);  // 切换到"水果大逃亡"游戏界面
  void goToGameScene();  // 切换到游戏选择界面
  void goToMainScene();  // 切换到主界面
  void goToSetScene();  // 切换到设置界面
  void goToRankBirdScene();  // 切换到"进击的小鸟"排行榜界面
  void goToRankFruitScene();  // 切换到"水果大逃亡"排行榜界面
  void goToAboutScene();  // 切换到关于界面
  void goToHelpScene();  // 切换到帮助界面
};

#endif  // __GameSceneManager_H__
