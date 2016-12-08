//
//  GShare.h
//  PaoPao
//
//  Created by guang on 2016/12/1.
//
//

#ifndef GShare_h
#define GShare_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;


class GShare : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GShare);
    
    void initUI();
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
private:
    cocos2d::Layer* bg;
    
};

#endif /* GShare_h */
