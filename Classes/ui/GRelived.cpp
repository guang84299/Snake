//
//  GRelived.cpp
//  PaoPao
//
//  Created by guang on 2016/11/10.
//
//

#include "GRelived.h"

#include "ui/GTouchLayer.h"
#include "data/GCache.h"
#include "protocol_model/GModeGame.h"
#include "ui/GNumber.h"
USING_NS_CC;

bool GRelived::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    bg = GTouchLayer::create();
    this->addChild(bg);
    return true;
}

void GRelived::initUI(GJsonObject* obj)
{    
    std::string killMe = obj->getString("killMe");
    std::string killMeName = obj->getString("killMeName");
    int rank = obj->getInt("exp");
    int kill = obj->getInt("kill");
    GCache::getInstance()->setKillMeUid(killMe);
    int len = GTools::gbkStrLen(killMeName);
    if(len > 8)
    {
        int to = 4;
        if(killMeName.length() >= 21)
            to = 2;
        std::string name_head = GTools::gbkSubStr(killMeName, 0, to);
        std::string name_end = GTools::gbkSubStr(killMeName, len-2, len);
        killMeName = name_head + ".." + name_end;
    }
    
    Size s = Director::getInstance()->getWinSize();
    
//    Button* relived_home = Button::create("relived_home.png");
//    relived_home->setName("relived_home");
//    relived_home->setAnchorPoint(Vec2(0,1));
//    relived_home->setPosition(Vec2(20, s.height-20));
//    relived_home->addTouchEventListener(CC_CALLBACK_2(GRelived::touchEvent, this));
//    bg->addChild(relived_home);
    
    auto relived_bg = Sprite::create("jm-diban2.png");
    relived_bg->setPosition(s.width/2, s.height*0.7f);
    bg->addChild(relived_bg);
    
    
    auto txt_bg = Scale9Sprite::create("jm-tiao4.png");
    txt_bg->setContentSize(Size(relived_bg->getContentSize().width*0.85f,80));
    txt_bg->setPosition(relived_bg->getContentSize().width/2, relived_bg->getContentSize().height*0.7f);
    relived_bg->addChild(txt_bg);
    
    uint32_t flags = 0;
    flags |= RichElementText::BOLD_FLAG;
    flags |= RichElementText::OUTLINE_FLAG;
    
    auto sp = Sprite::create("ui-wenzhi2.png");
    sp->setAnchorPoint(Vec2(1,0.5));
    sp->setScale(10/7.f);
    sp->setPosition(txt_bg->getContentSize().width/2-10,
                    txt_bg->getContentSize().height/2);
    txt_bg->addChild(sp);
    
    if(killMeName == "0")
        killMeName = _T("wall");
    auto info = Text::create(killMeName,"",42);
    info->setTextColor(Color4B::BLACK);
    info->setAnchorPoint(Vec2(0,0.5));
    info->setPosition(Vec2(sp->getPositionX() + 10, sp->getPositionY()));
    txt_bg->addChild(info);
    
//    sp = Sprite::create("jm-WZ2.png");
//    sp->setAnchorPoint(Vec2(0,0.5));
//    sp->setScale(10/6.f);
//    sp->setPosition(info->getPositionX()+ info->getContentSize().width/2+10, info->getPositionY());
//    txt_bg->addChild(sp);
    
    auto t_rank = Sprite::create("ui-wenzhi1.png");
    t_rank->setAnchorPoint(Vec2(1,0.5));
    t_rank->setScale(10/7.f);
    t_rank->setPosition(Vec2(relived_bg->getContentSize().width/2+10,
                             relived_bg->getContentSize().height*0.43f));
    relived_bg->addChild(t_rank);
    
    auto n_rank = GNumber::create(rank);
    n_rank->setColor(Color3B(255,219,67));
    n_rank->setAnchorPoint(Vec2(0,0.5));
    n_rank->setScale(10/7.f);
    n_rank->setPosition(t_rank->getPositionX()+10,t_rank->getPositionY());
    relived_bg->addChild(n_rank);
    
    auto t_kill = Sprite::create("jm-WZ4.png");
    t_kill->setAnchorPoint(Vec2(1,0.5));
    t_kill->setScale(10/7.f);
    t_kill->setPosition(Vec2(relived_bg->getContentSize().width/2+10,
                             relived_bg->getContentSize().height*0.25f));
    relived_bg->addChild(t_kill);
    
    auto n_kill = GNumber::create(kill);
    n_kill->setColor(Color3B(109,244,78));
    n_kill->setAnchorPoint(Vec2(0,0.5));
    n_kill->setScale(10/7.f);
    n_kill->setPosition(t_kill->getPositionX()+10,t_kill->getPositionY());
    relived_bg->addChild(n_kill);

    relived_bg->setScale(0.6f);
    
    Button* btn = Button::create("an-annniu2-1.png","an-annniu2-2.png");
    btn->setName("relived");
    btn->setPosition(Vec2(s.width/2,s.height*0.38f));
    btn->addTouchEventListener(CC_CALLBACK_2(GRelived::touchEvent, this));
    bg->addChild(btn);
    
    auto title = Sprite::create("an-WZ5.png");
    title->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    btn->addChild(title);


    time = 5;
    
    t_time = GNumber::create(time);
    t_time->setScale(2);
    t_time->setPosition(Vec2(s.width/2,s.height*0.2f));
    bg->addChild(t_time);
    
    
    this->schedule(SEL_SCHEDULE(&GRelived::updateTime),1);
}

void GRelived::updateTime(float dt)
{
    time--;
    
//    char c[7];
//    sprintf(c, "%d",time);
    t_time->setNum(time);
    
    if(time <= 0)
    {
        GModeGame::relived();
        this->unschedule(SEL_SCHEDULE(&GRelived::updateTime));
        this->runAction(Sequence::create(DelayTime::create(0.2f),RemoveSelf::create(), NULL));
    }
}

void GRelived::touchEvent(Ref *pSender, Widget::TouchEventType type)
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
            
            if(name == "relived")
            {
                GModeGame::relived();
                this->runAction(Sequence::create(DelayTime::create(0.2f),RemoveSelf::create(), NULL));
            }
            else if(name == "relived_home")
            {
                GModeGame::questLeaveRoom();
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }

}
