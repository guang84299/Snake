//
//  GBubbleSprite.cpp
//  OnlineGame
//
//  Created by guang on 16/10/2.
//
//

#include "GBubbleSprite.h"
#include "tools/GTools.h"
#include "data/GCache.h"
#include "common/GCommon.h"
#include "controller/GGameController.h"
#include "protocol_model/GModeGame.h"



USING_NS_CC;

#define ACTION_MOVE 1
#define ACTION_ROTATE 2
#define ACTION_SPEEDUP 3
#define ACTION_LOADBULLET 4
#define ACTION_FILLBULLET 5
#define ACTION_MOVE_LAST 6
#define ACTION_SPEEDDOWN 7
#define ACTION_COLL 8
#define ACTION_STOPMOVE 9
#define ACTION_AIM 10
#define ACTION_RECHP 11
#define ACTION_REDHP 12

GBubbleSprite::~GBubbleSprite()
{
    this->bubble->release();
    this->bullet->release();
}

GBubbleSprite* GBubbleSprite::create(GBubble* bubble)
{
    GBubbleSprite* sp = new GBubbleSprite();
    if(sp && sp->init(bubble))
    {
        sp->autorelease();
        return sp;
    }
    CC_SAFE_DELETE(sp);
    return nullptr;
}

bool GBubbleSprite::init(GBubble *bubble)
{
//    bubble->currSp = 1.0f;
    bubble->retain();
    this->bubble = bubble;
    
    GBullet* bulletData = GCache::getInstance()->getBullet(GCache::getInstance()->getBulletId());
    this->bullet = GBullet::create(bulletData);
    this->bullet->retain();
    this->skillBullet = nullptr;
    
    modeLayer = Layer::create();
    modeLayer->setTag(GCommon::NO_COLL);
    this->addChild(modeLayer);
    
    isSelf = bubble->uid == GCache::getInstance()->getUser()->uid;
  
    createShapeMode();
    
    //名字
    _name = Label::createWithSystemFont(bubble->name, "", 20);
    _name->setPosition(0, this->getContentSize().height/2 + 30);
    _name->setTag(GCommon::NO_COLL);
    this->addChild(_name,2);
    
    if(isSelf)
        _name->setTextColor(Color4B::RED);
    
    //血条
    hpBg = Sprite::create("hp_bg2.png");
    hpBg->setPosition(Vec2(0, -this->getContentSize().height/2 - 20));
    hpBg->setTag(GCommon::NO_COLL);
    this->addChild(hpBg,2);
    
    auto hpBg2 = Sprite::create("hp_bg.png");
//    hpBg2->setAnchorPoint(Vec2(0,0));
    hpBg2->setPosition(Vec2(hpBg->getContentSize().width/2, hpBg->getContentSize().height/2));
    hpBg2->setTag(GCommon::NO_COLL);
    hpBg->addChild(hpBg2);
    
    hpBar = LoadingBar::create("hp.png");
    hpBar->setTag(GCommon::NO_COLL);
    hpBar->setAnchorPoint(Vec2(0,0));
    hpBar->setPosition(Vec2(0, 0));
    hpBar->setDirection(LoadingBar::Direction::LEFT);
    hpBar->setPercent(100);
    hpBg2->addChild(hpBar);
    
    //技能icon
    skillIcon = Sprite::create("ui-anniu-zidan1.png");
    skillIcon->setPosition(Vec2(-10, hpBg->getContentSize().height/2));
    skillIcon->setTag(GCommon::NO_COLL);
    skillIcon->setScale(0.2f);
    hpBg->addChild(skillIcon);
    
    //子弹
    auto bulletBg = Sprite::create("hp_bg2.png");
    bulletBg->setPosition(Vec2(0, -this->getContentSize().height/2 - 25));
    bulletBg->setTag(GCommon::NO_COLL);
    this->addChild(bulletBg,2);
    
    auto bulletBg2 = Sprite::create("hp_bg.png");
    //    bulletBg2->setAnchorPoint(Vec2(0,0));
    bulletBg2->setPosition(Vec2(hpBg->getContentSize().width/2, hpBg->getContentSize().height/2));
    bulletBg2->setTag(GCommon::NO_COLL);
    bulletBg->addChild(bulletBg2);
    
    bulletBar = LoadingBar::create("bullet.png");
    bulletBar->setTag(GCommon::NO_COLL);
    bulletBar->setAnchorPoint(Vec2(0,0));
    bulletBar->setPosition(Vec2(0, 0));
    bulletBar->setDirection(LoadingBar::Direction::LEFT);
    bulletBar->setPercent(100);
    bulletBg2->addChild(bulletBar);
//
//    //经验
//    auto expBg = Sprite::create("hp_bg.png");
//    expBg->setPosition(Vec2(0, this->getContentSize().height/2 + 20));
//    expBg->setTag(GCommon::NO_COLL);
//    this->addChild(expBg,2);
//    
//    expBar = LoadingBar::create("hp.png");
//    expBar->setTag(GCommon::NO_COLL);
//    expBar->setAnchorPoint(Vec2(0,0));
//    expBar->setPosition(Vec2(0, 0));
//    expBar->setDirection(LoadingBar::Direction::LEFT);
//    expBar->setPercent(100);
//    expBg->addChild(expBar);

    
    currBulletNum = bullet->num;
    isSkill = false;
    isSkill_4 = false;
    skill_4_rotateSpeed = bubble->rotateSpeed;
    isHurt = false;
    isSpeedDown = false;
    downSpeed = 1;
    isSpeedUp = false;
    dirPower = 1;
    isSkill_Coll = false;
    skillCollTime = 0;
//    isPlaySpeedUp = false;
    target = nullptr;
//    isStopMove = true;
    updateHpBar();
    updateExp();
    updateBulletBar();
    
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create(CC_CALLBACK_0(GBubbleSprite::initData, this)), NULL));
    return true;
}

