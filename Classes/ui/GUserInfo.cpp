//
//  GUserInfo.cpp
//  PaoPao
//
//  Created by guang on 2016/11/12.
//
//

#include "GUserInfo.h"

#include "ui/GTouchLayer.h"
#include "ui/GNumber.h"
#include "data/GCache.h"
#include "protocol_model/GModeGame.h"
#include "protocol_model/GModeUser.h"

USING_NS_CC;

bool GUserInfo::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    bg = GTouchLayer::create();
    this->addChild(bg);
    GCache::getInstance()->getUser()->login = true;
    if(GCache::getInstance()->getUser()->login)
        initLoginUI();
    else
        initUI();
    return true;
}

void GUserInfo::initUI()
{
    Size s = Director::getInstance()->getWinSize();
  
    auto info_bg = Sprite::create("jm-diban2.png");
    info_bg->setPosition(s.width/2, s.height*0.5f);
    bg->addChild(info_bg);
    
    auto info_title = Sprite::create("jm-biaoti1.png");
    info_title->setAnchorPoint(Vec2(0.5,1));
    info_title->setPosition(info_bg->getContentSize().width/2,info_bg->getContentSize().height+20);
    info_bg->addChild(info_title);
    
    auto t_title = Sprite::create("jm-biaotiWZ2.png");
    t_title->setAnchorPoint(Vec2(0.5,1));
    t_title->setPosition(Vec2(info_title->getContentSize().width/2,
                              info_title->getContentSize().height-10));
    info_title->addChild(t_title);

    
    s = info_bg->getContentSize();
    
    Button* btn = Button::create("jm-guanbi1.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setName("close");
    btn->setPosition(Vec2(s.width-20,s.height-20));
    btn->addTouchEventListener(CC_CALLBACK_2(GUserInfo::touchEvent, this));
    info_bg->addChild(btn);
    
    auto nameBg = Scale9Sprite::create("jm-tiao4.png");
    nameBg->setContentSize(Size(248,162));
    nameBg->setPosition(Vec2(s.width/2,s.height*0.62f));
    info_bg->addChild(nameBg);
    
    char c[7];
    sprintf(c, "%d",GCache::getInstance()->getUser()->skinId);
    
    std::string skinId = c;
    std::string path = "juese";
    std::string path1 = path + skinId + std::string("-1.png");
    
    auto skeleton = Sprite::create(path1);
//    skeleton->setAnchorPoint(Vec2(0, 0));
    skeleton->setPosition(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.65f);
    skeleton->setScale(1);
    nameBg->addChild(skeleton);
    
//    auto stencil = Sprite::create("jm-souye2.png");
//    stencil->setAnchorPoint(Vec2(0.5,0.5));
//    stencil->setPosition(0, 0);
//    stencil->setScale(74/stencil->getContentSize().width);
//    
//    auto clipper = ClippingNode::create();
//    clipper->setContentSize(stencil->getContentSize()*stencil->getScale());
//    clipper->setAnchorPoint(Vec2(0, 0));
//    clipper->setPosition(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.65f);
//    clipper->setAlphaThreshold(0);
//    clipper->setInverted(false);
//    nameBg->addChild(clipper,2);
//    clipper->setStencil(stencil);
//
//    char c[7];
//    sprintf(c, "%d",GCache::getInstance()->getUser()->headId);
//    std::string headPath = "head_";
//    headPath = headPath + c + std::string(".png");
//    auto info_head = Sprite::create(headPath);
//    float r = MIN(info_head->getContentSize().width, info_head->getContentSize().height);
//    info_head->setScale(clipper->getContentSize().width/r);
////    info_head->setPosition(clipper->getContentSize().width/2, clipper->getContentSize().height/2);
//    clipper->addChild(info_head);
//    clipper->setScale(1.2f);
    
    std::string userName = GCache::getInstance()->getUser()->name;
    if(GCache::getInstance()->getUser()->visitor)
        userName = _T("visitor") + userName;
    auto t_name = Text::create(userName,"",36);
    t_name->setColor(Color3B::BLACK);
    t_name->setPosition(Vec2(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.2f));
    nameBg->addChild(t_name);
    
    auto t_prompt = Text::create(_T("info_prompt"),"",18);
    t_prompt->setColor(Color3B::RED);
    t_prompt->setPosition(Vec2(s.width/2,s.height*0.32f));
    info_bg->addChild(t_prompt);
    
    btn = Button::create("an-annniu1-1.png","an-annniu1-2.png");
    btn->setScale9Enabled(true);
    btn->setContentSize(Size(216,60));
    btn->setName("weixin");
    btn->setPosition(Vec2(s.width/2-130,s.height*0.18f));
    btn->addTouchEventListener(CC_CALLBACK_2(GUserInfo::touchEvent, this));
    info_bg->addChild(btn);
    
    auto t = Sprite::create("an-WZ7.png");
    t->setPosition(btn->getContentSize().width/2,btn->getContentSize().height/2);
    btn->addChild(t);
    
    btn = Button::create("an-annniu4-1.png","an-annniu4-2.png");
    btn->setScale9Enabled(true);
    btn->setContentSize(Size(216,60));
    btn->setName("qq");
    btn->setPosition(Vec2(s.width/2+130,s.height*0.18f));
    btn->addTouchEventListener(CC_CALLBACK_2(GUserInfo::touchEvent, this));
    info_bg->addChild(btn);
    
    t = Sprite::create("an-WZ6.png");
    t->setPosition(btn->getContentSize().width/2,btn->getContentSize().height/2);
    btn->addChild(t);
}

void GUserInfo::initLoginUI()
{
    Size s = Director::getInstance()->getWinSize();
    
    auto info_bg = Sprite::create("jm-diban1.png");
    info_bg->setPosition(s.width/2, s.height*0.5f);
    bg->addChild(info_bg);
    
    auto info_title = Sprite::create("jm-biaoti1.png");
    info_title->setAnchorPoint(Vec2(0.5,1));
    info_title->setPosition(info_bg->getContentSize().width/2,info_bg->getContentSize().height+20);
    info_bg->addChild(info_title);
    
    auto t_title = Sprite::create("jm-biaotiWZ2.png");
    t_title->setAnchorPoint(Vec2(0.5,1));
    t_title->setPosition(Vec2(info_title->getContentSize().width/2,
                              info_title->getContentSize().height-10));
    info_title->addChild(t_title);
    
    s = info_bg->getContentSize();
    
    Button* btn = Button::create("jm-guanbi1.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setName("close");
    btn->setPosition(Vec2(s.width-20,s.height-20));
    btn->addTouchEventListener(CC_CALLBACK_2(GUserInfo::touchEvent, this));
    info_bg->addChild(btn);
    
    auto nameBg = Scale9Sprite::create("jm-tiao4.png");
    nameBg->setContentSize(Size(248,188));
    nameBg->setPosition(Vec2(s.width/2,s.height*0.7f));
    info_bg->addChild(nameBg);
    
    char c[7];
    sprintf(c, "%d",GCache::getInstance()->getUser()->skinId);
    
    std::string skinId = c;
    std::string path = "juese";
    std::string path1 = path + skinId + std::string("-1.png");
    
    auto skeleton = Sprite::create(path1);
//    skeleton->setAnchorPoint(Vec2(0, 0));
    skeleton->setPosition(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.65f);
    skeleton->setScale(1);
    nameBg->addChild(skeleton);

    
//    auto stencil = Sprite::create("jm-souye2.png");
//    stencil->setAnchorPoint(Vec2(0.5,0.5));
//    stencil->setPosition(0, 0);
//    stencil->setScale(74/stencil->getContentSize().width);
//    
//    auto clipper = ClippingNode::create();
//    clipper->setContentSize(stencil->getContentSize()*stencil->getScale());
//    clipper->setAnchorPoint(Vec2(0, 0));
//    clipper->setPosition(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.65f);
//    clipper->setAlphaThreshold(0);
//    clipper->setInverted(false);
//    nameBg->addChild(clipper,2);
//    clipper->setStencil(stencil);
//    
//    char c[7];
//    sprintf(c, "%d",GCache::getInstance()->getUser()->headId);
//    std::string headPath = "head_";
//    headPath = headPath + c + std::string(".png");
//    auto info_head = Sprite::create(headPath);
//    float r = MIN(info_head->getContentSize().width, info_head->getContentSize().height);
//    info_head->setScale(clipper->getContentSize().width/r);
//    clipper->addChild(info_head);
//    clipper->setScale(1.2f);
    
    std::string userName = GCache::getInstance()->getUser()->name;
    if(GCache::getInstance()->getUser()->visitor)
        userName = _T("visitor") + userName;
    auto t_name = Text::create(userName,"",34);
    t_name->setColor(Color3B::BLACK);
    t_name->setPosition(Vec2(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.27f));
    nameBg->addChild(t_name);
    
    std::string uid = "ID: "+GCache::getInstance()->getUser()->uid;
    auto t_uid = Text::create(uid,"",20);
    t_uid->setColor(Color3B::RED);
    t_uid->setPosition(Vec2(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.11f));
    nameBg->addChild(t_uid);
    
    btn = Button::create("jm-souye4.png");
    btn->setName("update");
    btn->setScale(1.2f);
    btn->setAnchorPoint(Vec2(1,1));
    btn->setPosition(Vec2(nameBg->getContentSize().width-12, nameBg->getContentSize().height-16));
    btn->addTouchEventListener(CC_CALLBACK_2(GUserInfo::touchEvent, this));
    nameBg->addChild(btn);
    
    //下面为详细信息
    auto infoBg = Sprite::create("jm-WZ5.png");
    infoBg->setAnchorPoint(Vec2(0,0));
    infoBg->setPosition(70, 55);
    info_bg->addChild(infoBg);
    //已收集皮肤数量
    auto* t_skin = GNumber::create(GCache::getInstance()->getUser()->skinNum);
    t_skin->setColor(Color3B::GREEN);
    t_skin->setAnchorPoint(Vec2(0,0.5));
    t_skin->setPosition(Vec2(190,146));
    infoBg->addChild(t_skin);
    
    auto t_sp = Sprite::create("jm-WZ6.png");
    t_sp->setAnchorPoint(Vec2(0,0.5));
    t_sp->setPosition(310, t_skin->getPositionY());
    infoBg->addChild(t_sp);
    
    RichText* t_skin_rank = RichText::create();
    sprintf(c, "%d",(int)(GCache::getInstance()->getUser()->skinNumRank*100));
    t_skin_rank->pushBackElement(RichElementText::create(0, Color3B::BLACK, 255, _T("server rank") + c+std::string("%"), "", 20));
    t_skin_rank->setAnchorPoint(Vec2(0,0.5));
    t_skin_rank->setPosition(Vec2(t_sp->getPositionX()+t_sp->getContentSize().width+10,t_sp->getPositionY()));
    infoBg->addChild(t_skin_rank);
    
    //累计杀敌数量
    auto* t_cum_kill = GNumber::create(GCache::getInstance()->getUser()->cumKill);
    t_cum_kill->setColor(Color3B::GREEN);
    t_cum_kill->setAnchorPoint(Vec2(0,0.5));
    t_cum_kill->setPosition(Vec2(t_skin->getPositionX(),t_skin->getPositionY() - 44));
    infoBg->addChild(t_cum_kill);

    t_sp = Sprite::create("jm-WZ6.png");
    t_sp->setAnchorPoint(Vec2(0,0.5));
    t_sp->setPosition(310, t_cum_kill->getPositionY());
    infoBg->addChild(t_sp);
    
    RichText* t_cum_kill_rank = RichText::create();
    sprintf(c, "%d",(int)(GCache::getInstance()->getUser()->cumKillRank*100));
    t_cum_kill_rank->pushBackElement(RichElementText::create(0, Color3B::BLACK, 255, _T("server rank") +c+std::string("%"), "", 20));
    t_cum_kill_rank->setAnchorPoint(Vec2(0,0.5));
    t_cum_kill_rank->setPosition(Vec2(t_sp->getPositionX()+t_sp->getContentSize().width+10,t_sp->getPositionY()));
    infoBg->addChild(t_cum_kill_rank);
    
    //最多击杀
    auto* t_max_kill = GNumber::create(GCache::getInstance()->getUser()->maxKill);
    t_max_kill->setColor(Color3B::GREEN);
    t_max_kill->setAnchorPoint(Vec2(0,0.5));
    t_max_kill->setPosition(Vec2(t_skin->getPositionX(),t_skin->getPositionY() - 44*2));
    infoBg->addChild(t_max_kill);
    
    t_sp = Sprite::create("jm-WZ6.png");
    t_sp->setAnchorPoint(Vec2(0,0.5));
    t_sp->setPosition(310, t_max_kill->getPositionY());
    infoBg->addChild(t_sp);
    
    RichText* t_max_kill_rank = RichText::create();
    sprintf(c, "%d",(int)(GCache::getInstance()->getUser()->maxKillRank*100));
    t_max_kill_rank->pushBackElement(RichElementText::create(0, Color3B::BLACK, 255, _T("server rank") +c+std::string("%"), "", 20));
    t_max_kill_rank->setAnchorPoint(Vec2(0,0.5));
    t_max_kill_rank->setPosition(Vec2(t_sp->getPositionX()+t_sp->getContentSize().width+10,t_sp->getPositionY()));
    infoBg->addChild(t_max_kill_rank);
    
    //MVP次数
    auto* t_mvp = GNumber::create(GCache::getInstance()->getUser()->mvp);
    t_mvp->setColor(Color3B::GREEN);
    t_mvp->setAnchorPoint(Vec2(0,0.5));
    t_mvp->setPosition(Vec2(t_skin->getPositionX(),t_skin->getPositionY() - 44*3));
    infoBg->addChild(t_mvp);
    
    t_sp = Sprite::create("jm-WZ6.png");
    t_sp->setAnchorPoint(Vec2(0,0.5));
    t_sp->setPosition(310, t_mvp->getPositionY());
    infoBg->addChild(t_sp);
    
    RichText* t_mvp_rank = RichText::create();
    sprintf(c, "%d",(int)(GCache::getInstance()->getUser()->mvpRank*100));
    t_mvp_rank->pushBackElement(RichElementText::create(0, Color3B::BLACK, 255, _T("server rank") +c+std::string("%"), "", 20));
    t_mvp_rank->setAnchorPoint(Vec2(0,0.5));
    t_mvp_rank->setPosition(Vec2(t_sp->getPositionX()+t_sp->getContentSize().width+10,t_sp->getPositionY()));
    infoBg->addChild(t_mvp_rank);
}

void GUserInfo::touchEvent(Ref *pSender, Widget::TouchEventType type)
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
            else if(name == "qq")
            {
                GCache::getInstance()->getUser()->login = true;
                this->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
            }
            else if(name == "update")
            {
                auto lay = GUserInfoUpdate::create();
                lay->setName("UserInfoUpdate");
                this->getParent()->addChild(lay,2);
                this->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
    
}


//------------------------------------GUserInfoUpdate----------------
bool GUserInfoUpdate::init()
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

void GUserInfoUpdate::initUI()
{
    Size s = Director::getInstance()->getWinSize();
    
    auto info_bg = Sprite::create("jm-diban2.png");
    info_bg->setPosition(s.width/2, s.height*0.5f);
    bg->addChild(info_bg);
    
    auto info_title = Sprite::create("jm-biaoti1.png");
    info_title->setAnchorPoint(Vec2(0.5,1));
    info_title->setPosition(info_bg->getContentSize().width/2,info_bg->getContentSize().height+20);
    info_bg->addChild(info_title);
    
    auto t_title = Sprite::create("jm-biaotiWZ3.png");
    t_title->setAnchorPoint(Vec2(0.5,1));
    t_title->setPosition(Vec2(info_title->getContentSize().width/2,
                              info_title->getContentSize().height-10));
    info_title->addChild(t_title);
    
    
    s = info_bg->getContentSize();
    
    Button* btn = Button::create("jm-guanbi1.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setName("close");
    btn->setPosition(Vec2(s.width-20,s.height-20));
    btn->addTouchEventListener(CC_CALLBACK_2(GUserInfoUpdate::touchEvent, this));
    info_bg->addChild(btn);
    
    auto nameBg = Scale9Sprite::create("jm-tiao4.png");
    nameBg->setContentSize(Size(248,162));
    nameBg->setPosition(Vec2(s.width/2,s.height*0.62f));
    info_bg->addChild(nameBg);
    
    char c[7];
    sprintf(c, "%d",GCache::getInstance()->getUser()->skinId);
    
    std::string skinId = c;
    std::string path = "juese";
    std::string path1 = path + skinId + std::string("-1.png");
    
    auto skeleton = Sprite::create(path1);
//    skeleton->setAnchorPoint(Vec2(0, 0));
    skeleton->setPosition(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.5f);
    skeleton->setScale(1.4f);
    nameBg->addChild(skeleton);

    
//    auto stencil = Sprite::create("jm-souye2.png");
//    stencil->setAnchorPoint(Vec2(0.5,0.5));
//    stencil->setPosition(0, 0);
//    stencil->setScale(74/stencil->getContentSize().width);
//    
//    auto clipper = ClippingNode::create();
//    clipper->setContentSize(stencil->getContentSize()*stencil->getScale());
//    clipper->setAnchorPoint(Vec2(0, 0));
//    clipper->setPosition(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.65f);
//    clipper->setAlphaThreshold(0);
//    clipper->setInverted(false);
//    nameBg->addChild(clipper,2);
//    clipper->setStencil(stencil);
//    
//    char c[7];
//    sprintf(c, "%d",GCache::getInstance()->getUser()->headId);
//    std::string headPath = "head_";
//    headPath = headPath + c + std::string(".png");
//    info_head = Sprite::create(headPath);
//    float r = MIN(info_head->getContentSize().width, info_head->getContentSize().height);
//    info_head->setScale(clipper->getContentSize().width/r);
//    clipper->addChild(info_head);
//    clipper->setScale(1.2f);
    
    
//    btn = Button::create("an-annniu1-1.png","an-annniu1-2.png");
//    btn->setScale9Enabled(true);
//    btn->setContentSize(Size(130,50));
//    btn->setName("head");
//    btn->setPosition(Vec2(nameBg->getContentSize().width/2, nameBg->getContentSize().height*0.2f));
//    btn->addTouchEventListener(CC_CALLBACK_2(GUserInfoUpdate::touchEvent, this));
//    nameBg->addChild(btn);
    
//    auto t_sp = Sprite::create("an-WZ8.png");
//    t_sp->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
//    btn->addChild(t_sp);
    
    auto nick_bg = Scale9Sprite::create("jm-tiao4.png");
    nick_bg->setContentSize(Size(414,64));
    nick_bg->setPosition(s.width/2, s.height*0.3f);
    info_bg->addChild(nick_bg);
    
    auto nick_bg_2 = Scale9Sprite::create("jm-tiao6.png");
    nick_bg_2->setContentSize(Size(340,38));
    nick_bg_2->setAnchorPoint(Vec2(0,0.5));
    nick_bg_2->setPosition(20, nick_bg->getContentSize().height/2);
    nick_bg->addChild(nick_bg_2);
    
    std::string userName = GCache::getInstance()->getUser()->name;
    if(GCache::getInstance()->getUser()->visitor)
        userName = _T("visitor") + userName;
    
    nameField = TextField::create(userName, "", 24);
    nameField->setTextColor(Color4B::WHITE);
    nameField->setPlaceHolderColor(Color4B::WHITE);
    nameField->setTouchSize(nick_bg_2->getContentSize());
    nameField->setTouchAreaEnabled(true);
//    nameField->setTextAreaSize(name_bg->getContentSize());
    nameField->setMaxLength(8);
    nameField->setMaxLengthEnabled(true);
    nameField->setAnchorPoint(Vec2(0.5, 0));
    nameField->setPosition(Vec2(nick_bg_2->getContentSize().width/2, 2));
    nameField->addEventListener(CC_CALLBACK_2(GUserInfoUpdate::textFieldEvent, this));
    nick_bg_2->addChild(nameField);

//    btn = Button::create("an-annniu1-1.png","an-annniu1-2.png");
//    btn->setScale9Enabled(true);
//    btn->setContentSize(Size(130,50));
//    btn->setName("nick");
//    btn->setAnchorPoint(Vec2(1,0.5));
//    btn->setPosition(Vec2(nick_bg->getContentSize().width-8, nick_bg->getContentSize().height/2));
//    btn->addTouchEventListener(CC_CALLBACK_2(GUserInfoUpdate::touchEvent, this));
//    nick_bg->addChild(btn);
    
    auto t_sp = Sprite::create("jm-souye5.png");
    t_sp->setAnchorPoint(Vec2(1,0.5));
    t_sp->setPosition(nick_bg->getContentSize().width-12, nick_bg->getContentSize().height/2);
    nick_bg->addChild(t_sp);
}

void GUserInfoUpdate::updateUI()
{
//    char c[7];
//    sprintf(c, "%d",GCache::getInstance()->getUser()->headId);
//    std::string headPath = "head_";
//    headPath = headPath + c + std::string(".png");
//    
//    info_head->initWithFile(headPath);
}

void GUserInfoUpdate::textFieldEvent(Ref *pSender, TextField::EventType type)
{
    TextField* textField = dynamic_cast<TextField*>(pSender);
    std::string s = textField->getString();
    switch (type)
    {
        case TextField::EventType::ATTACH_WITH_IME:
            if(s.length() > 24)
            {
                s = GTools::gbkSubStr(s, 0, 8);
                textField->setString(s);
            }
            break;
            
        case TextField::EventType::DETACH_WITH_IME:
            GModeUser::updateName(s);
            break;
            
        case TextField::EventType::INSERT_TEXT:
            break;
            
        case TextField::EventType::DELETE_BACKWARD:
            break;
            
        default:
            break;
    }
}


void GUserInfoUpdate::touchEvent(Ref *pSender, Widget::TouchEventType type)
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
            else if(name == "nick")
            {
                std::string name = nameField->getString();
                GModeUser::updateName(name);
            }
            else if(name == "head")
            {
                auto lay = GUserInfoUpdateHead::create();
                lay->setName("UserInfoUpdateHead");
                this->getParent()->addChild(lay,2);
//                this->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
    
}


//-------------------------GUserInfoUpdateHead------------------

bool GUserInfoUpdateHead::init()
{
    if(!Layer::init())
    {
        return false;
    }
    Size s = Director::getInstance()->getWinSize();
    bg = LayerColor::create(Color4B(0, 0, 0, 60),s.width,s.height);
    this->addChild(bg);
    
    initUI();
    return true;
}

void GUserInfoUpdateHead::initUI()
{
    Size s = Director::getInstance()->getWinSize();
    
//    auto info_bg = Sprite::create("jm-diban2.png");
//    info_bg->setPosition(s.width/2, s.height*0.5f);
//    bg->addChild(info_bg);
//    
//    auto info_title = Sprite::create("jm-biaoti1.png");
//    info_title->setAnchorPoint(Vec2(0.5,1));
//    info_title->setPosition(info_bg->getContentSize().width/2,info_bg->getContentSize().height+20);
//    info_bg->addChild(info_title);
//    
//    auto t_title = Sprite::create("jm-biaotiWZ4.png");
//    t_title->setAnchorPoint(Vec2(0.5,1));
//    t_title->setPosition(Vec2(info_title->getContentSize().width/2,
//                              info_title->getContentSize().height-10));
//    info_title->addChild(t_title);
//    
//    
//    s = info_bg->getContentSize();
//    
//    Button* btn = Button::create("jm-guanbi1.png");
//    btn->setAnchorPoint(Vec2(1,1));
//    btn->setName("close");
//    btn->setPosition(Vec2(s.width-20,s.height-20));
//    btn->addTouchEventListener(CC_CALLBACK_2(GUserInfoUpdateHead::touchEvent, this));
//    info_bg->addChild(btn);

    auto headBg = Scale9Sprite::create("jm-tishi1.png");
    headBg->setContentSize(Size(560,150));
    headBg->setPosition(Vec2(s.width/2,s.height*0.68f));
    bg->addChild(headBg);
    
//    auto headBg2 = Scale9Sprite::create("jm-tiao4.png");
//    headBg2->setContentSize(Size(560,124));
//    headBg2->setPosition(Vec2(s.width/2,s.height*0.32f));
//    info_bg->addChild(headBg2);
    
    _listView = ListView::create();
    _listView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _listView->setContentSize(headBg->getContentSize() - Size(20,10));
    _listView->setPosition(Vec2(10,2));
    headBg->addChild(_listView);
    
    for (int i = 1; i<=5; i++) {
        _listView->pushBackCustomItem(createItem(i));
    }
    _listView->jumpToTop();
}

bool GUserInfoUpdateHead::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 v = touch->getLocation();
    if(!_listView->getBoundingBox().containsPoint(v))
    {
        this->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
        return true;
    }
    return false;
}
void GUserInfoUpdateHead::onEnter()
{
    Layer::onEnter();
    
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(GUserInfoUpdateHead::onTouchBegan, this);
    touchListener->setSwallowTouches(true);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
}
void GUserInfoUpdateHead::onExit()
{
    Layer::onExit();
    _eventDispatcher->removeEventListener(touchListener);
}

Layout* GUserInfoUpdateHead::createItem(int headId)
{
    float w = _listView->getContentSize().width/4;
    float h = _listView->getContentSize().height;
    
    Layout* item = Layout::create();
    //    item->setTouchEnabled(true);
    item->setContentSize(Size(w,h));
    item->setPosition(Vec2(0,0));
    
    char c[7];
    sprintf(c, "%d",headId);
    std::string headPath = "head_";
    headPath = headPath + c + std::string(".png");
    
    auto stencil = Sprite::create("jm-souye2.png");
    stencil->setAnchorPoint(Vec2(0.5,0.5));
    stencil->setPosition(0, 0);
    stencil->setScale(74/stencil->getContentSize().width);
    
    auto clipper = ClippingNode::create();
    clipper->setContentSize(stencil->getContentSize()*stencil->getScale());
    clipper->setAnchorPoint(Vec2(0, 0));
    clipper->setPosition(Vec2(w/2, h/2));
    clipper->setAlphaThreshold(0.05f);
    clipper->setInverted(false);
    clipper->setName("itembg");
    item->addChild(clipper,2);
    clipper->setStencil(stencil);
    
    auto head = Button::create(headPath);
    head->setSwallowTouches(false);
    head->setPosition(Vec2(0, 0));
    head->setName("item");
    head->setTag(headId);
    head->addTouchEventListener(CC_CALLBACK_2(GUserInfoUpdateHead::touchEvent, this));
    float r = MIN(head->getContentSize().width, head->getContentSize().height);
    head->setScale(clipper->getContentSize().width/r);
    clipper->addChild(head);
    clipper->setScale(1.2f);
    
    auto sel = Sprite::create("jm-xuanzhong1.png");
    sel->setPosition(w/2, h/2-2);
    sel->setName("sel");
    item->addChild(sel,3);
    
    if(GCache::getInstance()->getUser()->headId != headId)
        sel->setVisible(false);
    return item;
}

void GUserInfoUpdateHead::updateUI()
{
    Vector<Widget*> list = _listView->getItems();
    for (int i=0; i<list.size(); i++) {
        Widget* node = list.at(i);
        
        auto head = node->getChildByName("itembg")->getChildByName("item");
        auto sel = node->getChildByName("sel");
        if(GCache::getInstance()->getUser()->headId == head->getTag())
        {
            sel->setVisible(true);
        }
        else
        {
            sel->setVisible(false);
        }
    }
}


void GUserInfoUpdateHead::touchEvent(Ref *pSender, Widget::TouchEventType type)
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
            else if(name == "item")
            {
                int headId = btn->getTag();
                GModeUser::updateHead(headId);
                this->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
            }
            
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
    
}
