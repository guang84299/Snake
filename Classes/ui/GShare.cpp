//
//  GShare.cpp
//  PaoPao
//
//  Created by guang on 2016/12/1.
//
//

#include "GShare.h"

#include "ui/GTouchLayer.h"
#include "ui/GNumber.h"
#include "data/GCache.h"
#include "tools/GJNITools.h"

USING_NS_CC;

bool GShare::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    bg = GTouchLayer::create();
    this->addChild(bg);
    initUI();
    this->setName("share");
    return true;
}

void GShare::initUI()
{
    Size s = Director::getInstance()->getWinSize();
    
    auto setting_bg = Sprite::create("jm-diban2.png");
    setting_bg->setPosition(s.width/2, s.height*0.5f);
    bg->addChild(setting_bg);
    
    auto info_title = Sprite::create("jm-biaoti1.png");
    info_title->setAnchorPoint(Vec2(0.5,1));
    info_title->setPosition(setting_bg->getContentSize().width/2,setting_bg->getContentSize().height+20);
    setting_bg->addChild(info_title);
    
    auto t_title = Sprite::create("jm-biaotiWZ8.png");
    t_title->setAnchorPoint(Vec2(0.5,1));
    t_title->setPosition(Vec2(info_title->getContentSize().width/2,
                              info_title->getContentSize().height-10));
    info_title->addChild(t_title);
    
    
    s = setting_bg->getContentSize();
    
    Button* btn = Button::create("jm-guanbi1.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setName("close");
    btn->setPosition(Vec2(s.width-20,s.height-20));
    btn->addTouchEventListener(CC_CALLBACK_2(GShare::touchEvent, this));
    setting_bg->addChild(btn);
    
    auto setBg = Scale9Sprite::create("jm-tiao4.png");
    setBg->setContentSize(Size(560,180));
    setBg->setPosition(Vec2(s.width/2,s.height*0.58f));
    setting_bg->addChild(setBg);
    
    auto t_share = Text::create(_T("share describe"),"",24);
    t_share->setColor(Color3B::BLACK);
    t_share->setAnchorPoint(Vec2(0,1));
    t_share->setTextAreaSize(Size(520,0));
    t_share->setPosition(Vec2(20,setBg->getContentSize().height-20));
    setBg->addChild(t_share);
    
    auto crystalBg = Scale9Sprite::create("jm-tiao6.png");
    crystalBg->setContentSize(Size(110,30));
    crystalBg->setAnchorPoint(Vec2(0.5,0.5));
    crystalBg->setPosition(Vec2(s.width/2,s.height*0.29f));
    setting_bg->addChild(crystalBg);
    
    auto sp_crystal = Sprite::create("jm-zuanshi1.png");
    sp_crystal->setPosition(0, crystalBg->getContentSize().height/2);
    crystalBg->addChild(sp_crystal);
    
    GNumber* t_crystal = GNumber::create(GCache::getInstance()->getShareAward());
    t_crystal->setAnchorPoint(Vec2(0,0.5));
    t_crystal->setPosition(Vec2(30,crystalBg->getContentSize().height/2));
    crystalBg->addChild(t_crystal);
    
    btn = Button::create("an-annniu5-1.png","an-annniu5-2.png");
    btn->setAnchorPoint(Vec2(0.5,0.5));
    btn->setName("share");
    btn->setPosition(Vec2(s.width/2,s.height*0.16f));
    btn->setScale(1.3f);
    btn->addTouchEventListener(CC_CALLBACK_2(GShare::touchEvent, this));
    setting_bg->addChild(btn);
    
    auto t_sp = Sprite::create("an-WZ14.png");
    t_sp->setScale(0.9f);
    t_sp->setPosition(btn->getContentSize().width/2,btn->getContentSize().height/2);
    btn->addChild(t_sp);
}

void GShare::touchEvent(Ref *pSender, Widget::TouchEventType type)
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
            else if(name == "share")
            {
                GJNITools::shareUrl();
                this->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
    
}