void GBubbleSprite::initData()
{
    isSelfBubble = (isSelf || (bubble->robot && GGameController::getInstance()->isSelfBubble(bubble)));
}

Vec2 GBubbleSprite::getUpdatePosition()
{
    return this->getPosition();
}

void GBubbleSprite::createShapeMode()
{
    modeLayer->removeAllChildren();
    
    moveParticle = ParticleSystemQuad::create("qianshuitingqipao.plist");
//    moveParticle->setPositionType(cocos2d::ParticleSystem::PositionType::GROUPED);
    modeLayer->addChild(moveParticle);
    upSpeedParticle = ParticleSystemQuad::create("qianshuitingqipao2.plist");
//    upSpeedParticle->setPositionType(cocos2d::ParticleSystem::PositionType::GROUPED);
    modeLayer->addChild(upSpeedParticle);
    upSpeedParticle->stopSystem();
    
//    fuzhu = Sprite::create("fuzhu1.png");
//    fuzhu->setAnchorPoint(Vec2(0.5,0.3));
//    modeLayer->addChild(fuzhu);
//    fuzhu->setVisible(isSelf);
    
    char c[7];
    sprintf(c, "%d",bubble->level+1);
    std::string level = c;
    sprintf(c, "%d",bubble->skinId);
    std::string skinId = c;
    
    std::string path = "skeleton";
    std::string lin = "_";
    std::string path1 = path + skinId + lin + level + std::string(".json");
    std::string path2 = path + skinId + lin + level + std::string(".atlas");
    skeleton = SkeletonAnimation::createWithFile(path1, path2);
    skeleton->setAnimation(0, "move", true);
    skeleton->setRotation(bubble->rotate);
    modeLayer->addChild(skeleton);
    
    tou = Sprite::create();
    skeleton->addChild(tou);
    
    modeLayer->setContentSize(Size(70,70));
    tou->setPosition(0, modeLayer->getContentSize().height/2);

    this->setContentSize(modeLayer->getContentSize());
    
//    fuzhu->setPosition(0, modeLayer->getContentSize().height);

    
    float r = MAX(this->getContentSize().width/2,this->getContentSize().height/2);
    
//    auto body = PhysicsBody::createCircle(r*1.1f);
//    body->setDynamic(false);
//    body->setName("body");
////    body->setPositionOffset(Vec2(-r,-r));
//    body->setCategoryBitmask(GCommon::PLANE_1);
//    body->setContactTestBitmask(GCommon::PLANE_2);
//    body->setCollisionBitmask(GCommon::PLANE_3);
//    skeleton->addComponent(body);

    
    this->removeComponent("body");
    auto body = PhysicsBody::createCircle(r*1.5f,PhysicsMaterial(1,0,1));
    body->setDynamic(true);
    body->setName("body");
    body->setRotationEnable(false);
    body->setPositionOffset(Vec2(-r,-r));
    body->setCategoryBitmask(GCommon::PLANE_CIRLE_1);
    body->setContactTestBitmask(GCommon::PLANE_CIRLE_2);
    body->setCollisionBitmask(GCommon::PLANE_CIRLE_3);
    this->addComponent(body);

}

void GBubbleSprite::addBody()
{
    
}

void GBubbleSprite::removeBody()
{
    modeLayer->getChildByTag(1)->removeComponent("body");
    modeLayer->removeChildByTag(1);
    this->removeComponent("body");
}

void GBubbleSprite::changeState(GBubble::State state)
{
    if(this->bubble->state == GBubble::State::DIE)
        return;
    this->bubble->state = state;
}

