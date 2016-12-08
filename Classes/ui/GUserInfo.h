//
//  GUserInfo.h
//  PaoPao
//
//  Created by guang on 2016/11/12.
//
//

#ifndef GUserInfo_h
#define GUserInfo_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;


class GUserInfo : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GUserInfo);
    
    void initUI();
    void initLoginUI();
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
private:
    cocos2d::Layer* bg;
    
};

class GUserInfoUpdate : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GUserInfoUpdate);
    
    void initUI();
    void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
    
    void updateUI();
private:
    cocos2d::Layer* bg;
    TextField* nameField;
    cocos2d::Sprite* info_head;
};

class GUserInfoUpdateHead : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GUserInfoUpdateHead);
    
    void initUI();
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    Layout* createItem(int headId);
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
    
    void updateUI();
private:
    cocos2d::Layer* bg;
    ListView* _listView;
    cocos2d::EventListenerTouchOneByOne* touchListener;
};

#endif /* GUserInfo_h */
