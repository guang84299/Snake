//
//  GSetting.h
//  PaoPao
//
//  Created by guang on 2016/11/16.
//
//

#ifndef GSetting_h
#define GSetting_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;


class GSetting : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GSetting);
    
    void initUI();
    void sliderEvent(Ref *pSender, Slider::EventType type);
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
private:
    cocos2d::Layer* bg;
    
};

#endif /* GSetting_h */