void GBubbleSprite::move(Vec2 &dir,float power,bool speedUp)
{
    if(this->bubble->state == GBubble::State::DIE)
        return;
    
    this->isSpeedUp = speedUp;
    
    if(isSpeedUp)
        changeState(GBubble::State::SPEEDUP);
    else
        changeState(GBubble::State::MOVE);
    
    currDir = dir;
    dirPower = power;
    
    if(target)
    {
        if(isSpeedUp)
            this->speedUp();
        else
            move2();
        return;
    }
    
    skeleton->stopActionByTag(ACTION_ROTATE);
//    this->stopActionByTag(ACTION_MOVE);
//    PhysicsBody* body = (PhysicsBody*)this->getComponent("body");
//    body->setVelocity(currDir*0);


    float angle = -180 / M_PI * dir.getAngle() + 90;
    angle = angle < 0 ? (360 + angle) : angle;
    angle = angle > 180 ? angle-360 : angle;
    
    float currAngle = skeleton->getRotation();
    
    float d1 = angle < 0 ? (360 + angle) : angle;
    float d2 = currAngle < 0 ? (360 + currAngle) : currAngle;
    float dis = fabs(d1-d2);
    dis = MIN(dis, 360 - MAX(d1,d2) + MIN(d1, d2));
    
    if(dis > 60 && dis < 90)
        this->stopActionByTag(ACTION_MOVE);
    
    float time = dis / bubble->rotateSpeed;
    if(isSkill_4)
        time = dis / skill_4_rotateSpeed;
    
    Action *actionRo = nullptr;
    if(isSpeedUp)
    {
        actionRo = Sequence::create(RotateTo::create(time, angle),
                                    CallFunc::create(CC_CALLBACK_0(GBubbleSprite::speedUp, this)),
                                    NULL);
    }
    else
    {
        actionRo = Sequence::create(RotateTo::create(time, angle),
                                    CallFunc::create(CC_CALLBACK_0(GBubbleSprite::move2, this)),
                                    NULL);
    }
    actionRo->setTag(ACTION_ROTATE);
    skeleton->runAction(actionRo);

//    bubble->lastDirX = 0;
//    bubble->lastDirY = 0;
//    bubble->rotateTime = time;
}

void GBubbleSprite::move2()
{
    if(isSkill_4)
        return;
    this->stopActionByTag(ACTION_MOVE);
//    this->stopActionByTag(ACTION_STOPMOVE);
    
    float angle = -180 / M_PI * currDir.getAngle() + 90;
    angle = angle < 0 ? (360 + angle) : angle;
    angle = angle > 180 ? angle-360 : angle;
    
    float w = this->getParent()->getParent()->getContentSize().width;
    float h = this->getParent()->getParent()->getContentSize().height;
    Vec2 v;
    if(angle >= -45 && angle <= 45)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,h),Vec2(w,h));
    }
    else if(angle > 45 && angle <= 135)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(w,0),Vec2(w,h));
    }
    else if(angle <= -135 || angle > 135)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,0),Vec2(w,0));
    }
    else
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,0),Vec2(0,h));
    }
    
    if(v.x < 0)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,0),Vec2(0,h));
    }
    else if(v.x > w)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(w,0),Vec2(w,h));
    }
    if(v.y < 0)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,0),Vec2(w,0));
    }
    else if(v.y > h)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,h),Vec2(w,h));
    }
    
    v = getMoveVec(v);
    
    float moveDis = v.getDistance(this->getPosition());
    float moveTime = moveDis/bubble->speed;
    if(dirPower <= bubble->limiRradius)
        moveTime = moveDis / bubble->limitSpeed;
    if(isSpeedDown)
    {
        moveTime = moveDis/(bubble->speed * downSpeed);
        if(dirPower <= bubble->limiRradius)
            moveTime = moveDis / (bubble->limitSpeed * downSpeed);
    }
//    bubble->currSp -= 0.1f;
//    if(bubble->currSp < 0)
//        bubble->currSp = 0;
    
//    Action *action = Speed::create(MoveTo::create(moveTime, v),bubble->currSp);
    
    Action* action = MoveTo::create(moveTime, v);
    action->setTag(ACTION_MOVE);
    this->runAction(action);
    
    if(lastSpeedUp)
    {
        lastSpeedUp = false;
        skeleton->setAnimation(0, "move", true);
    }
    
    
    this->stopActionByTag(ACTION_REDHP);
    
//    if (bubble->currSp != 0) {
//        Action* action2 = Sequence::create(DelayTime::create(0.2f),
//                                           CallFunc::create(CC_CALLBACK_0(GBubbleSprite::move2, this)),NULL);
//        action2->setTag(ACTION_STOPMOVE);
//        this->runAction(action2);
//    }
    
//    Vec2 speed = currDir * bubble->speed;
//    if(dirPower <= bubble->limiRradius)
//        speed = currDir * bubble->limitSpeed;
//    if(isSpeedDown)
//    {
//        speed = currDir * (bubble->speed * downSpeed);
//        if(dirPower <= bubble->limiRradius)
//            speed = currDir * (bubble->limitSpeed * downSpeed);
//    }
//    PhysicsBody* body = (PhysicsBody*)this->getComponent("body");
//    body->setVelocity(speed);
    
//    bubble->lastDirX = bubble->dirX;
//    bubble->lastDirY = bubble->dirY;
    
}

void GBubbleSprite::aim(GBubbleSprite* target)
{
    this->target = target;
    if(target == nullptr || isSpeedUp || bubble->state == GBubble::State::SPEEDUP)
        return;
    
    Action * ac = skeleton->getActionByTag(ACTION_AIM);
    if(ac && !ac->isDone())
        return;
    
    skeleton->stopActionByTag(ACTION_ROTATE);
    
    Vec2 dir = (target->getUpdatePosition()-this->getUpdatePosition()).getNormalized();
 
    float angle = -180 / M_PI * dir.getAngle() + 90;
    angle = angle < 0 ? (360 + angle) : angle;
    angle = angle > 180 ? angle-360 : angle;
    
    float currAngle = skeleton->getRotation();
    
    float d1 = angle < 0 ? (360 + angle) : angle;
    float d2 = currAngle < 0 ? (360 + currAngle) : currAngle;
    float dis = fabs(d1-d2);
    dis = MIN(dis, 360 - MAX(d1,d2) + MIN(d1, d2));
    
    float time = dis / bubble->rotateSpeed;
    if(isSkill_4)
        time = dis / skill_4_rotateSpeed;
    
    Action *actionRo = RotateTo::create(time, angle);
    actionRo->setTag(ACTION_AIM);
    skeleton->runAction(actionRo);
    
    auto seq = Sequence::create(DelayTime::create(time + 0.2f),
                                CallFunc::create(CC_CALLBACK_0(GBubbleSprite::aimEnd, this)), NULL);
    seq->setTag(ACTION_ROTATE);
    skeleton->runAction(seq);
}

