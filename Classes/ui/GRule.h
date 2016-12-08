//
//  GRule.h
//  PaoPao
//
//  Created by guang on 2016/11/17.
//
//

#ifndef GRule_h
#define GRule_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;


class GRule : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GRule);
    
    void initUI();
    Layout* createPage(int index,cocos2d::Size s);
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
private:
    cocos2d::Layer* bg;
    
};

#endif /* GRule_h */
