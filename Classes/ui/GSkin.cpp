//
//  GSkin.cpp
//  PaoPao
//
//  Created by guang on 2016/11/21.
//
//

#include "GSkin.h"

#include "ui/GTouchLayer.h"
#include "ui/GNumber.h"
#include "data/GCache.h"
#include "protocol_model/GModeGame.h"
#include "protocol_model/GModeUser.h"

USING_NS_CC;

bool GSkin::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    bg = GTouchLayer::create();
    this->addChild(bg);
    initUI();
    this->setName("skin");
    return true;
}

void GSkin::initUI()
{
    Size s = Director::getInstance()->getWinSize();
    
    _data = GJsonArray::create(GCache::getInstance()->getSkins().c_str());
    _data->retain();
    
    auto skin_bg = Sprite::create("jm-diban1.png");
    skin_bg->setPosition(s.width/2, s.height*0.5f);
    bg->addChild(skin_bg,1);

	auto top_guang = Sprite::create("jm-diban4.png");
	top_guang->setAnchorPoint(Vec2(0.5, 0));
	top_guang->setPosition(skin_bg->getPositionX(), skin_bg->getPositionY() + skin_bg->getContentSize().height / 2 - 40);
	bg->addChild(top_guang, 0);

	auto bottom_guang = Sprite::create("jm-diban3.png");
	bottom_guang->setAnchorPoint(Vec2(0.5, 1));
	bottom_guang->setPosition(skin_bg->getPositionX(), skin_bg->getPositionY() - skin_bg->getContentSize().height / 2 + 34);
	bg->addChild(bottom_guang, 0);
    
    auto info_title = Sprite::create("jm-biaoti1.png");
    info_title->setAnchorPoint(Vec2(0.5,1));
    info_title->setPosition(skin_bg->getContentSize().width/2,skin_bg->getContentSize().height+20);
    //skin_bg->addChild(info_title);
    
    auto t_title = Sprite::create("jm-biaotiWZ6.png");
    t_title->setAnchorPoint(Vec2(0.5,1));
	t_title->setPosition(Vec2(skin_bg->getContentSize().width / 2, skin_bg->getContentSize().height - 30));
	skin_bg->addChild(t_title);
    
    
    s = skin_bg->getContentSize();
    
    Button* btn = Button::create("jm-guanbi1.png");
    btn->setAnchorPoint(Vec2(1,1));
    btn->setName("close");
    btn->setPosition(Vec2(s.width-40,s.height-35));
    btn->addTouchEventListener(CC_CALLBACK_2(GSkin::touchEvent, this));
    skin_bg->addChild(btn,2);
    
    auto setBg = Scale9Sprite::create("jm-tiao1.png");
    setBg->setContentSize(Size(670,40));
    setBg->setPosition(Vec2(s.width/2,s.height*0.86f));
    skin_bg->addChild(setBg);
    
    auto crystalBg = Scale9Sprite::create("jm-tiao6.png");
    crystalBg->setContentSize(Size(150,30));
    crystalBg->setAnchorPoint(Vec2(0,0.5));
    crystalBg->setPosition(Vec2(30,setBg->getContentSize().height/2 - 30));
    setBg->addChild(crystalBg);
    
    auto sp_crystal = Sprite::create("jm-zuanshi1.png");
    sp_crystal->setPosition(0, crystalBg->getContentSize().height/2);
    crystalBg->addChild(sp_crystal);
    
    t_crystal = GNumber::create(GCache::getInstance()->getUser()->crystal);
    t_crystal->setAnchorPoint(Vec2(0,0.5));
    t_crystal->setPosition(Vec2(30,crystalBg->getContentSize().height/2));
    crystalBg->addChild(t_crystal);
    
    auto skinBg = Scale9Sprite::create("jm-tiao6.png");
    skinBg->setContentSize(Size(150,30));
    skinBg->setAnchorPoint(Vec2(0,0.5));
    skinBg->setPosition(Vec2(crystalBg->getPositionX() + crystalBg->getContentSize().width + 50,setBg->getContentSize().height/2 - 30));
    setBg->addChild(skinBg);
    
    auto sp_skin = Sprite::create("jm-pifu1.png");
    sp_skin->setPosition(0, skinBg->getContentSize().height/2);
    skinBg->addChild(sp_skin);
    
    t_skin = GNumber::create(GCache::getInstance()->getUser()->skinNum);
    t_skin->setColor(Color3B::GREEN);
    t_skin->setAnchorPoint(Vec2(0,0.5));
    t_skin->setPosition(Vec2(30,skinBg->getContentSize().height/2));
    skinBg->addChild(t_skin);
    
    auto sp_skin2 = Sprite::create("jm-pifu2.png");
    sp_skin2->setPosition(t_skin->getContentSize().width+t_skin->getPositionX()+10, skinBg->getContentSize().height/2);
    skinBg->addChild(sp_skin2);
    
    auto t_skin2 = GNumber::create(_data->size());
    t_skin2->setAnchorPoint(Vec2(0,0.5));
    t_skin2->setPosition(Vec2(sp_skin2->getContentSize().width+sp_skin2->getPositionX(),skinBg->getContentSize().height/2));
    skinBg->addChild(t_skin2);
    
    
    _listView = ListView::create();
    _listView->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    _listView->setContentSize(Size(630, 500));
    _listView->setPosition(Vec2(33,43));
    skin_bg->addChild(_listView);
    
    for(int i=0;i<_data->size();i++)
    {
        _listView->pushBackCustomItem(createItem(i));
    }
    _listView->jumpToTop();
}

