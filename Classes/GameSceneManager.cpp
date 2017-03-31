#include "cocos2d.h"
#include "GameSceneManager.h"
#include "MainLayer.h"
#include "RankBirdLayer.h"
#include "RankFruitLayer.h"
#include "SetLayer.h"
#include "HelpLayer.h"
#include "AboutLayer.h"
#include "GameLayer.h"
#include "BirdLayer.h"

using namespace cocos2d;

void GameSceneManager::createMainScene() {  // 创建第一次进入游戏的场景
  mainScene = Scene::create();
  MainLayer* layer = MainLayer::create();
  layer->sceneManager = this;
  mainScene->addChild(layer);
}

void GameSceneManager::goToMainScene() {  // 切换到主界面
  Director::getInstance()->setDepthTest(true);
  mainScene = Scene::create();
  MainLayer* layer = MainLayer::create();
  mainScene->addChild(layer);
  layer->sceneManager = this;
  auto ss = TransitionFade::create(1, mainScene);
  Director::getInstance()->replaceScene(ss);  // 切换场景
}

void GameSceneManager::goToFlappyScene(int i) {  // 切换到"进击的小鸟"游戏界面
  Director::getInstance()->setDepthTest(true);
  birdScene = Scene::create();
  BirdLayer* layer = BirdLayer::create();
  birdScene->addChild(layer);
  layer->sceneManager = this;
  auto ss = TransitionFadeTR::create(1, birdScene);
  if (i == 1)
    auto ss = TransitionFade::create(1, birdScene);
  Director::getInstance()->replaceScene(ss);  // 切换场景
}

void GameSceneManager::goToSquishyScene(int i) {  // 切换到"水果大逃亡"游戏界面
  /* code */
}

void GameSceneManager::goToGameScene() {  // 切换到游戏选择界面
  Director::getInstance()->setDepthTest(true);
  gameScene = Scene::create();
  GameLayer* layer = GameLayer::create();
  gameScene->addChild(layer);
  layer->sceneManager = this;
  auto ss = TransitionPageTurn::create(1, gameScene, false);
  Director::getInstance()->replaceScene(ss);  // 切换场景
}

void GameSceneManager::goToSetScene() {  // 切换到设置界面
  Director::getInstance()->setDepthTest(true);
  musicScene = Scene::create();
  SetLayer* layer = SetLayer::create();
  musicScene->addChild(layer);
  layer->sceneManager = this;
  auto ss = TransitionPageTurn::create(1, musicScene, false);
  Director::getInstance()->replaceScene(ss);  // 切换场景
}

void GameSceneManager::goToRankBirdScene() {  // 切换到"进击的小鸟"排行榜界面
  Director::getInstance()->setDepthTest(true);
  rankScene = Scene::create();
  RankBirdLayer* layer = RankBirdLayer::create();
  rankScene->addChild(layer);
  layer->sceneManager = this;
  auto ss = TransitionPageTurn::create(1, rankScene, false);
  Director::getInstance()->replaceScene(ss);  // 切换场景
}

void GameSceneManager::goToRankFruitScene() {  // 切换到"水果大逃亡"排行榜界面
  Director::getInstance()->setDepthTest(true);
  rankScene = Scene::create();
  RankFruitLayer* layer = RankFruitLayer::create();
  rankScene->addChild(layer);
  layer->sceneManager = this;
  auto ss = TransitionPageTurn::create(1, rankScene, false);
  Director::getInstance()->replaceScene(ss);  // 切换场景
}

void GameSceneManager::goToAboutScene() {  // 切换到关于界面
  Director::getInstance()->setDepthTest(true);
  aboutScene = Scene::create();
  AboutLayer* layer = AboutLayer::create();
  aboutScene->addChild(layer);
  layer->sceneManager = this;
  auto ss = TransitionPageTurn::create(1, aboutScene, false);
  Director::getInstance()->replaceScene(ss);  // 切换场景
}

void GameSceneManager::goToHelpScene() {  // 切换到帮助界面
  Director::getInstance()->setDepthTest(true);
  helpScene = Scene::create();
  HelpLayer* layer = HelpLayer::create();
  helpScene->addChild(layer);
  layer->sceneManager = this;
  auto ss = TransitionPageTurn::create(1, helpScene, false);
  Director::getInstance()->replaceScene(ss);  // 切换场景
}
