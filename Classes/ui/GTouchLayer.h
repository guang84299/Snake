//
//  GTouchLayer.h
//  PaoPao
//
//  Created by guang on 2016/10/27.
//
//

#ifndef GTouchLayer_h
#define GTouchLayer_h

#include "cocos2d.h"
#include "tools/GTools.h"
#include "common/GCommon.h"

class GTouchLayer : public cocos2d::Layer
{
public:
    virtual bool init() override;
    
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) override;
    virtual void onEnter() override;
    virtual void onExit() override;
    CREATE_FUNC(GTouchLayer);
    
    void setColor(cocos2d::Color4B color);
private:
    cocos2d::LayerColor* colorBg;
    cocos2d::EventListenerTouchOneByOne* touchListener;
    
};
#endif /* GTouchLayer_h */
