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
    
//    if(block->type == 0)
//    {
//        if(block->blockType == 1)
//        {
//            this->initWithFile("wujian-A1.png");
//        }
//        else if(block->blockType == 2)
//        {
//            this->initWithFile("wujian-A2.png");
//        }
//        else if(block->blockType == 3)
//        {
//            this->initWithFile("wujian-A3.png");
//        }
//        else if(block->blockType == 4)
//        {
//            this->initWithFile("wujian-A4.png");
//        }
//        this->setScale(0.5f);
//    }
//    char c[7];
//    sprintf(c, "%d",block->_id);
//    auto name = Label::createWithSystemFont(c, "", 18);
//    this->addChild(name);
    return true;
}

void GBlockSprite::init(cocos2d::Texture2D *texture)
{
    this->initWithTexture(texture);
    if(this->block->exp <= 1)
        this->setScale(0.5f);
    else
        this->setScale(0.7f);
//    float fs = this->getScale();
//    float ts = fs * 0.7f;
//    auto seq = Sequence::create(DelayTime::create(random(0, 10)), ScaleTo::create(1, ts),ScaleTo::create(1, fs), NULL);
//    auto seq2 = Sequence::create(DelayTime::create(random(0, 10)),FadeTo::create(1, 180),FadeTo::create(1, 255), NULL);
//    this->runAction(RepeatForever::create(Spawn::create(seq, NULL)));
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
    
    Action * stop = Sequence::create(EaseSineOut::create(MoveTo::create(0.1, bubble->getUpdatePosition())),
                                     CallFunc::create(CC_CALLBACK_0(GBlockSprite::dieEnd, this)),
                                     nullptr);
    this->runAction(stop);
    auto sc = ScaleTo::create(0.1, 0.1);
    this->runAction(sc);
}

void GBlockSprite::dieEnd()
{
    int _id = block->_id;
    float x = block->x;
    float y = block->y;
    this->removeFromParent();
    GGameController::getInstance()->deleteBlock(_id,x,y);
}