void GBubbleSprite::aimEnd()
{
    float angle = -180 / M_PI * currDir.getAngle() + 90;
    angle = angle < 0 ? (360 + angle) : angle;
    angle = angle > 180 ? angle-360 : angle;
    skeleton->setRotation(angle);
}

Vec2 GBubbleSprite::getMoveVec(Vec2 &v)
{
    float w = this->getParent()->getParent()->getContentSize().width;
    float h = this->getParent()->getParent()->getContentSize().height;
    float dis = MAX(this->getContentSize().width/2,this->getContentSize().height/2);
    Vec2 by =  v;
    if(by.x < dis)
        by.x = dis;
    if(by.x > w-dis)
        by.x = w-dis;
    if(by.y < dis)
        by.y = dis;
    if(by.y > h-dis)
        by.y = h-dis;

    return by;
}

bool GBubbleSprite::isCollWall()
{
    float w = this->getParent()->getParent()->getContentSize().width;
    float h = this->getParent()->getParent()->getContentSize().height;
    float dis = MAX(this->getContentSize().width/2,this->getContentSize().height/2);
    bool b = false;
    Vec2 v =  this->getPosition();
    if(v.x < dis+10)
        b = true;
    if(v.x > w-dis-10)
        b = true;
    if(v.y < dis+10)
        b = true;
    if(v.y > h-dis-10)
        b = true;

    return b;
}

void GBubbleSprite::stopMove(bool speedUp)
{
    changeState(GBubble::State::IDLE);
    this->stopActionByTag(ACTION_MOVE);
    this->stopActionByTag(ACTION_SPEEDUP);
//    this->stopActionByTag(ACTION_STOPMOVE);
    skeleton->stopActionByTag(ACTION_ROTATE);
    
    lastSpeedUp = false;
    if(speedUp)
    {
        skeleton->setAnimation(0, "move", true);
    }
    this->stopActionByTag(ACTION_REDHP);
//    if(bubble->currSp < 1)
//    {
//        Action *action = EaseSineOut::create(MoveBy::create(1.f, currDir*200));
//        action->setTag(ACTION_MOVE);
//        this->runAction(action);
//    }
    
//    PhysicsBody* body = (PhysicsBody*)this->getComponent("body");
//    body->setVelocity(Vec2::ZERO);
//    isStopMove = true;
//    bubble->currSp = 1;
}

void GBubbleSprite::hurt(Vec2 dir,float damageCD)
{
    if(damageCD > 0)
    {
        isHurt = true;
        Action *action = Sequence::create(DelayTime::create(damageCD),
                                          CallFunc::create(CC_CALLBACK_0(GBubbleSprite::hurtEnd, this)),
                                          NULL);
        this->runAction(action);
    }
    
    
    //受击
//    auto particle = ParticleSystemQuad::create("zidan-lianfa1Blizi.plist");
//    particle->setTag(GCommon::NO_COLL);
//    particle->setScale(1.0f);
//    particle->setPosition(this->getPosition());
//    this->getParent()->addChild(particle);
//    
//    auto seq = Sequence::create(DelayTime::create(0.5f),
//                           RemoveSelf::create(),
//                           nullptr);
//    particle->runAction(seq);
    
    if(dir != Vec2::ZERO)
    {
        auto ac = EaseSineOut::create(MoveBy::create(0.25f, dir*40));
        this->runAction(ac);
    }
}

void GBubbleSprite::hurtEnd()
{
    isHurt = false;
}

void GBubbleSprite::updateExp()
{
//    int currLevelExp = GCache::getInstance()->getExp(bubble->level);
//    int nextLevelExp = GCache::getInstance()->getExp(bubble->level+1);
//    
//    float currExp = bubble->exp - currLevelExp;
//    float interExp = nextLevelExp - currLevelExp;
//    if(interExp > 0)
//    {
//        expBar->setPercent(currExp/interExp*100);
//    }
//    else
//    {
//        expBar->setPercent(100);
//    }
    if(bubble->grow != this->getScale())
    {
        createShapeMode();
        
        this->setScale(bubble->grow);
        PhysicsBody* body = (PhysicsBody*)this->getComponent("body");
        float r = MAX(this->getContentSize().width/2,this->getContentSize().height/2);
        body->setPositionOffset(Vec2(-r,-r)*bubble->grow);
        
        auto particle = ParticleSystemQuad::create("shengji.plist");
//        particle->setPosition(sp->getContentSize().width/2,sp->getContentSize().height/2);
        this->addChild(particle);
        particle->runAction(Sequence::create(DelayTime::create(2),RemoveSelf::create(), NULL));
    }
}

