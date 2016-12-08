//
//  GRelived.h
//  PaoPao
//
//  Created by guang on 2016/11/10.
//
//

#ifndef GRelived_h
#define GRelived_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "data/GJson.h"

using namespace cocos2d::ui;

class GNumber;

class GRelived : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GRelived);
    
    void initUI(GJsonObject* obj);
    void updateTime(float dt);
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
private:
    cocos2d::Layer* bg;
    GNumber* t_time;
    
    int time;
};

#endif /* GRelived_h */
