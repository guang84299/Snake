//
//  GBlockSprite.cpp
//  OnlineGame
//
//  Created by guang on 16/10/9.
//
//

#include "GBlockSprite.h"
#include "common/GCommon.h"
#include "controller/GGameController.h"
USING_NS_CC;

GBlockSprite::~GBlockSprite()
{
    this->block->release();
}

GBlockSprite* GBlockSprite::create(GBlock* block)
{
    GBlockSprite* sp = new GBlockSprite();
    if(sp && sp->init(block))
    {
        sp->autorelease();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return nullptr;
}

bool GBlockSprite::init(GBlock* block)
{
    block->retain();
    this->block = block;
    
    if(block->bulletType == 2)
    {
        this->initWithFile("crystal_2.png");
    }
    else if(block->bulletType == 3)
    {
        this->initWithFile("crystal_3.png");
    }
    else if(block->bulletType == 4)
    {
        this->initWithFile("crystal_4.png");
    }
    else if(block->bulletType == 5)
    {
        this->initWithFile("crystal_5.png");
    }
    else if(block->bulletType == 6)
    {
        this->initWithFile("crystal_6.png");
    }
    else if(block->bulletType == 7)
    {
        this->initWithFile("crystal_7.png");
    }
    else
    {
        
        
        if(block->type == 0)
        {
            this->initWithFile("crystal.png");
        }
        else if(block->type == 2)
        {
            this->initWithFile("crystal_hp.png");
        }
    }
//    char c[7];
//    sprintf(c, "%d",block->_id);
//    auto name = Label::createWithSystemFont(c, "", 18);
//    this->addChild(name);
    return true;
}

void GBlockSprite::init(cocos2d::Texture2D *texture)
{
    this->initWithTexture(texture);
//    auto body = PhysicsBody::createBox(this->getContentSize());
//    body->setDynamic(false);
//    body->setName("body");
//    body->setCategoryBitmask(GCommon::block_1);    // 0010
//    body->setContactTestBitmask(GCommon::block_2); // 1000
//    body->setCollisionBitmask(GCommon::block_3);   // 0001
//    this->addComponent(body);

}

void GBlockSprite::die(GBubbleSprite* bubble)
{
    block->state = GBlock::State::DIE;
    
    Action * stop = Sequence::create(
                                     EaseSineOut::create(MoveTo::create(0.1, bubble->getUpdatePosition())),
                                     CallFunc::create(CC_CALLBACK_0(GBlockSprite::dieEnd, this)),
                                     nullptr);
    this->runAction(stop);
}

void GBlockSprite::dieEnd()
{
    int _id = block->_id;
    this->removeFromParent();
    GGameController::getInstance()->deleteBlock(_id);
}

