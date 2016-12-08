//
//  GCloundSprite.cpp
//  OnlineGame
//
//  Created by guang on 16/10/17.
//
//

#include "GCloundSprite.h"
#include "common/GCommon.h"
USING_NS_CC;

GCloundSprite::~GCloundSprite()
{
    if(this->clound)
    this->clound->release();
}

GCloundSprite* GCloundSprite::create(GClound* clound)
{
    GCloundSprite* sp = new GCloundSprite();
    if(sp && sp->init(clound))
    {
        sp->autorelease();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return nullptr;
}

bool GCloundSprite::init(GClound* clound)
{
    clound->retain();
    this->clound = clound;
    
//    this->initWithFile("ball.png");
    
    this->setScale(clound->scale);
    this->setPosition(clound->x, clound->y);

    auto body = PhysicsBody::createCircle(this->getContentSize().width/2,PhysicsMaterial(0,0,0));
    body->setDynamic(false);
    body->setCategoryBitmask(GCommon::BALL_1);
    body->setContactTestBitmask(GCommon::BALL_2);
    body->setCollisionBitmask(GCommon::BALL_3);
    this->addComponent(body);
   
    
    return true;
}


GCloundSprite* GCloundSprite::create(Vec2* points,int size)
{
    GCloundSprite* sp = new GCloundSprite();
    if(sp && sp->init(points,size))
    {
        sp->autorelease();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return nullptr;
}

bool GCloundSprite::init(Vec2* points,int size)
{
    
    auto body = PhysicsBody::createEdgePolygon(points, size,PhysicsMaterial(0,0.1,1));
    body->setDynamic(false);
    body->setCategoryBitmask(GCommon::BALL_1);
    body->setContactTestBitmask(GCommon::BALL_2);
    body->setCollisionBitmask(GCommon::BALL_3);
    this->addComponent(body);
    
    
    return true;
}


GCloundSprite* GCloundSprite::create(int size)
{
    GCloundSprite* sp = new GCloundSprite();
    if(sp && sp->init(size))
    {
        sp->autorelease();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return nullptr;
}

bool GCloundSprite::init(int size)
{
    
    auto body = PhysicsBody::createBox(Size(size,size),PhysicsMaterial(0,0.1,1));
    body->setDynamic(false);
    body->setCategoryBitmask(GCommon::BALL_1);
    body->setContactTestBitmask(GCommon::BALL_2);
    body->setCollisionBitmask(GCommon::BALL_3);
    this->addComponent(body);
    
    return true;
}



