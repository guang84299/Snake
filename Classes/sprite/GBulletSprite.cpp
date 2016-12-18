//
//  GBulletSprite.cpp
//  OnlineGame
//
//  Created by guang on 16/10/12.
//
//

#include "GBulletSprite.h"
#include "GBubbleSprite.h"
#include "data/GCache.h"
#include "common/GCommon.h"
#include "controller/GGameController.h"
#include "protocol_model/GModeGame.h"
#include "tools/GTools.h"
USING_NS_CC;

#define ACTION_MOVE 1
#define ACTION_ROTATE 2

GBulletSprite::~GBulletSprite()
{
    this->bullet->release();
    if(bullet_5)
        bullet_5->release();
}

GBulletSprite* GBulletSprite::create(GBullet* bullet,cocos2d::Vec2 &dir)
{
    GBulletSprite* sp = new GBulletSprite();
    if(sp && sp->init(bullet,dir))
    {
        sp->autorelease();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return nullptr;
}

bool GBulletSprite::init(GBullet* bullet,cocos2d::Vec2 &dir)
{
    bullet->retain();
    this->bullet = bullet;
    this->dir = dir;
    this->lastDir = dir;
    this->isSelf = (bullet->uid == GCache::getInstance()->getUser()->uid);
    this->runTime = bullet->bulletTime;
    this->bullet->dirX = dir.x;
    this->bullet->dirY = dir.y;
    if(bullet->_id == 1)
    {
        createBullet_1();
    }
    else if(bullet->_id == 2)
    {
        int rotate = random(0, bullet->fireAngle);
        if(random(0,1) == 0)
            rotate = -rotate;
        float angle = 3.14f/180*rotate;
        dir.rotate(Vec2::ZERO, angle);
        this->dir = dir;
        this->lastDir = dir;
        this->bullet->dirX = dir.x;
        this->bullet->dirY = dir.y;
        
        createBullet_2();
    }
    else if(bullet->_id == 3)
    {
        int rotate = random(0, bullet->fireAngle);
        if(random(0,1) == 0)
            rotate = -rotate;
        float angle = 3.14f/180*rotate;
        dir.rotate(Vec2::ZERO, angle);
        this->dir = dir;
        this->lastDir = dir;
        this->bullet->dirX = dir.x;
        this->bullet->dirY = dir.y;
        this->currSpeed = bullet->bulletAccelerated;
        createBullet_3();
        _body->setRotation(rotate);
    }
    else if(bullet->_id == 4)
    {
        createBullet_4();
    }
    else if(bullet->_id == 5)
    {
        createBullet_5();
    }
    else if(bullet->_id == 6)
    {
        createBullet_6();
    }
    else if(bullet->_id == 7)
    {
        createBullet_7();
        this->setTag(GCommon::NO_COLL);
    }
    else if(bullet->_id == 8)
    {
        createBullet_8();
        this->setTag(GCommon::NO_COLL);
    }
    
    this->setVisible(false);

    return true;
}

void GBulletSprite::createBullet_1()
{
    auto sp = Sprite::create("zidan-putong1B.png");
    this->addChild(sp);
    _body = sp;
    
    this->setContentSize(_body->getContentSize());
    
//    if(GGameController::getInstance()->isSelfBullet(bullet))
//    {
//        auto body = PhysicsBody::createBox(_body->getContentSize());
//        body->setDynamic(false);
//        body->setCategoryBitmask(GCommon::BULLET_1);
//        body->setContactTestBitmask(GCommon::BULLET_2);
//        body->setCollisionBitmask(GCommon::BULLET_3);
//        _body->addComponent(body);
//    }
//    else
//    {
//        this->setTag(GCommon::NO_COLL);
//    }
}

void GBulletSprite::createBullet_2()
{
    auto sp = Sprite::create("zidan-lianfa1A1.png");
    this->addChild(sp);
    
    _body = sp;
    this->setContentSize(_body->getContentSize());
    
//    if(GGameController::getInstance()->isSelfBullet(bullet))
//    {
//        auto body = PhysicsBody::createBox(_body->getContentSize());
//        body->setDynamic(false);
//        body->setCategoryBitmask(GCommon::BULLET_1);
//        body->setContactTestBitmask(GCommon::BULLET_2);
//        body->setCollisionBitmask(GCommon::BULLET_3);
//        _body->addComponent(body);
//    }
//    else
//    {
//        this->setTag(GCommon::NO_COLL);
//    }
}

void GBulletSprite::createBullet_3()
{
    auto sp = Sprite::create("zidan-gengzhongA1.png");
    this->addChild(sp);
    
    _body = sp;
    
    auto particle = ParticleSystemQuad::create("zidan-gengzhongAlizhi.plist");
    particle->setScale(1.0f);
    particle->setPosition(sp->getContentSize().width/2,sp->getContentSize().height/2);
    _body->addChild(particle);
    
    this->setContentSize(_body->getContentSize());
    
//    if(GGameController::getInstance()->isSelfBullet(bullet))
//    {
//        auto body = PhysicsBody::createBox(_body->getContentSize());
//        body->setDynamic(false);
//        body->setCategoryBitmask(GCommon::BULLET_1);
//        body->setContactTestBitmask(GCommon::BULLET_2);
//        body->setCollisionBitmask(GCommon::BULLET_3);
//        _body->addComponent(body);
//    }
//    else
//    {
//        this->setTag(GCommon::NO_COLL);
//    }
}

void GBulletSprite::createBullet_4()
{
    bubble_4 = nullptr;
    if(bullet->uid == GCache::getInstance()->getUser()->uid)
    {
        bubble_4 = GGameController::getInstance()->bubble;
    }
    else
    {
        bubble_4 = GGameController::getInstance()->findByUid(bullet->uid);
        if(!bubble_4)
        {
            bubble_4 = GGameController::getInstance()->findRobotByUid(bullet->uid);
        }
    }
    
    if(bubble_4)
    {
//        auto sp = Sprite::create();
//        sp->setContentSize(bubble_4->getContentSize());
//        this->addChild(sp);
        
        this->setContentSize(bubble_4->getContentSize());
        
        auto sp = Sprite::create("zidan-zhixianA_01.png");
        float sy = bullet->range / sp->getContentSize().height;
        sp->setScale(sy);
        sp->setAnchorPoint(Vec2(0.5,0));
        sp->setPosition(0, 0);
        this->addChild(sp);
        
        _body = sp;
//        sp->setBlendFunc({GL_ONE, GL_ONE});
        
        if(GGameController::getInstance()->isSelfBullet(bullet))
        {
//            auto body = PhysicsBody::createBox(_body->getContentSize(),PhysicsMaterial(0.1f,0.1f,0.1f));
//            body->setDynamic(false);
//            body->setCategoryBitmask(GCommon::BULLET_1);
//            body->setContactTestBitmask(GCommon::BULLET_2);
//            body->setCollisionBitmask(GCommon::BULLET_3);
//            _body->addComponent(body);
            float w = _body->getContentSize().width / 3;
            int num = _body->getContentSize().height / w;
            for(int i=0;i<num;i++)
            {
                auto coll = Sprite::create("ui-ditu5.png");
                coll->setVisible(false);
                coll->setScale(w/coll->getContentSize().width);
                coll->setPosition(_body->getContentSize().width/2, w*(i+1) - w/2);
                _body->addChild(coll,2);
                
                collSp.push_back(coll);
            }
        }
//        else
//        {
            this->setTag(GCommon::NO_COLL);
//        }
        
    }
}

void GBulletSprite::createBullet_5()
{
    bubble_5 = nullptr;
    if(bullet->uid == GCache::getInstance()->getUser()->uid)
    {
        bubble_5 = GGameController::getInstance()->bubble;
    }
    else
    {
        bubble_5 = GGameController::getInstance()->findByUid(bullet->uid);
        if(!bubble_5)
        {
            bubble_5 = GGameController::getInstance()->findRobotByUid(bullet->uid);
        }
    }
    GBullet* b = GCache::getInstance()->getBullet(bullet->twoBulletId);
    bullet_5 = GBullet::create(b);
    bullet_5->retain();
    
    auto sp = Sprite::create("zidan-fenlieA.png");
    this->addChild(sp);
    
    _body = sp;
    this->setContentSize(_body->getContentSize());
    
//    if(GGameController::getInstance()->isSelfBullet(bullet))
//    {
//        auto body = PhysicsBody::createBox(_body->getContentSize());
//        body->setDynamic(false);
//        body->setCategoryBitmask(GCommon::BULLET_1);
//        body->setContactTestBitmask(GCommon::BULLET_2);
//        body->setCollisionBitmask(GCommon::BULLET_3);
//        _body->addComponent(body);
//    }
//    else
//    {
//        this->setTag(GCommon::NO_COLL);
//    }
}

void GBulletSprite::createBullet_6()
{
    auto sp = Sprite::create("zidan-fenlieA.png");
    sp->setScale(0.6f);
    this->addChild(sp);
    
    _body = sp;
    this->setContentSize(_body->getContentSize());
    
//    if(GGameController::getInstance()->isSelfBullet(bullet))
//    {
//        auto body = PhysicsBody::createBox(_body->getContentSize());
//        body->setDynamic(false);
//        body->setCategoryBitmask(GCommon::BULLET_1);
//        body->setContactTestBitmask(GCommon::BULLET_2);
//        body->setCollisionBitmask(GCommon::BULLET_3);
//        _body->addComponent(body);
//    }
//    else
//    {
//        this->setTag(GCommon::NO_COLL);
//    }
}

void GBulletSprite::createBullet_7()
{
    bubble_7 = nullptr;
    if(bullet->uid == GCache::getInstance()->getUser()->uid)
    {
        bubble_7 = GGameController::getInstance()->bubble;
    }
    else
    {
        bubble_7 = GGameController::getInstance()->findByUid(bullet->uid);
        if(!bubble_7)
        {
            bubble_7 = GGameController::getInstance()->findRobotByUid(bullet->uid);
        }
    }
    
    
    auto sp = Sprite::create("zidan-fusheA1.png");
    sp->setOpacity(0);
    this->addChild(sp);
    
    auto particle = ParticleSystemQuad::create("zidan-fusheAlizhi.plist");
    particle->setScale(1.0f);
//    particle->setPosition(sp->getContentSize().width/2,sp->getContentSize().height/2);
    this->addChild(particle);
    
    _body = sp;
    this->setContentSize(_body->getContentSize());
}

void GBulletSprite::createBullet_8()
{
    bubble_8 = nullptr;
    if(bullet->uid == GCache::getInstance()->getUser()->uid)
    {
        bubble_8 = GGameController::getInstance()->bubble;
    }
    else
    {
        bubble_8 = GGameController::getInstance()->findByUid(bullet->uid);
        if(!bubble_8)
        {
            bubble_8 = GGameController::getInstance()->findRobotByUid(bullet->uid);
        }
    }
    
    if(bubble_8)
    {
//        auto sp = Sprite::create("bullet/42.png");
//        sp->setAnchorPoint(Vec2(0.5,0));
//        this->addChild(sp);
        
        auto particle = ParticleSystemQuad::create("zidan-penshe1.plist");
        particle->setScale(1.0f);
        particle->setAnchorPoint(Vec2(0.5,0));
        particle->setPositionType(ParticleSystem::PositionType::GROUPED);
//        particle->setPosition(sp->getContentSize().width/2,sp->getContentSize().height/2);
        this->addChild(particle);
        
        _body = particle;
        this->setContentSize(_body->getContentSize());
    }
   
}


void GBulletSprite::setUpdateRotation(float rotation)
{
    if(bullet->_id == 3)
    {
        _body->setRotation(_body->getRotation() + rotation);
    }
    else
    {
        _body->setRotation(rotation);
    }
    
}

float GBulletSprite::getUpdateRotation()
{
    return _body->getRotation();
}

void GBulletSprite::run()
{
    this->setVisible(true);

    if(bullet->_id == 1)
    {
        runBullet_1();
    }
    else if(bullet->_id == 2)
    {
        runBullet_2();
    }
    else if(bullet->_id == 3)
    {
        runBullet_3();
        this->schedule(SEL_SCHEDULE(&GBulletSprite::updateBullet_3), bullet->changeTargetTime);
    }
    else if(bullet->_id == 4)
    {
        runBullet_4();
        this->schedule(SEL_SCHEDULE(&GBulletSprite::updateBullet_4));
    }
    else if(bullet->_id == 5)
    {
        runBullet_5();
        this->schedule(SEL_SCHEDULE(&GBulletSprite::updateBullet_5), bullet->splitCD);
    }
    else if(bullet->_id == 6)
    {
        runBullet_6();
    }
    else if(bullet->_id == 7)
    {
        runBullet_7();
        this->schedule(SEL_SCHEDULE(&GBulletSprite::updateBullet_7));
    }
    else if(bullet->_id == 8)
    {
        runBullet_8();
        this->schedule(SEL_SCHEDULE(&GBulletSprite::updateBullet_8));
    }
}


void GBulletSprite::runBullet_1()
{
    Vec2 by = dir*bullet->bulletSpeed*bullet->bulletTime;
    
    auto seq = Sequence::create(EaseSineOut::create(MoveBy::create(bullet->bulletTime, by)),
                                CallFunc::create(CC_CALLBACK_0(GBulletSprite::die, this)),
                                nullptr);
    this->runAction(seq);
    _body->setScaleX(0.2);
    _body->runAction(ScaleTo::create(bullet->bulletTime, 1));
    
    float angle = -180 / M_PI * dir.getAngle() + 90;
    angle = angle < 0 ? (360 + angle) : angle;
    angle = angle > 180 ? angle-360 : angle;
    
    int r = random(1, 2);
    char c[7];
    sprintf(c, "%d",r);
    std::string path = "bullet_001_a";
    path = path + c + std::string(".png");
    auto sp = Sprite::create(path);
    sp->setTag(GCommon::NO_COLL);
    sp->setAnchorPoint(Vec2(0.5,0));
    sp->setPosition(this->getPosition());
    sp->setRotation(angle);
    this->getParent()->getParent()->getChildByName("hideLayer")->addChild(sp);
    
    seq = Sequence::create(FadeOut::create(2),
                           RemoveSelf::create(),
                           nullptr);
    sp->runAction(seq);
}

void GBulletSprite::runBullet_2()
{
//    lastDir = dir;
//    Vec2 by = dir*bullet->bulletSpeed*bullet->changeTargetTime;
//    
//    auto seq = Sequence::create(MoveBy::create(bullet->changeTargetTime, by),
//                                CallFunc::create(CC_CALLBACK_0(GBulletSprite::runBullet_2_resetDir, this)),
//                                nullptr);
//    this->runAction(seq);
    
    
    Vec2 by = dir*bullet->bulletSpeed*bullet->bulletTime;
    
    auto seq = Sequence::create(EaseSineOut::create(MoveBy::create(bullet->bulletTime, by)),
                                CallFunc::create(CC_CALLBACK_0(GBulletSprite::die, this)),
                                nullptr);
    this->runAction(seq);
    
//    float angle = -180 / M_PI * dir.getAngle() + 90;
//    angle = angle < 0 ? (360 + angle) : angle;
//    angle = angle > 180 ? angle-360 : angle;
    
//    auto sp = Sprite::create();
//    sp->setTag(GCommon::NO_COLL);
//    sp->setAnchorPoint(Vec2(0.5,0));
//    sp->setPosition(this->getPosition());
//    sp->setRotation(angle);
//    this->getParent()->getParent()->getChildByName("bubbleLayer")->addChild(sp);
//    
//    seq = Sequence::create(GTools::createAnimate("bullet002_fire", 6, 0.06f),
//                           RemoveSelf::create(),
//                           nullptr);
//    sp->runAction(seq);
}

void GBulletSprite::runBullet_2_resetDir()
{
    findTarget();
    
    float angle = -180 / M_PI * dir.getAngle() + 90;
    angle = angle < 0 ? (360 + angle) : angle;
    angle = angle > 180 ? angle-360 : angle;
    
    float currAngle = -180 / M_PI * lastDir.getAngle() + 90;
    currAngle = currAngle < 0 ? (360 + currAngle) : currAngle;
    currAngle = currAngle > 180 ? currAngle-360 : currAngle;
    
    float d1 = angle < 0 ? (360 + angle) : angle;;
    float d2 = currAngle < 0 ? (360 + currAngle) : currAngle;;
    float dis = fabs(d1-d2);
    dis = MIN(dis, 360 - MAX(d1,d2) + MIN(d1, d2));
    
    if(dis > bullet->targetRange )
    {
        dir = lastDir;
    }


    Vec2 by = dir*bullet->bulletSpeed*bullet->bulletTime;
    
    auto seq = Sequence::create(EaseSineOut::create(MoveBy::create(bullet->bulletTime, by)),
                                CallFunc::create(CC_CALLBACK_0(GBulletSprite::die, this)),
                                nullptr);
    this->runAction(seq);
}

void GBulletSprite::runBullet_3()
{
    this->currSpeed += bullet->bulletAccelerated;
    if(this->currSpeed > bullet->bulletSpeed)
        this->currSpeed = bullet->bulletSpeed;
    
    _body->stopActionByTag(ACTION_ROTATE);
    this->stopAllActions();
    
    //先旋转角度对准目标
    float angle = -180 / M_PI * dir.getAngle() + 90;
    angle = angle < 0 ? (360 + angle) : angle;
    angle = angle > 180 ? angle-360 : angle;
    
    float currAngle = -180 / M_PI * lastDir.getAngle() + 90;
    currAngle = currAngle < 0 ? (360 + currAngle) : currAngle;
    currAngle = currAngle > 180 ? currAngle-360 : currAngle;
    
    float d1 = angle < 0 ? (360 + angle) : angle;;
    float d2 = currAngle < 0 ? (360 + currAngle) : currAngle;;
    float dis = fabs(d1-d2);
    dis = MIN(dis, 360 - MAX(d1,d2) + MIN(d1, d2));
    
    if(dis > bullet->targetRange)
    {
        angle = _body->getRotation();
        angle = angle > 180 ? angle - 360 : angle;
        angle = angle < -180 ? angle + 360 : angle;
        
        currAngle = (angle-90)/(-180 / M_PI);
        Vec2 dir = Vec2::forAngle(currAngle);
        
        Vec2 by = dir*this->currSpeed*bullet->bulletTime;
        
        Vec2 v1 = dir;
        Vec2 v2 = dir;
        v1.rotate(Vec2::ZERO, 3.14f/4);
        v2.rotate(Vec2::ZERO, -3.14f/4);
        
        this->runAction(MoveBy::create(bullet->bulletTime, by));
        return;
    }
    
    float time = dis / bullet->rotateSpeed;
    
    RotateTo *actionRo = RotateTo::create(time, angle);
    actionRo->setTag(ACTION_ROTATE);
   
    angle = _body->getRotation();
    angle = angle > 180 ? angle - 360 : angle;
    angle = angle < -180 ? angle + 360 : angle;
    
    currAngle = (angle-90)/(-180 / M_PI);
    dir = Vec2::forAngle(currAngle);

    Vec2 by = dir*this->currSpeed*bullet->bulletTime;
    
    Vec2 v1 = dir;
    Vec2 v2 = dir;
    v1.rotate(Vec2::ZERO, 3.14f/4);
    v2.rotate(Vec2::ZERO, -3.14f/4);
    
    _body->runAction(actionRo);
    this->runAction(Spawn::create(MoveBy::create(bullet->bulletTime, by),
                                  Sequence::create(MoveBy::create(0.2f, v1*25),
                                                   MoveBy::create(0.2f, v2*25),NULL), NULL));
    lastDir = dir;
}

void GBulletSprite::runBullet_4()
{
    _body->runAction(RepeatForever::create(GTools::createAnimate("zidan-zhixianA", 4, 0.1f)));
    
    auto seq = Sequence::create(DelayTime::create(bullet->bulletTime),
                                CallFunc::create(CC_CALLBACK_0(GBulletSprite::die, this)),
                                nullptr);
    this->runAction(seq);
}


void GBulletSprite::runBullet_5()
{
    Vec2 by = dir*bullet->bulletSpeed*bullet->bulletTime;
    
    auto seq = Sequence::create(EaseSineOut::create(MoveBy::create(bullet->bulletTime, by)),
                                CallFunc::create(CC_CALLBACK_0(GBulletSprite::die, this)),
                                nullptr);
    this->runAction(seq);
    
    _body->runAction(RepeatForever::create(RotateBy::create(1, bullet->rotateSpeed)));
}

void GBulletSprite::runBullet_6()
{
    Vec2 by = dir*bullet->bulletSpeed*bullet->bulletTime;
    
    auto seq = Sequence::create(EaseSineOut::create(MoveBy::create(bullet->bulletTime, by)),
                                CallFunc::create(CC_CALLBACK_0(GBulletSprite::die, this)),
                                nullptr);
    this->runAction(seq);
    
    _body->runAction(RepeatForever::create(RotateBy::create(1, bullet->rotateSpeed)));
//    float angle = -180 / M_PI * dir.getAngle() + 90;
//    angle = angle < 0 ? (360 + angle) : angle;
//    angle = angle > 180 ? angle-360 : angle;
//    
//    auto sp = Sprite::create("bullet_001.png");
//    sp->setTag(GCommon::NO_COLL);
//    sp->setAnchorPoint(Vec2(0.5,0));
//    sp->setPosition(this->getPosition());
//    sp->setRotation(angle);
//    this->getParent()->addChild(sp);
//    
//    seq = Sequence::create( TintTo::create(0, random(100, 255), random(100, 255), random(100, 255)),
//                           FadeOut::create(15),
//                           RemoveSelf::create(),
//                           nullptr);
//    sp->runAction(seq);
}

void GBulletSprite::runBullet_7()
{
    float scale = bullet->range / _body->getContentSize().width;
    
    auto seq = Sequence::create(EaseSineOut::create(ScaleTo::create(bullet->bulletTime, scale)),
                                CallFunc::create(CC_CALLBACK_0(GBulletSprite::die, this)),
                                nullptr);
    _body->runAction(seq);
}

void GBulletSprite::runBullet_8()
{
    auto seq = Sequence::create(DelayTime::create(bullet->bulletTime),
                                CallFunc::create(CC_CALLBACK_0(GBulletSprite::die, this)),
                                nullptr);
    this->runAction(seq);
}

void GBulletSprite::updateBullet_3(float dt)
{
    runTime-=dt;
    if(runTime > 0)
    {
        findTarget();
        runBullet_3();
    }
    else
    {
        die();
    }
}

void GBulletSprite::updateBullet_4(float dt)
{
    if(bubble_4 && bubble_4->bubble->state != GBubble::State::DIE)
    {
        _body->setRotation(bubble_4->getUpdateRotation());
        this->setPosition(bubble_4->getBulletPosition(bullet->type));
    }
    else
    {
        die();
    }
}

void GBulletSprite::updateBullet_5(float dt)
{
    if(bubble_5 && GGameController::getInstance()->isSelfBullet(bullet))
    {
        bullet_5->tid = bullet->tid;
        bullet_5->dirX = dir.x;
        bullet_5->dirY = dir.y;
        GModeGame::attack(bubble_5->bubble,bullet_5);
    }
}

void GBulletSprite::updateBullet_7(float dt)
{
    if(bullet->state != GBullet::State::DIE && bubble_7 && bubble_7->bubble->state != GBubble::State::DIE)
    {
        _body->setRotation(bubble_7->getUpdateRotation());
        this->setPosition(bubble_7->getBulletPosition(bullet->type));
    }
    else
    {
        die();
    }
}

void GBulletSprite::updateBullet_8(float dt)
{
    if(bullet->state != GBullet::State::DIE && bubble_8 && bubble_8->bubble->state != GBubble::State::DIE)
    {
        _body->setRotation(bubble_8->getUpdateRotation());
        this->setPosition(bubble_8->getBulletPosition(bullet->type));
    }
    else
    {
        die();
    }
}


void GBulletSprite::die()
{
    bullet->state = GBullet::State::DIE;
    float time = 0.2f;
    //播放子弹炸粒子
    if(bullet->_id == 1)
    {
        float angle = -180 / M_PI * dir.getAngle() + 90;
        angle = angle < 0 ? (360 + angle) : angle;
        angle = angle > 180 ? angle-360 : angle;
        
        int r = random(1, 2);
        char c[7];
        sprintf(c, "%d",r);
        std::string path = "bullet_001_b";
        path = path + c + std::string(".png");
        
        auto sp = Sprite::create(path);
        sp->setTag(GCommon::NO_COLL);
        sp->setAnchorPoint(Vec2(0.5,0));
        sp->setPosition(this->getPosition());
        sp->setRotation(angle);
        this->getParent()->getParent()->getChildByName("hideLayer")->addChild(sp);
        
        auto seq = Sequence::create(FadeOut::create(15),
                               RemoveSelf::create(),
                               nullptr);
        sp->runAction(seq);
        
        //受击
        angle = -180 / M_PI * -dir.getAngle() + 90;
        angle = angle < 0 ? (360 + angle) : angle;
        angle = angle > 180 ? angle-360 : angle;
        
        sp = Sprite::create();
        sp->setTag(GCommon::NO_COLL);
        sp->setAnchorPoint(Vec2(0.5,0));
        sp->setPosition(this->getPosition());
        sp->setRotation(-angle);
        this->getParent()->addChild(sp);
        
        seq = Sequence::create(GTools::createAnimate("bullet001", 6, 0.08f),
                                    RemoveSelf::create(),
                                    nullptr);
        sp->runAction(seq);
        
    }
    else if(bullet->_id == 2)
    {
        float angle = -180 / M_PI * dir.getAngle() + 90;
        angle = angle < 0 ? (360 + angle) : angle;
        angle = angle > 180 ? angle-360 : angle;
        
        
        int r = random(1, 3);
        char c[7];
        sprintf(c, "%d",r);
        std::string path = "zidan-lianfa1C";
        path = path + c + std::string(".png");
        
        auto sp = Sprite::create(path);
        sp->setTag(GCommon::NO_COLL);
        sp->setAnchorPoint(Vec2(0.5,0));
        sp->setPosition(this->getPosition());
        sp->setRotation(angle);
        this->getParent()->getParent()->getChildByName("hideLayer")->addChild(sp);
        
        auto seq = Sequence::create(FadeOut::create(15),
                                    RemoveSelf::create(),
                                    nullptr);
        sp->runAction(seq);
        
        //受击
        auto particle = ParticleSystemQuad::create("zidan-lianfa1Blizi.plist");
        particle->setTag(GCommon::NO_COLL);
        particle->setScale(1.0f);
        particle->setPosition(this->getPosition());
        this->getParent()->addChild(particle);
        
        seq = Sequence::create(DelayTime::create(0.5f),
                               RemoveSelf::create(),
                               nullptr);
        particle->runAction(seq);
    }
    else if(bullet->_id == 3)
    {
        auto particle = ParticleSystemQuad::create("ppz.plist");
        particle->setScale(1.0f);
        particle->setPosition(0,0);
        this->addChild(particle);
        
        this->unschedule(SEL_SCHEDULE(&GBulletSprite::updateBullet_3));
        
        time = 1.0f;
    }
    else if(bullet->_id == 4)
    {
        if(bubble_4)
        {
//            bubble_4->isSkill_4 = false;
        }
        
        this->unschedule(SEL_SCHEDULE(&GBulletSprite::updateBullet_4));
    }
    else if(bullet->_id == 5)
    {
//        auto particle = ParticleSystemQuad::create("zidanzha.plist");
//        particle->setScale(1.0f);
//        particle->setPosition(0,0);
//        this->addChild(particle);
        this->unschedule(SEL_SCHEDULE(&GBulletSprite::updateBullet_5));
        if(bubble_5 && GGameController::getInstance()->isSelfBullet(bullet))
        {
            bullet_5->tid = bullet->tid;
            bullet_5->dirX = dir.x;
            bullet_5->dirY = dir.y;
            GModeGame::attack(bubble_5->bubble,bullet_5);
        }
    }
    else if(bullet->_id == 6)
    {
//        float angle = -180 / M_PI * dir.getAngle() + 90;
//        angle = angle < 0 ? (360 + angle) : angle;
//        angle = angle > 180 ? angle-360 : angle;
//        
//        auto sp = Sprite::create("bullet_003.png");
//        sp->setTag(GCommon::NO_COLL);
//        sp->setAnchorPoint(Vec2(0.5,0));
//        sp->setPosition(this->getPosition());
//        sp->setRotation(angle);
//        this->getParent()->addChild(sp);
//        
//        auto seq = Sequence::create(
//                                    TintTo::create(0, random(100, 255), random(100, 255), random(100, 255)),
//                                    FadeOut::create(15),
//                                    RemoveSelf::create(),
//                                    nullptr);
//        sp->runAction(seq);

        
    }
    else if(bullet->_id == 7 || bullet->_id == 8)
    {
        
    }
    if(bullet->_id != 4)
    _body->setVisible(false);
    this->stopAllActions();
    Action * stop = Sequence::create(DelayTime::create(time),
                                     CallFunc::create(CC_CALLBACK_0(GBulletSprite::dieEnd, this)),
                                     nullptr);
    this->runAction(stop);
}

void GBulletSprite::dieEnd()
{
    int tid = bullet->tid;
    this->removeFromParent();
    GGameController::getInstance()->deleteBullet(tid);

}

void GBulletSprite::attack(GBulletSprite* bullet)
{
    Node* bulletLayer = this->getParent();
    bulletLayer->addChild(bullet);
    Vec2 v = bulletLayer->convertToWorldSpace(this->getPosition())-bulletLayer->getParent()->getPosition();
    bullet->setPosition(v);
    
    bullet->run();
}

void GBulletSprite::findTarget()
{
    // 重新寻找最近的目标
    GBubbleSprite* sp = nullptr;
    for (int i=0; i<GGameController::getInstance()->bubbles.size(); i++) {
        GBubbleSprite* s = GGameController::getInstance()->bubbles.at(i);
        if(s && s->bubble->uid!=bullet->uid && s->bubble->state != GBubble::State::DIE)
        {
            if(!sp)
            {
                sp = s;
            }
            else
            {
                //判断距离最近
                Vec2 v = sp->getUpdatePosition();
                Vec2 v1 = s->getUpdatePosition();
                if(this->getPosition().getDistance(v1) < this->getPosition().getDistance(v))
                {
                    sp = s;
                }
            }
        }
    }
    //机器人
    for (int i=0; i<GGameController::getInstance()->robots.size(); i++) {
        GBubbleSprite* s = GGameController::getInstance()->robots.at(i);
        if(s && s->bubble->uid!=bullet->uid && s->bubble->state != GBubble::State::DIE)
        {
            if(!sp)
            {
                sp = s;
            }
            else
            {
                //判断距离最近
                Vec2 v = sp->getUpdatePosition();
                Vec2 v1 = s->getUpdatePosition();
                if(this->getPosition().getDistance(v1) < this->getPosition().getDistance(v))
                {
                    sp = s;
                }
            }
        }
    }
    //自己
    GBubbleSprite* s = GGameController::getInstance()->bubble;
    if(s && s->bubble->uid!=bullet->uid && s->bubble->state != GBubble::State::DIE)
    {
        if(!sp)
        {
            sp = s;
        }
        else
        {
            //判断距离最近
            Vec2 v = sp->getUpdatePosition();
            Vec2 v1 = s->getUpdatePosition();
            if(this->getPosition().getDistance(v1) < this->getPosition().getDistance(v))
            {
                sp = s;
            }
        }
    }
    if(sp)
    {
        Vec2 dir = (sp->getUpdatePosition() - this->getPosition()).getNormalized();
        if(bullet->type == 3)
        {
            dir = ((sp->getUpdatePosition() -Vec2(0,sp->getContentSize().height/2))- this->getPosition()).getNormalized();
        }
        changeTarget(dir);
    }
}

void GBulletSprite::changeTarget(cocos2d::Vec2 &dir)
{
    this->dir = dir;
}

void GBulletSprite::judgeColl(GBubbleSprite* target)
{
    if(bullet->type == 1 || bullet->type == 2 || bullet->type == 3)
    {
        float dis = target->getUpdatePosition().getDistance(this->getPosition());
        float dis2 = target->getContentSize().width/2;
        if(dis < dis2)
        {
            bullet->state = GBullet::State::DIE;
            GModeGame::bulletCollision(nullptr, target->bubble);
        }
    }
    else if(bullet->type == 4)
    {
//        if(!target->isHurt)
        {
            Node* bulletLayer = this->getParent()->getParent()->getChildByName("bulletLayer");
            for (int i=0; i<collSp.size(); i++) {
                auto coll = collSp.at(i);
                Vec2 v = coll->getParent()->convertToWorldSpace(coll->getPosition())-bulletLayer->getParent()->getPosition();
                
                float dis = target->getUpdatePosition().getDistance(v);
                float dis2 = target->getContentSize().width/2 + _body->getContentSize().width/3;
                if(dis < dis2)
                {
//                    target->hurt(Vec2::ZERO,bullet->damageCD);
                    GModeGame::bulletCollision(nullptr, target->bubble);
                    break;
                }
            }
        }
    }
    else if(bullet->type == 5)
    {
        return;
        
    }
    else if(bullet->type == 6)
    {
//        if(!target->isHurt)
        {
            float dis = target->getUpdatePosition().getDistance(this->getPosition());
            float dis2 = target->getContentSize().width/2 + _body->getContentSize().width/2*_body->getScale();
            if(dis < dis2)
            {
//                target->hurt(Vec2::ZERO,bullet->damageCD);
                GModeGame::bulletCollision(nullptr, target->bubble);
            }
        }
    }
    else if(bullet->type == 7)
    {
//        if(!target->isHurt)
        {
            float dis = target->getUpdatePosition().getDistance(this->getPosition());
            float dis2 = target->getContentSize().width/2 + bullet->range;
            if(dis < dis2)
            {
                //判断在扇形区域内
                Vec2 dir = (target->getUpdatePosition() - this->getPosition()).getNormalized();
                
                float angle = -180 / M_PI * dir.getAngle() + 90;
                angle = angle < 0 ? (360 + angle) : angle;
                angle = angle > 180 ? angle-360 : angle;
                
                
                float angle2 = _body->getRotation();
                angle2 = angle2 > 180 ? angle2 - 360 : angle2;
                angle2 = angle2 < -180 ? angle2 + 360 : angle2;
                
                float currAng = (angle2-90)/(-180 / M_PI);
                Vec2 currdir = Vec2::forAngle(currAng);
                
                Vec2 currDir1 = currdir;
                currDir1.rotate(Vec2::ZERO, 3.14f/180*bullet->targetRange/2);
                Vec2 currDir2 = currdir;
                currDir2.rotate(Vec2::ZERO, -3.14f/180*bullet->targetRange/2);
                
                float currAngle1 = -180 / M_PI * currDir1.getAngle() + 90;
                currAngle1 = currAngle1 < 0 ? (360 + currAngle1) : currAngle1;
                currAngle1 = currAngle1 > 180 ? currAngle1-360 : currAngle1;
                
                float currAngle2 = -180 / M_PI * currDir2.getAngle() + 90;
                currAngle2 = currAngle2 < 0 ? (360 + currAngle2) : currAngle2;
                currAngle2 = currAngle2 > 180 ? currAngle2-360 : currAngle2;
                
                float d1 = angle < 0 ? (360 + angle) : angle;
                float d2 = currAngle1 < 0 ? (360 + currAngle1) : currAngle1;
                float d3 = currAngle2 < 0 ? (360 + currAngle2) : currAngle2;
                float di1 = fabs(d1-d2);
                float di2 = fabs(d1-d3);
                di1 = MIN(di1, 360 - MAX(d1,d2) + MIN(d1, d2));
                di2 = MIN(di2, 360 - MAX(d1,d3) + MIN(d1, d3));
                if(di1 <= bullet->targetRange && di2 <= bullet->targetRange)
                {
//                    target->hurt(Vec2::ZERO,bullet->damageCD);
                    GModeGame::bulletCollision(nullptr, target->bubble);
                }
            }
        }
    }
}