Vec2 GBubbleSprite::getCurrDir()
{
    float angle = skeleton->getRotation();
    angle = angle > 180 ? angle - 360 : angle;
    angle = angle < -180 ? angle + 360 : angle;
    
    float currAngle = (angle-90)/(-180 / M_PI);
    Vec2 dir = Vec2::forAngle(currAngle);
    
    return dir;
}

void GBubbleSprite::speedUp()
{
    if(this->bubble->state == GBubble::State::DIE)
        return;
    if(isSkill_4)
        return;
    isSpeedUp = true;
    
    changeState(GBubble::State::SPEEDUP);
    
    this->stopActionByTag(ACTION_MOVE);
    this->stopActionByTag(ACTION_SPEEDUP);

//    float angle = skeleton->getRotation();
//    angle = angle > 180 ? angle - 360 : angle;
//    angle = angle < -180 ? angle + 360 : angle;
//    
//    float currAngle = (angle-90)/(-180 / M_PI);
//    Vec2 dir = Vec2::forAngle(currAngle);
    
    float angle = -180 / M_PI * currDir.getAngle() + 90;
    angle = angle < 0 ? (360 + angle) : angle;
    angle = angle > 180 ? angle-360 : angle;
    
    skeleton->setRotation(angle);
    
    float w = this->getParent()->getParent()->getContentSize().width;
    float h = this->getParent()->getParent()->getContentSize().height;
    Vec2 v;
    if(angle >= -45 && angle <= 45)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,h),Vec2(w,h));
    }
    else if(angle > 45 && angle <= 135)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(w,0),Vec2(w,h));
    }
    else if(angle <= -135 || angle > 135)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,0),Vec2(w,0));
    }
    else
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,0),Vec2(0,h));
    }
    
    if(v.x < 0)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,0),Vec2(0,h));
    }
    else if(v.x > w)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(w,0),Vec2(w,h));
    }
    if(v.y < 0)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,0),Vec2(w,0));
    }
    else if(v.y > h)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+currDir,Vec2(0,h),Vec2(w,h));
    }

    
    v = getMoveVec(v);
    float moveDis = this->getPosition().getDistance(v);
//    if(moveDis > bubble->sDistance)
//    {
//        v = this->getPosition()+dir*bubble->sDistance;
//        v = getMoveVec(v);
//    }
    
    Vector<FiniteTimeAction*> vecs;
    moveDis = this->getPosition().getDistance(v);
    float moveTime = moveDis / bubble->sSpeed;
    if(isSpeedDown)
        moveTime = moveDis/(bubble->sSpeed * downSpeed);
    //FiniteTimeAction *a = EaseExponentialOut::create(MoveTo::create(moveTime, v));
    FiniteTimeAction *a = MoveTo::create(moveTime, v);
    
    vecs.pushBack(DelayTime::create(moveTime));
    vecs.pushBack(CallFunc::create(CC_CALLBACK_0(GBubbleSprite::speedUpEnd, this)));
//    vecs.pushBack(DelayTime::create(bubble->sCD+0.1f));
//    vecs.pushBack(CallFunc::create(CC_CALLBACK_0(GBubbleSprite::speedUpCDEnd, this)));
    
    Action *action = Sequence::create(vecs);
    action->setTag(ACTION_SPEEDUP);
    this->runAction(action);
    
    a->setTag(ACTION_MOVE);
    this->runAction(a);
    
    if(!lastSpeedUp)
    {
        lastSpeedUp = true;
        skeleton->setAnimation(0, "shadow", false);//speedup
    }

    upSpeedParticle->resetSystem();
    upSpeedParticle->setRotation(skeleton->getRotation());

    
//    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
//    GGameScene* game = dynamic_cast<GGameScene*>(sc);
//    if (game && isSelf)
//    {
//        game->updateSpeedCd(bubble->sDistance/bubble->sSpeed, 0);
//    }
    this->stopActionByTag(ACTION_RECHP);
    if(isSelfBubble)
    reduceHp();
    
    if(bubble->currHp <= bubble->HP*0.1f)
    {
        speedUpEnd();
    }
}

void GBubbleSprite::speedUpEnd()
{
    this->stopActionByTag(ACTION_MOVE);
    this->stopActionByTag(ACTION_SPEEDUP);
    
    changeState(GBubble::State::IDLE);
    
    
    skeleton->setAnimation(0, "move", true);
    
//    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
//    GGameScene* game = dynamic_cast<GGameScene*>(sc);
//    if (game && isSelf)
//    {
//        game->updateSpeedCd(bubble->sCD+0.1f, 100);
//    }
    
    upSpeedParticle->stopSystem();
    
    this->stopActionByTag(ACTION_REDHP);
    if(isSelfBubble)
        recoverHp();
    isSpeedUp = false;
}

void GBubbleSprite::speedUpCDEnd()
{
    isSpeedUp = false;
}

void GBubbleSprite::speedDown(float downSpeed,float time)
{
    if(this->bubble->state == GBubble::State::DIE)
        return;
    this->stopActionByTag(ACTION_MOVE);
    this->stopActionByTag(ACTION_SPEEDUP);
    this->stopActionByTag(ACTION_SPEEDDOWN);
    
    isSpeedDown = true;
    this->downSpeed = downSpeed;
    
    Action * stop = Sequence::create(
                                     DelayTime::create(time),
                                     CallFunc::create(CC_CALLBACK_0(GBubbleSprite::speedDownEnd, this)),
                                     nullptr);
    this->runAction(stop);
    if(bubble->state == GBubble::State::MOVE)
        move(currDir,dirPower,isSpeedUp);
}