void GSkin::updateUI()
{
    t_crystal->setNum(GCache::getInstance()->getUser()->crystal);
    t_skin->setNum(GCache::getInstance()->getUser()->skinNum);
    
    _listView->removeAllItems();
    for(int i=0;i<_data->size();i++)
    {
        _listView->pushBackCustomItem(createItem(i));
    }
    _listView->jumpToTop();
}

Layout* GSkin::createItem(int index)
{
    GJsonObject* obj = _data->at(index);
    
    int id = obj->getInt("id");
    int price = obj->getInt("price");
    std::vector<std::string> ids = GTools::subString(GCache::getInstance()->getUser()->skins, ",");
    bool isBuy = false;
    
    for(std::string s : ids)
    {
        if(std::atoi(s.c_str()) == id)
        {
            isBuy = true;
            break;
        }
    }
    
    float w = _listView->getContentSize().width/4;
    float h = _listView->getContentSize().height;
    Layout* item = Layout::create();
    //    item->setTouchEnabled(true);
    item->setContentSize(Size(w,h));
    item->setPosition(Vec2(0,0));
    
    //auto bg = Scale9Sprite::create("jm-tiao7.png");
	auto bg = Scale9Sprite::create("jm-tiao4.png");
    bg->setContentSize(Size(150,286));
    bg->setAnchorPoint(Vec2(0.5,0));
    bg->setPosition(Vec2(w/2,80));
    item->addChild(bg);
    
    char c[7];
    sprintf(c, "%d",id);
//    std::string name = "jm-pifu3-";//obj->getString("name");
//    name = name + c + ".png";
//    auto t_name = Sprite::create(name);
//    t_name->setAnchorPoint(Vec2(0.5,1));
//    t_name->setPosition(Vec2(bg->getContentSize().width/2,bg->getContentSize().height+4));
//    bg->addChild(t_name);

    std::string skinId = c;
    std::string path = "juese";
    std::string path1 = path + skinId + std::string("-1.png");
    std::string path2 = path + skinId + std::string("-2.png");
    std::string path3 = path + skinId + std::string("-3.png");
    std::string path4 = path + skinId + std::string("-4.png");
    std::string path5 = path + skinId + std::string("-5.png");
    
    auto skeleton = Sprite::createWithSpriteFrameName(path1);
    skeleton->setPosition(bg->getContentSize().width/2,bg->getContentSize().height*0.72f);
    skeleton->setScale(1);
    bg->addChild(skeleton,5);
    
    skeleton = Sprite::createWithSpriteFrameName(path4);
    skeleton->setPosition(bg->getContentSize().width/2,bg->getContentSize().height*0.72f-30);
    skeleton->setScale(1);
    bg->addChild(skeleton,4);
    
    skeleton = Sprite::createWithSpriteFrameName(path4);
    skeleton->setPosition(bg->getContentSize().width/2,bg->getContentSize().height*0.72f-30*2);
    skeleton->setScale(1);
    bg->addChild(skeleton,3);
    
    skeleton = Sprite::createWithSpriteFrameName(path4);
    skeleton->setPosition(bg->getContentSize().width/2,bg->getContentSize().height*0.72f-30*3);
    skeleton->setScale(1);
    bg->addChild(skeleton,2);
    
    skeleton = Sprite::createWithSpriteFrameName(path5);
    skeleton->setPosition(bg->getContentSize().width/2,bg->getContentSize().height*0.72f-30*4);
    skeleton->setScale(1);
    bg->addChild(skeleton,1);
    
    
    //auto sel = Sprite::create("jm-xuanzhong2.png");
	auto sel = Sprite::create("jm-pifu4.png");
    sel->setAnchorPoint(Vec2(1,0));
    sel->setPosition(bg->getContentSize().width-5,-1);
    bg->addChild(sel);
    sel->setVisible(false);

    Button* btn = Button::create("an-annniu4-1.png","an-annniu4-2.png");
    btn->setScale9Enabled(true);
    btn->setContentSize(Size(150,60));
    btn->setName("buy");
    btn->setTag(index);
    btn->setPosition(Vec2(w/2,40));
    btn->addTouchEventListener(CC_CALLBACK_2(GSkin::touchEvent, this));
    item->addChild(btn);
    
    auto t_sp = Sprite::create("jm-zuanshi1.png");
    t_sp->setAnchorPoint(Vec2(1,0.5));
    t_sp->setPosition(btn->getContentSize().width/2 - 5, btn->getContentSize().height/2);
    btn->addChild(t_sp);
	
    auto n_price = GNumber::create(price);
    n_price->setAnchorPoint(Vec2(0,0.5));
    n_price->setPosition(btn->getContentSize().width/2 + 5, btn->getContentSize().height/2);
    btn->addChild(n_price);
    
    if(isBuy)
    {
        btn->removeChild(n_price);
        t_sp->setAnchorPoint(Vec2(0.5,0.5));
        t_sp->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2 + 2);
        
        btn->setName("use");
        
        bool isUse = false;
        if(GCache::getInstance()->getUser()->skinId == id)
            isUse = true;
        if(isUse)
        {
            btn->setTouchEnabled(false);
            btn->loadTextures("an-annniu1-1.png","an-annniu1-2.png");
            t_sp->initWithFile("an-WZ13.png");
            sel->setVisible(true);
        }
        else
        {
            btn->loadTextures("an-annniu5-1.png","an-annniu5-2.png");
            t_sp->initWithFile("an-WZ12.png");
        }
       
    }
	else
	{
		t_sp->setScale(0.65);
		n_price->setScale(0.65);
	}
    
    return item;
}

void GSkin::touchEvent(Ref *pSender, Widget::TouchEventType type)
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
                _data->release();
                this->runAction(Sequence::create(DelayTime::create(0.1f),RemoveSelf::create(), NULL));
            }
            else if(name == "buy")
            {
                GJsonObject* obj = _data->at(btn->getTag());
                if(obj)
                {
                    int price = obj->getInt("price");
                    if(price > GCache::getInstance()->getUser()->crystal)
                    {
                        GTools::showTost(this, _T("crystal not enough"));
                    }
                    else
                    {
                        GModeUser::buySkin(obj->getInt("id"));
                    }
                }
            }
            else if(name == "use")
            {
                GJsonObject* obj = _data->at(btn->getTag());
                if(obj)
                {
                    GModeUser::useSkin(obj->getInt("id"));
                }
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
    
}
