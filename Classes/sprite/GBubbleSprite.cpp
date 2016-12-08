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
    
    
    isSpeedDown = false;
    isSpeedUp = false;
    dirPower = 1;
    updateHpBar();
    updateExp();
    
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
    modeLayer->addChild(upSpeedParticle);
    upSpeedParticle->stopSystem();
    
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
    
    if(isSpeedUp)
        this->speedUp();
    else
        move2();
    
    skeleton->stopActionByTag(ACTION_ROTATE);


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
}

void GBubbleSprite::move2()
{
    this->stopActionByTag(ACTION_MOVE);
    
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
    
    Action* action = MoveTo::create(moveTime, v);
    action->setTag(ACTION_MOVE);
    this->runAction(action);
    
    
    this->stopActionByTag(ACTION_REDHP);
    
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
    skeleton->stopActionByTag(ACTION_ROTATE);
    
    this->stopActionByTag(ACTION_REDHP);
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
    

    upSpeedParticle->resetSystem();
    upSpeedParticle->setRotation(skeleton->getRotation());

    
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
