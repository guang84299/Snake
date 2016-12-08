//
//  GNumber.cpp
//  PaoPao
//
//  Created by guang on 2016/11/28.
//
//

#include "GNumber.h"

USING_NS_CC;

GNumber* GNumber::create(int num)
{
    GNumber *sprite = new (std::nothrow) GNumber();
    if (sprite && sprite->init(num))
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool GNumber::init(int num)
{
    if(!Sprite::init())
    {
        return false;
    }
    this->_num = num;
    this->_color = Color3B::WHITE;
    this->_styleTime = false;
    setNum(num);
    
    return true;
}

void GNumber::setColor(cocos2d::Color3B color)
{
    this->_color = color;
    setNum(_num);
}

void GNumber::useRankStyle()
{
    if(_num <= 3)
    {
        this->removeAllChildren();
        
        char c[7];
        sprintf(c, "%d",_num);
        std::string s = c;
        
        float w = 0;
        float h = 0;
        std::string path = "jm-paiming";
        std::string file = path + s + ".png";
        
        auto sp = Sprite::create(file);
        sp->setAnchorPoint(Vec2(0,0));
        w = sp->getContentSize().width;
        h = sp->getContentSize().height;
        sp->setColor(_color);
        this->addChild(sp);
        
        this->setContentSize(Size(w*s.length(),h));
    }
}

void GNumber::useTimeStyle()
{
    this->_styleTime = true;
    
    this->removeAllChildren();
    int m = _num/60;
    int s = _num%60;
    
    char c[7];
    sprintf(c, "%02d",m);
    std::string sm = c;
    sprintf(c, "%02d",s);
    std::string ss = c;
    
    float w = 0;
    float h = 0;
    std::string path = "sz-1-";
    
    Node* node = Node::create();
    for (int i=0; i<sm.length(); i++) {
        std::string n = sm.substr(i,1);
        
        std::string file = path + n + ".png";
        auto sp = Sprite::create(file);
        sp->setAnchorPoint(Vec2(0,0));
        sp->setColor(_color);
        w = sp->getContentSize().width;
        h = sp->getContentSize().height;
        
        sp->setPosition(w*i, 0);
        node->addChild(sp);
    }
    for (int i=0; i<ss.length(); i++) {
        std::string n = ss.substr(i,1);
        
        std::string file = path + n + ".png";
        auto sp = Sprite::create(file);
        sp->setAnchorPoint(Vec2(0,0));
        sp->setColor(_color);
        w = sp->getContentSize().width;
        h = sp->getContentSize().height;
        
        sp->setPosition(w*(i+3), 0);
        node->addChild(sp);
    }
    node->setContentSize(Size(w*5,h));
    this->addChild(node);
    this->setContentSize(node->getContentSize());
}

void GNumber::setNum(int num)
{
    this->_num = num;
    if(this->_styleTime)
    {
        useTimeStyle();
        return;
    }
    this->removeAllChildren();
    
    char c[7];
    sprintf(c, "%d",num);
    std::string s = c;
    
    float w = 0;
    float h = 0;
    std::string path = "sz-1-";
    if(s.length() == 1)
    {
        std::string file = path + s + ".png";
        auto sp = Sprite::create(file);
        sp->setAnchorPoint(Vec2(0,0));
        w = sp->getContentSize().width;
        h = sp->getContentSize().height;
        sp->setColor(_color);
        this->addChild(sp);
    }
    else
    {
        Node* node = Node::create();
        for (int i=0; i<s.length(); i++) {
            std::string n = s.substr(i,1);
            
            std::string file = path + n + ".png";
            auto sp = Sprite::create(file);
            sp->setAnchorPoint(Vec2(0,0));
            sp->setColor(_color);
            w = sp->getContentSize().width;
            h = sp->getContentSize().height;

            sp->setPosition(w*i, 0);
            node->addChild(sp);
        }
        node->setContentSize(Size(w*s.length(),h));
        this->addChild(node);
    }
    
    this->setContentSize(Size(w*s.length(),h));
}