void GBubbleSprite::speedDownEnd()
{
    isSpeedDown = false;
}

void GBubbleSprite::startAttack()
{
    if(this->bubble->state == GBubble::State::DIE)
        return;
    if(isSkill || isSkill_4 || isSkill_Coll)
        return;
    float cd = bullet->CD+0.1f;
    if(skillBullet)
    {
        isSkill = true;
        cd = skillBullet->CD+0.05f;
        if(skillBullet->type == 4)
        {
            isSkill_4 = true;
            skill_4_rotateSpeed = skillBullet->rotateSpeed;
            this->stopActionByTag(ACTION_MOVE);
            this->stopActionByTag(ACTION_SPEEDUP);
            cd+=100;
        }
        
        if(skillBullet->type == 4 || skillBullet->type == 6 || skillBullet->type == 7)
        {
            isSkill_Coll = true;
            skillCollTime = skillBullet->bulletTime;
        }
//        playSkillAnimate();
    }
    this->schedule(SEL_SCHEDULE(&GBubbleSprite::updateAttack),cd);
    updateAttack(0);
}

void GBubbleSprite::stopAttack()
{
    if(!isSkill)
    this->unschedule(SEL_SCHEDULE(&GBubbleSprite::updateAttack));
}

void GBubbleSprite::updateAttack(float dt)
{
    if(bubble->state != GBubble::State::DIE)
    {
        if(isSkill)
        {
            Vec2 dir = getCurrDir();
            skillBullet->dirX = dir.x;
            skillBullet->dirY = dir.y;
            GModeGame::attack(bubble,skillBullet);
        }
        else
        {
            if(currBulletNum > 0)
            {
                Vec2 dir = getCurrDir();
                bullet->dirX = dir.x;
                bullet->dirY = dir.y;
                GModeGame::attack(bubble,bullet);
            }
        }
        
    }
    
}

void GBubbleSprite::attack(GBulletSprite* bullet)
{
    if(this->bubble->state == GBubble::State::DIE)
        return;
    skeleton->setAnimation(0, "attack2", false);
    
    Node* bulletLayer = this->getParent()->getParent()->getChildByName("bulletLayer");
    if(bullet->bullet->type == 4)
    {
        bulletLayer = this->getParent();
    }
    bulletLayer->addChild(bullet,2);
    Vec2 v = getBulletPosition(bullet->bullet->type);
    bullet->setPosition(v);
    if(bullet->bullet->_id == 1 || bullet->bullet->type == 2 || bullet->bullet->type == 3 || bullet->bullet->type == 7)
        bullet->setUpdateRotation(this->getUpdateRotation());
    bullet->run();

    if(isSkill)
    {
        skillBullet->continueNum--;
        if(skillBullet->continueNum <= 0)
        {
            GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
            GGameScene* game = dynamic_cast<GGameScene*>(sc);
            if (game && isSelf)
            {
                game->updateBulletSkill(1);
            }
            updateSkillIcon(1);
//            stopSkillAnimate();
            isSkill = false;
            stopAttack();
            skillBullet->release();
            skillBullet = nullptr;
        }
    }
    else
    {
        currBulletNum--;
        
        this->stopActionByTag(ACTION_FILLBULLET);
        this->stopActionByTag(ACTION_LOADBULLET);
        if(currBulletNum <= 0)
        {
            auto seq = Sequence::create(DelayTime::create(this->bullet->fillBullet),
                                        CallFunc::create(CC_CALLBACK_0(GBubbleSprite::fillBullet, this)), NULL);
            seq->setTag(ACTION_FILLBULLET);
            this->runAction(seq);
        }
        else if(currBulletNum < this->bullet->num)
        {
            auto seq = Sequence::create(DelayTime::create(this->bullet->loadBullet),
                                        CallFunc::create(CC_CALLBACK_0(GBubbleSprite::loadBullet, this)), NULL);
            seq->setTag(ACTION_LOADBULLET);
            this->runAction(seq);
        }
    }
    auto seq = Sequence::create(DelayTime::create(0.15f),
                                CallFunc::create(CC_CALLBACK_0(GBubbleSprite::attackEnd, this)), NULL);
    this->runAction(seq);
    
    updateBulletBar();
    GTools::playSound(SOUND_SHOOT);
}

void GBubbleSprite::attackEnd()
{
    skeleton->setAnimation(0, "move", true);
}

void GBubbleSprite::playSkillAnimate()
{
    stopSkillAnimate();
    if(skillBullet->_id == 2)
    {
        auto sp = Sprite::create();
        sp->setName("skillAnimate");
        sp->setTag(GCommon::NO_COLL);
        sp->setAnchorPoint(Vec2(0.5,0));
        sp->setPosition(0,modeLayer->getContentSize().height/2-5);
        skeleton->addChild(sp);
        
        sp->runAction(RepeatForever::create(GTools::createAnimate("bullet002_fire", 6, 0.08f)));
    }
}

void GBubbleSprite::stopSkillAnimate()
{
    if(skillBullet->_id == 2)
    {
        skeleton->removeChildByName("skillAnimate");
    }
}

