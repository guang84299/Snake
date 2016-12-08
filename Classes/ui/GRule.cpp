//
//  GRule.cpp
//  PaoPao
//
//  Created by guang on 2016/11/17.
//
//

#include "GRule.h"

#include "ui/GTouchLayer.h"
#include "data/GCache.h"
#include "protocol_model/GModeGame.h"
#include "protocol_model/GModeUser.h"

#include <spine/spine-cocos2dx.h>
#include "spine/spine.h"
using namespace spine;

USING_NS_CC;

bool GRule::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    bg = GTouchLayer::create();
    this->addChild(bg);
    initUI();
    return true;
}

void GRule::initUI()
{
    Size s = Director::getInstance()->getWinSize();
    
    auto rule_bg = Sprite::create("jm-diban1.png");
    rule_bg->setPosition(s.width/2, s.height*0.5f);
    bg->addChild(rule_bg);
    
    auto info_title = Sprite::create("jm-biaoti1.png");
    info_title->setAnchorPoint(Vec2(0.5,1));
    info_title->setPosition(rule_bg->getContentSize().width/2,rule_bg->getContentSize().height+20);
    rule_bg->addChild(info_title);
    
    auto t_title = Sprite::create("jm-biaotiWZ7.png");
    t_title->setAnchorPoint(Vec2(0.5,1));
    t_title->setPosition(Vec2(info_title->getContentSize().width/2,
                              info_title->getContentSize().height-10));
    info_title->addChild(t_title);
    
    
    s = rule_bg->getContentSize();
    
    Button* btn = Button::create("jm-guanbi1.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setName("close");
    btn->setPosition(Vec2(s.width-20,s.height-20));
    btn->addTouchEventListener(CC_CALLBACK_2(GRule::touchEvent, this));
    rule_bg->addChild(btn,2);
    
    auto setBg = Scale9Sprite::create("jm-tiao1.png");
    setBg->setContentSize(Size(630,80));
    setBg->setPosition(Vec2(s.width/2,s.height*0.78f));
    rule_bg->addChild(setBg);
    
    auto sp = Sprite::create("jm-guizheWZ1.png");
    sp->setPosition(setBg->getContentSize().width/2, setBg->getContentSize().height/2);
    setBg->addChild(sp);

    PageView* pageView = PageView::create();
    pageView->setDirection(PageView::Direction::HORIZONTAL);
    pageView->setContentSize(Size(640,358));
    pageView->setPosition(Vec2(26,33));
    pageView->removeAllPages();
//    pageView->setIndicatorEnabled(true);
//    pageView->setIndicatorSelectedIndexColor(Color3B::BLUE);
//    pageView->setIndicatorIndexNodesColor(Color3B::BLACK);
    
    pageView->addPage(createPage(0,pageView->getContentSize()));
    
    rule_bg->addChild(pageView);
}

Layout* GRule::createPage(int index,Size s)
{
    Layout* layout = Layout::create();
    layout->setContentSize(s);
    if(index == 0)
    {
        auto sp = Sprite::create("jm-guizhe1.png");
        sp->setPosition(s.width/2, s.height/2);
        layout->addChild(sp);
    }
    
    return layout;
}

void GRule::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    std::string name = btn->getName();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            GTools::playSound(SOUND_BUTTON);
            break;
            
        case Widget::TouchEventType::MOVED:
            break;
            
        case Widget::TouchEventType::ENDED:
            
            if(name == "close")
            {
                this->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
            }
            else if(name == "weixin")
            {
                GCache::getInstance()->getUser()->login = true;
                this->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
            }
            
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
    
}
