//
//  GBodySprite.cpp
//  Snake
//
//  Created by guang on 2016/12/9.
//
//

#include "GBodySprite.h"
#include "sprite/GBubbleSprite.h"

USING_NS_CC;

GBodySprite* GBodySprite::create(GBubbleSprite* bubble,cocos2d::Sprite* parent,int skinId,int tag,float sc)
{
    GBodySprite* sp = new GBodySprite();
    if(sp && sp->init(bubble,parent,skinId,tag,sc))
    {
        sp->autorelease();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return nullptr;

}

bool GBodySprite::init(GBubbleSprite* bubble,cocos2d::Sprite* parent,int skinId,int tag,float sc)
{
    this->parent = parent;
    this->isTou = false;
    this->skinId = skinId;
    this->tag = tag;
    this->sc = sc;
    this->child = nullptr;
    this->bubble = dynamic_cast<GBubbleSprite*>(parent);
    
    char c[7];
    sprintf(c, "%d",skinId);
    std::string sId = c;
    
    std::string path = "juese";
    std::string path2 = path + sId + std::string("-2.png");
    std::string path3 = path + sId + std::string("-3.png");
    std::string path4 = path + sId + std::string("-4.png");
    std::string path5 = path + sId + std::string("-5.png");
    
    if(this->bubble)
    {
        this->isTou = true;
    }
    else
    {
        this->bubble = bubble;
        GBodySprite* b = dynamic_cast<GBodySprite*>(parent);
        if(b)
        {
            if(b->tag/5 % 2 == 1)
            {
                int t = b->tag % 5;

                b->color->setVisible(true);
                float op = 0.2f;
                if(t == 1)
                    op = 0.7;
                else if(t == 2)
                    op = 1.0;
                else if(t == 3)
                    op = 0.5;
                else if(t == 4)
                    op = 0.3;
                b->color->setOpacity(255*op);
            }
            b->initWithFile(path3);
            
            b->setChild(this);
        }
    }
    this->initWithFile(path5);
    
    color = Sprite::create(path4);
    color->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    color->setVisible(false);
    this->addChild(color);
    
    speed  = Sprite::create(path2);
    speed->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    speed->setOpacity(0);
    this->addChild(speed);
    
    this->setScale(sc);
//    this->setContentSize(this->getContentSize()*sc);
    this->op = 255 * ((20-(tag%20))/20.f);
    return true;
}

void GBodySprite::resetScale(float sc)
{
    this->sc = sc;
    this->setScale(sc);
}

void GBodySprite::changeEnd()
{
    this->initWithFile("juese-A3.png");
    child = nullptr;
}

void GBodySprite::showSpeed()
{
    this->op += 20;
    if(this->op > 255)
        this->op = 0;
    speed->setOpacity(this->op);
}

void GBodySprite::showSpeedEnd()
{
    this->op = 255 * ((10-(tag%10))/10.f);
    speed->setOpacity(0);
    speed->stopAllActions();
}

void GBodySprite::setChild(GBodySprite* child)
{
    this->child = child;
}
GBodySprite* GBodySprite::getChild()
{
    return this->child;
}
