//
//  GTouchLayer.cpp
//  PaoPao
//
//  Created by guang on 2016/10/27.
//
//

#include "GTouchLayer.h"

USING_NS_CC;

bool GTouchLayer::init()
{
    if (!Layer::init() )
    {
        return false;
    }
    Size s = Director::getInstance()->getWinSize();
    
    colorBg = LayerColor::create(Color4B(0, 0, 0, 60),s.width,s.height);
    this->addChild(colorBg);
    
    return true;
}

void GTouchLayer::setColor(cocos2d::Color4B color)
{
    colorBg->initWithColor(color);
}

bool GTouchLayer::onTouchBegan(Touch* touch, Event* event)
{
    return true;
}

void GTouchLayer::onEnter()
{
    Layer::onEnter();
    
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GTouchLayer::onTouchBegan, this);
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
}

void GTouchLayer::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(touchListener);
}