void GBubbleSprite::die()
{
    skeleton->setAnimation(0, "die", false);
    bubble->state = GBubble::State::DIE;
    
    Action * stop = Sequence::create(
                                     FadeOut::create(1),
                                     CallFunc::create(CC_CALLBACK_0(GBubbleSprite::dieEnd, this)),
                                     nullptr);
    this->stopAllActions();
    this->runAction(stop);
}

void GBubbleSprite::dieEnd()
{
    Node* bulletLayer = this->getParent()->getParent()->getChildByName("hideLayer");
    auto sp = Sprite::create("die.png");
    sp->setTag(GCommon::NO_COLL);
    sp->setAnchorPoint(Vec2(0.5,0.5));
    sp->setPosition(getBulletPosition(6));
    sp->setRotation(this->getUpdateRotation());
    bulletLayer->addChild(sp);
    
    auto seq = Sequence::create(FadeOut::create(10),
                           RemoveSelf::create(),
                           nullptr);
    sp->runAction(seq);
    
    if(GGameController::getInstance()->isSelfBubble(bubble))
    {
        GModeGame::drop(bubble);
    }
    
    if(isSelf)
    {
        this->setVisible(false);
    }
    else
    {
        std::string uid = bubble->uid;
        bool robot = bubble->robot;
        this->removeFromParent();
        if(robot)
            GGameController::getInstance()->deleteRobot(uid);
        else
            GGameController::getInstance()->deleteBubble(uid);
    }
    
    GTools::playSound(SOUND_DIE);
}

void GBubbleSprite::changeSkillBullet(int bulletId)
{
    if(skillBullet)
    {
        isSkill = false;
        stopAttack();
        skillBullet->release();
        skillBullet = nullptr;
    }
    GBullet* bulletData = GCache::getInstance()->getBullet(bulletId);
    if(bulletData)
    {
        this->skillBullet = GBullet::create(bulletData);
        this->skillBullet->retain();
        this->skillNum = this->skillBullet->continueNum;
        stopAttack();
        if(isSelf)
        {
            GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
            GGameScene* game = dynamic_cast<GGameScene*>(sc);
            if (game)
            {
                game->updateBulletSkill(bulletId);
            }
        }
        updateSkillIcon(bulletId);
    }
}

float GBubbleSprite::getBulletRange()
{
    if(skillBullet)
        return skillBullet->range;
    return bullet->range;
}

void GBubbleSprite::updateSkillIcon(int bulletId)
{
    char c[7];
    sprintf(c, "%d",bulletId);
    std::string path = "ui-anniu-zidan";
    path = path + c + std::string(".png");
    skillIcon->initWithFile(path);
}

void GBubbleSprite::fillBullet()
{
    currBulletNum = bullet->num;
    updateBulletBar();
}

void GBubbleSprite::loadBullet()
{
    currBulletNum++;
    updateBulletBar();
    if(currBulletNum < this->bullet->num)
    {
        auto seq = Sequence::create(DelayTime::create(this->bullet->loadBullet),
                                    CallFunc::create(CC_CALLBACK_0(GBubbleSprite::loadBullet, this)), NULL);
        seq->setTag(ACTION_LOADBULLET);
        this->runAction(seq);
    }
}

void GBubbleSprite::updateHpBar()
{
    float per = 0;
    if(bubble->currHp > 0)
    {
        float hp = bubble->currHp;
        per = hp / bubble->HP * 100;
        
        if(per <= 10)
        {
            GTools::playSound(SOUND_DANGERIOUS);
        }
    }
    hpBar->setPercent(per);
}

void GBubbleSprite::updateBulletBar()
{
    float per = 0;
    if(currBulletNum > 0)
    {
        float num = currBulletNum;
        per = num / bullet->num * 100;
    }
    bulletBar->setPercent(per);
    
    if(isSkill_Coll)
    {
        GGameScene* game = getSelfGame();
        if (game)
        {
            game->updateAttackCd(skillCollTime, 0);
            auto * stop = Sequence::create(DelayTime::create(skillCollTime),
                                             CallFunc::create(CC_CALLBACK_0(GBubbleSprite::updateSkillBulletBarEnd, this)),
                                             nullptr);
            this->runAction(stop);
        }
        return;
    }
    if(isSkill)
    {
        if(skillBullet->continueNum <= 0)
        {
            GGameScene* game = getSelfGame();
            if (game)
            {
                game->updateAttackCd(0.1f, per);
            }
        }
        else
        {
            GGameScene* game = getSelfGame();
            if (game)
            {
                float per = (float)skillBullet->continueNum / (float)this->skillNum * 100;
                game->updateAttackCd(0.1f, per);
            }
        }
    }
    else
    {
        if(currBulletNum <= 0)
        {
            GGameScene* game = getSelfGame();
            if (game)
            {
                game->updateAttackCd(0, 0);
                game->updateAttackCd(this->bullet->fillBullet, 100);
            }
        }
        else
        {
            GGameScene* game = getSelfGame();
            if (game)
            {
                game->updateAttackCd(0, per);
                float per = (float)(currBulletNum+1) / (float)this->bullet->num * 100;
                game->updateAttackCd(this->bullet->loadBullet, per);
            }
        }
    }
}

