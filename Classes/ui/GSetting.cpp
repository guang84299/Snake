//
//  GSetting.cpp
//  PaoPao
//
//  Created by guang on 2016/11/16.
//
//

#include "GSetting.h"


#include "ui/GTouchLayer.h"
#include "data/GCache.h"
#include "protocol_model/GModeGame.h"
#include "protocol_model/GModeUser.h"

USING_NS_CC;

bool GSetting::init()
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

void GSetting::initUI()
{
    Size s = Director::getInstance()->getWinSize();
    
    auto setting_bg = Sprite::create("jm-diban2.png");
    setting_bg->setPosition(s.width/2, s.height*0.5f);
    bg->addChild(setting_bg);
    
    auto info_title = Sprite::create("jm-biaoti1.png");
    info_title->setAnchorPoint(Vec2(0.5,1));
    info_title->setPosition(setting_bg->getContentSize().width/2,setting_bg->getContentSize().height+20);
    //setting_bg->addChild(info_title);
    
    auto t_title = Sprite::create("jm-biaotiWZ5.png");
    t_title->setAnchorPoint(Vec2(0.5,1));
	t_title->setPosition(Vec2(setting_bg->getContentSize().width / 2, setting_bg->getContentSize().height - 30));
	setting_bg->addChild(t_title);
    
    
    s = setting_bg->getContentSize();

	auto setBg2 = Scale9Sprite::create("jm-tiao1.png");
	setBg2->setContentSize(Size(670, 40));
	setBg2->setPosition(Vec2(s.width / 2, s.height*0.82f));
	setting_bg->addChild(setBg2);
    
    Button* btn = Button::create("jm-guanbi1.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setName("close");
    btn->setPosition(Vec2(s.width-40,s.height-35));
    btn->addTouchEventListener(CC_CALLBACK_2(GSetting::touchEvent, this));
    setting_bg->addChild(btn);
    
    auto setBg = Scale9Sprite::create("jm-tiao4.png");
    setBg->setContentSize(Size(560,s.height*0.3f));
    setBg->setPosition(Vec2(s.width/2,s.height*0.6f));
    setting_bg->addChild(setBg);

    auto sp = Sprite::create("jm-shezhiWZ1.png");
    sp->setAnchorPoint(Vec2(0.5,0.5));
    sp->setPosition(Vec2(150, setBg->getContentSize().height*0.74f));
    setBg->addChild(sp);
    
    Slider* slider = Slider::create();
    slider->loadBarTexture("jm-shezhi1.png");
    slider->loadSlidBallTextures("jm-shezhi2.png", "jm-shezhi2.png", "");
    slider->setName("music");
    slider->setMaxPercent(100);
    slider->setPercent(100/4+3);
    slider->setPosition(Vec2(sp->getPositionX(), setBg->getContentSize().height*0.32f));
    slider->addEventListener(CC_CALLBACK_2(GSetting::sliderEvent, this));
    setBg->addChild(slider);

    if(GCache::isMusic())
        slider->setPercent(100/4*3-3);
    
    sp = Sprite::create("jm-shezhiWZ2.png");
    sp->setAnchorPoint(Vec2(0.5,0.5));
	sp->setPosition(Vec2(setBg->getContentSize().width - 150, setBg->getContentSize().height*0.74f));
    setBg->addChild(sp);
    
    slider = Slider::create();
    slider->loadBarTexture("jm-shezhi1.png");
    slider->loadSlidBallTextures("jm-shezhi2.png", "jm-shezhi2.png", "");
    slider->setName("sound");
    slider->setMaxPercent(100);
    slider->setPercent(100/4+3);
    slider->setPosition(Vec2(sp->getPositionX(), setBg->getContentSize().height*0.32f));
    slider->addEventListener(CC_CALLBACK_2(GSetting::sliderEvent, this));
    setBg->addChild(slider);
    
    if(GCache::isSound())
        slider->setPercent(100/4*3-3);
    
	auto setBg3 = Scale9Sprite::create("jm-tiao4.png");
	setBg3->setContentSize(Size(260, s.height*0.3f));
	setBg3->setPosition(Vec2(s.width / 2, s.height*0.28f));
	setting_bg->addChild(setBg3);

	sp = Sprite::create("jm-shezhiWZ5.png");
	sp->setAnchorPoint(Vec2(0.5, 0.5));
	sp->setPosition(Vec2(setBg3->getContentSize().width/2, setBg3->getContentSize().height*0.74f));
	setBg3->addChild(sp);

	slider = Slider::create();
	slider->loadBarTexture("jm-shezhi1.png");
	slider->loadSlidBallTextures("jm-shezhi2.png", "jm-shezhi2.png", "");
	slider->setName("yaogan");
	slider->setMaxPercent(100);
	slider->setPercent(100 / 4 + 3);
	slider->setPosition(Vec2(sp->getPositionX(), setBg3->getContentSize().height*0.32f));
	slider->addEventListener(CC_CALLBACK_2(GSetting::sliderEvent, this));
	setBg3->addChild(slider);

	if (GCache::isRightHand())
		slider->setPercent(100 / 4 * 3 - 3);
	/*
    auto radioButtonGroup = RadioButtonGroup::create();
    setBg->addChild(radioButtonGroup);
    
    RadioButton* radioButton = RadioButton::create("radio_button_off.png", "radio_button_on.png");
    radioButton->setPosition(Vec2(setBg->getContentSize().width/2-50, 30));
    radioButton->setName("left");
    radioButtonGroup->addRadioButton(radioButton);
    radioButton->addEventListener(CC_CALLBACK_2(GSetting::onChangedRadioButtonSelect, this));
    setBg->addChild(radioButton);
    
    
    Text* left = Text::create("LEFT","",16);
    left->setAnchorPoint(Vec2(1,0.5));
    left->setPosition(Vec2(radioButton->getPositionX() - radioButton->getContentSize().width/2 - 10,
                           radioButton->getPositionY()));
    setBg->addChild(left);


    radioButton = RadioButton::create("radio_button_off.png", "radio_button_on.png");
    radioButton->setPosition(Vec2(setBg->getContentSize().width/2+50, 30));
    radioButton->setName("right");
    radioButtonGroup->addRadioButton(radioButton);
    radioButton->addEventListener(CC_CALLBACK_2(GSetting::onChangedRadioButtonSelect, this));
    setBg->addChild(radioButton);
    
    if(GCache::isRightHand())
    {
        radioButtonGroup->setSelectedButton(1);
    }
    
    Text* right = Text::create("RIGHT","",16);
    right->setAnchorPoint(Vec2(1,0.5));
    right->setPosition(Vec2(radioButton->getPositionX() - radioButton->getContentSize().width/2 - 10,
                           radioButton->getPositionY()));
    setBg->addChild(right);
    */
    //账号信息
//    if(GCache::getInstance()->getUser()->login)
//    {
//        auto t_sp = Sprite::create("jm-shezhiWZ3.png");
//        t_sp->setAnchorPoint(Vec2(1,0.5));
//        t_sp->setPosition(s.width/2-10,s.height*0.26f);
//        setting_bg->addChild(t_sp);
//        
//        btn = Button::create("an-annniu5-1.png","an-annniu5-2.png");
//        btn->setName("login");
//        btn->setAnchorPoint(Vec2(0,0.5));
//        btn->setPosition(Vec2(s.width/2+10,s.height*0.26f));
//        btn->addTouchEventListener(CC_CALLBACK_2(GSetting::touchEvent, this));
//        setting_bg->addChild(btn);
//        
//        t_sp = Sprite::create("an-WZ11.png");
//        t_sp->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
//        btn->addChild(t_sp);
//    }
//    else
//    {
//        auto t_sp = Sprite::create("jm-shezhiWZ4.png");
//        t_sp->setAnchorPoint(Vec2(1,0.5));
//        t_sp->setPosition(s.width/2-10,s.height*0.26f);
//        setting_bg->addChild(t_sp);
//        
//        btn = Button::create("an-annniu1-1.png","an-annniu1-2.png");
//        btn->setName("loginout");
//        btn->setAnchorPoint(Vec2(0,0.5));
//        btn->setPosition(Vec2(s.width/2+10,s.height*0.26f));
//        btn->addTouchEventListener(CC_CALLBACK_2(GSetting::touchEvent, this));
//        setting_bg->addChild(btn);
//        
//        t_sp = Sprite::create("an-WZ10.png");
//        t_sp->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
//        btn->addChild(t_sp);
//    }
}


void GSetting::onChangedRadioButtonSelect(RadioButton* radioButton, RadioButton::EventType type)
{
    if(radioButton == nullptr)
    {
        return;
    }
    std::string name = radioButton->getName();
    switch (type)
    {
        case RadioButton::EventType::SELECTED:
        {
            if (name == "left") {
                GCache::setHandStyle(false);
            }
            else
            {
                 GCache::setHandStyle(true);
            }
            break;
        }
            
        case RadioButton::EventType::UNSELECTED:
        {
            radioButton->setSelected(false);
            break;
        }
        default:
            break;
    }

}

void GSetting::sliderEvent(Ref *pSender, Slider::EventType type)
{
    Slider* slider = dynamic_cast<Slider*>(pSender);
    int percent = slider->getPercent();
    int maxPercent = slider->getMaxPercent();
    std::string name = slider->getName();
    if (type == Slider::EventType::ON_PERCENTAGE_CHANGED ||
        type == Slider::EventType::ON_SLIDEBALL_DOWN)
    {
        if(percent < maxPercent/4+3)
        {
            slider->setPercent(maxPercent/4+3);
        }
        else if(percent > maxPercent/4*3-3)
        {
            slider->setPercent(maxPercent/4*3-3);
        }
    }
    else if(type == Slider::EventType::ON_SLIDEBALL_UP ||
            type == Slider::EventType::ON_SLIDEBALL_CANCEL)
    {
        if(percent <= maxPercent/2)
        {
            slider->setPercent(maxPercent/4+3);
            if(name == "music")
            {
                GCache::setMusic(false);
                GTools::stopMusic();
            }
            else if(name == "sound")
            {
                GCache::setSound(false);
            }
			else if (name == "yaogan")
			{
				GCache::setHandStyle(false);
			}
        }
        else if(percent > maxPercent/2)
        {
            slider->setPercent(maxPercent/4*3-3);
            if(name == "music")
            {
                GCache::setMusic(true);
                GTools::playMusic(MUSIC_MIAN);
            }
            else if(name == "sound")
            {
                GCache::setSound(true);
            }
			else if (name == "yaogan")
			{
				GCache::setHandStyle(true);
			}
        }
    }
}

void GSetting::touchEvent(Ref *pSender, Widget::TouchEventType type)
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
