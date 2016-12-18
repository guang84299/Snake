//
//  GUserController.cpp
//  OnlineGame
//
//  Created by guang on 16/10/1.
//
//

#include "GUserController.h"
#include "scene/GScene.h"
#include "scene/GHomeScene.h"
#include "scene/GGameScene.h"

USING_NS_CC;

static GUserController* _instance = nullptr;

GUserController* GUserController::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new GUserController();
        _instance->autorelease();
    }
    return _instance;
}

void GUserController::enterHomeScene()
{
    Director::getInstance()->replaceScene(GHomeScene::createScene());
}

void GUserController::enterGameScene()
{
//    auto t = TransitionFade::create(0.1f, GGameScene::createScene(), Color3B::BLACK);
    Director::getInstance()->replaceScene(GGameScene::createScene());
}