void GBubbleSprite::updateSkillBulletBarEnd()
{
    isSkill_Coll = false;
    updateBulletBar();
}

GGameScene* GBubbleSprite::getSelfGame()
{
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    GGameScene* game = dynamic_cast<GGameScene*>(sc);
    if (game && isSelf)
        return game;
    return nullptr;
}

void GBubbleSprite::updatePosAndRotate()
{
    bubble->x = getUpdatePosition().x;
    bubble->y = getUpdatePosition().y;
    bubble->rotate = getUpdateRotation();
    
//    moveParticle->setRotation(bubble->rotate);
//    upSpeedParticle->setRotation(bubble->rotate);
    
//    fuzhu->setRotation(bubble->rotate);
}

void GBubbleSprite::updateRotation(float rotate)
{
    skeleton->setRotation(rotate);
}
float GBubbleSprite::getUpdateRotation()
{
    return skeleton->getRotation();
}

void GBubbleSprite::recoverHp()
{
    this->stopActionByTag(ACTION_RECHP);
    if(bubble->currHp < bubble->HP)
    {
        auto ac = Sequence::create(DelayTime::create(bubble->recoverCD),
                                   CallFunc::create(CC_CALLBACK_0(GBubbleSprite::recoverHpEnd, this)), NULL);
        ac->setTag(ACTION_RECHP);
        this->runAction(ac);
    }
}

void GBubbleSprite::recoverHpEnd()
{
    GModeGame::updateHp(bubble, 1);
    recoverHp();
}

void GBubbleSprite::reduceHp()
{
    this->stopActionByTag(ACTION_REDHP);
    if(bubble->currHp > bubble->HP*0.1f)
    {
        auto ac = Sequence::create(DelayTime::create(bubble->reduceCD),
                                   CallFunc::create(CC_CALLBACK_0(GBubbleSprite::reduceHpEnd, this)), NULL);
        ac->setTag(ACTION_REDHP);
        this->runAction(ac);
    }
    else{
        if(isSpeedUp)
        {
            speedUpEnd();
        }
    }
}

void GBubbleSprite::reduceHpEnd()
{
    GModeGame::updateHp(bubble, 0);
    reduceHp();
}

void GBubbleSprite::updatePos(float x,float y,float rotate,int state)
{
    Vec2 p(x,y);
    this->setPosition(p);
    
//    struct timeval tv;
//    gettimeofday(&tv, NULL);
//    long time_sec = ((long long)tv.tv_sec) * 1000+ tv.tv_usec / 1000;
//   
////    log("%d %d %d",(int)(time_sec-lt),(int)x,(int)y);
//    int t = (int)(time_sec-lt) - 50;
//    lt = time_sec;
    
    
//    if(state == 1)
//    {
//        Vec2 p(x,y);
//        this->setPosition(p);
//    }
//    else if(state == 2)
//    {
//        Vec2 p(x,y);
//        float dis = this->getPosition().getDistance(p);
//        if(dis >= 3)
//        {
//            auto ac = MoveTo::create(dis/(bubble->speed*1.3f), p);
//            ac->setTag(ACTION_MOVE);
//            this->runAction(ac);
//        }
//        //如果点和上次一样则预测一个
////        else
////        {
////            currDir = (p - this->getPosition()).getNormalized();
////            Vec2 pos = this->getPosition() + currDir * bubble->speed * 0.05f;
////            float dis = this->getPosition().getDistance(pos);
////            auto ac = MoveTo::create(dis/bubble->speed, pos);
////            ac->setTag(ACTION_MOVE);
////            this->runAction(ac);
////            
////            log("w pos");
////        }
//    }
//    else if(state == 3)
//    {
//        this->stopActionByTag(ACTION_MOVE);
//        if(!isPlaySpeedUp)
//        {
//            isPlaySpeedUp = true;
//            skeleton->setAnimation(0, "speedup", false);
//        }
//        
//        Vec2 p(x,y);
//        float dis = this->getPosition().getDistance(p);
//        if(dis >= 3)
//        {
//            auto ac = MoveTo::create(dis/(bubble->sSpeed), p);
//            ac->setTag(ACTION_MOVE);
//            this->runAction(ac);
//        }
//    }
//    
//    skeleton->stopActionByTag(ACTION_ROTATE);
//    auto ro = RotateTo::create(0.04f, rotate);
//    ro->setTag(ACTION_ROTATE);
//    skeleton->runAction(ro);
}

float GBubbleSprite::getCollAndBlock()
{
    float r = MAX(this->getContentSize().width/2,this->getContentSize().height/2)*1.8f;
    return  r*bubble->grow;
}

Vec2 GBubbleSprite::getBulletPosition(int type)
{
    Node* bulletLayer = this->getParent()->getParent()->getChildByName("bulletLayer");
    Vec2 v;
    if(type == 6)
    {
        v = skeleton->getParent()->convertToWorldSpace(skeleton->getPosition())-bulletLayer->getParent()->getPosition();
    }
    else
    {
//         v = skeleton->getParent()->convertToWorldSpace(skeleton->getPosition())-bulletLayer->getParent()->getPosition();
        v = skeleton->convertToWorldSpace(tou->getPosition())-bulletLayer->getParent()->getPosition();
    }
    return v;
}
