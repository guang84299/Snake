#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "GScene.h"

#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

class GMainScene : public GScene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void login();
    void loginCallback(bool state,const char *data);
    void updateServerState(float dt);
    void updateRes(float dt);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GMainScene);
    
private:
    Text* label;
    LoadingBar* bar;
    cocos2d::Sprite* dian1;
    cocos2d::Sprite* dian2;
    cocos2d::Sprite* dian3;
    float resNum;
};

#endif // __HELLOWORLD_SCENE_H__
