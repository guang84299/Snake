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

GBodySprite* GBodySprite::create()
{
    GBodySprite* sp = new GBodySprite();
    if(sp)
    {
        sp->autorelease();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return nullptr;

}

void GBodySprite::init(int skinId,int tag,float sc,bool isEnd)
{
    this->isTou = false;
    this->isEnd = isEnd;
    this->skinId = skinId;
    this->tag = tag;
    this->sc = sc;
    
    char c[7];
    sprintf(c, "%d",skinId);
    std::string sId = c;
    
    std::string path = "juese";
    std::string path2 = path + sId + std::string("-2.png");
//    std::string path3 = path + sId + std::string("-3.png");
    std::string path4 = path + sId + std::string("-4.png");
    std::string path5 = path + sId + std::string("-5.png");
    
    
    if(isEnd)
        this->initWithSpriteFrameName(path5);

    else
        this->initWithSpriteFrameName(path4);

//    color = Sprite::createWithSpriteFrameName(path4);
//    color->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
//    this->addChild(color);

    speed  = Sprite::createWithSpriteFrameName(path2);
    speed->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    speed->setOpacity(0);
    this->addChild(speed);
    
    this->setScale(sc);
    
//    if(tag/10 % 3 == 0)
//    {
//        this->op2 = 255 * (tag%10)/10.f;
//    }
//    else if(tag/10 % 3 == 1)
//    {
//        this->op2 = 255 * (10-(tag%10))/10.f;
//    }
//    else
//    {
//        this->op2 = 0;
//        isShowSpeed = false;
//    }
    
    this->op = this->op2;
//    this->op = 255 * (tag%10)/10.f;
    
}

void GBodySprite::resetScale(float sc)
{
    this->sc = sc;
    this->setScale(sc);
}


void GBodySprite::showSpeed()
{
//    this->op -= 10;
//    if(this->op > 255)
//        this->op = 0;
//    speed->setOpacity(this->op);
    speed->setOpacity(0);
    speed->runAction(Sequence::create(FadeIn::create(0.15f),
                                      FadeOut::create(0.15f),
                                      NULL));
}

void GBodySprite::showSpeedEnd()
{
//    this->op = this->op2;
    speed->setOpacity(0);
    speed->stopAllActions();
}


const Size& GBodySprite::getCollSize()
{
    return  this->getContentSize()*sc;
}
