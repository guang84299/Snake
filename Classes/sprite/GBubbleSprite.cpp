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
    _name->setPosition(0, -this->getContentSize().height/2 - 30);
    _name->setTag(GCommon::NO_COLL);
    this->addChild(_name,2);
    
    if(isSelf)
        _name->setTextColor(Color4B(245,230,126,255));
    
    
    isSpeedDown = false;
    isSpeedUp = false;
    dirPower = 1;
    body_z = 99999;
    sSpeed = 0;

    updateExp(false,false);
//    this->schedule(SEL_SCHEDULE(&GBubbleSprite::update));
    this->runAction(Sequence::create(DelayTime::create(1),
                                     CallFunc::create(CC_CALLBACK_0(GBubbleSprite::initData, this)), NULL));
    
    return true;
}

void GBubbleSprite::initData()
{
    isSelfBubble = (isSelf || (bubble->robot && GGameController::getInstance()->isSelfBubble(bubble)));
    
}

void GBubbleSprite::initBody()
{
    for(int i=0;i<bubble->level+14;i++)
        addBody();
}

Vec2 GBubbleSprite::getUpdatePosition()
{
    return this->getPosition();
}


void GBubbleSprite::createShapeMode()
{
    for (int i=0; i<bodys.size(); i++) {
        auto b = bodys.at(i);
        b->removeFromParent();
    }
    bodys.clear();
    body_z = 99999;
    modeLayer->removeAllChildren();
    
    
    char c[7];
    sprintf(c, "%d",bubble->skinId);
    std::string skinId = c;
    
    std::string path = "juese";
    std::string path1 = path + skinId + std::string("-1.png");
    body = Sprite::create(path1);
//    body->setAnchorPoint(Vec2(0.5,1));
    body->setRotation(bubble->rotate);
    body->setScale(bubble->grow);
    modeLayer->addChild(body);
    
    tou = Sprite::create();
    body->addChild(tou);
    
    modeLayer->setContentSize(body->getContentSize()*body->getScale());
    tou->setPosition(0, modeLayer->getContentSize().height/2);

    this->setContentSize(modeLayer->getContentSize());
}

void GBubbleSprite::resetAll()
{
    body->setScale(bubble->grow);
    modeLayer->setContentSize(body->getContentSize()*body->getScale());
    tou->setPosition(0, modeLayer->getContentSize().height/2);
    
    this->setContentSize(modeLayer->getContentSize());

    for (int i=0; i<bodys.size(); i++) {
        auto b = bodys.at(i);
        b->resetScale(body->getScale());
    }
}

void GBubbleSprite::update(float dt)
{
    if(bubble->state == GBubble::State::DIE)
        return;
    
    float angle = getUpdateRotation();
    
    float rotateSpeedSc = 1.4f;
    if(isSpeedUp || isCheck)
        rotateSpeedSc = 4.0f;
    
    if(this->angle > angle)
    {
        float dis = this->angle - angle;
        float dis2 = 360 - this->angle + angle;
        if(dis < dis2)
        {
            angle = angle + bubble->rotateSpeed*dt*rotateSpeedSc;
            if(angle > this->angle)
                angle = this->angle;
        }
        else
        {
            angle = angle - bubble->rotateSpeed*dt*rotateSpeedSc;
            if(angle < 0)
                angle = 360 + angle;
        }
    }
    else
    {
        float dis = angle - this->angle;
        float dis2 = 360 - angle + this->angle;
        if(dis < dis2)
        {
            angle = angle - bubble->rotateSpeed*dt*rotateSpeedSc;
            if(angle < this->angle)
                angle = this->angle;
        }
        else
        {
            angle = angle + bubble->rotateSpeed*dt*rotateSpeedSc;
            if(angle > 360)
                angle = angle - 360;
        }
    }
    updateRotation(angle);
    
    Vec2 dir = getCurrDir();
    
    float speed = bubble->speed;
    int lag = 40*body->getScale();
    if(isSpeedUp)
    {
        speed = bubble->sSpeed;
        lag =  roundf(lag/2.f);
        sSpeed += dt;
        if(sSpeed >= bubble->reduceCD)
        {
            sSpeed = 0;
            if(isSelf)
                GModeGame::updateHp(bubble, 0,bodys.at(bodys.size()-1)->getPosition());
        }
    }
    
    if(isCheck)
    {
        speed = bubble->sSpeed;
        if(!isSpeedUp)
        lag = roundf(lag/2.f);
    }
    
    if(isDelay)
    {
//        speed = speed / 2;
//        lag =  lag*2;
    }
    
    float t = dt/4;
    for(int i=1;i<=4;i++)
    {
        Vec2 pos = dir * speed * t * i + this->getPosition();
        points.push_back(GPath(pos, this->getUpdateRotation()));
    }
    
    int p = (int)points.size()-1;
    for(int i=0;i<bodys.size();i++)
    {
        p = p - lag;
        if(p>=0)
        {
            GBodySprite* sp = bodys.at(i);
            GPath path = points.at(p);
            sp->setPosition(path.p);
            sp->setRotation(path.angle);
            if(isSpeedUp)
                sp->showSpeed();
        }
    }
//    int m_l = (int)bodys.size() * lag;
    if(p > 10)
    {
        points.erase(points.begin(),points.begin() + (p-10));
    }
    
    
    if(vecs.size())
    {
        delayDt = 0;
        
//        dir = (vecs.at(0).p - this->getPosition()).getNormalized();
        lastDir = dir;
        Vec2 pos = dir*dt*speed + this->getPosition();
        this->setPosition(pos);
        float dis = vecs.at(0).p.getDistance(pos);
        float d = 10;
        if(isCheck || isSpeedUp)
            d = d*2;
        if(dis <= d)
        {
            vecs.erase(vecs.begin());
        }
    }
    else
    {
        delayDt += dt;
        if(delayDt>0.07f)
        {
            delayDt = 0;
            isDelay = true;
            log("isDelay");
        }
        if(!isDelay)
        {
            Vec2 pos = lastDir*dt*speed + this->getPosition();
            this->setPosition(pos);
        }
        
    }
}

void GBubbleSprite::updateBody(cocos2d::Action* ac)
{
    for(int i=0;i<bodys.size();i++)
    {
//        GBodySprite* sp = bodys.at(i);
//        sp->move(0.1f*i,ac->clone());
    }
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
    
    
    
//    currDir = dir;
    dirPower = power;
//    
//    body->stopActionByTag(ACTION_ROTATE);
//
//
//    float angle = -180 / M_PI * dir.getAngle() + 90;
//    angle = angle < 0 ? (360 + angle) : angle;
//    this->angle = angle;
//    angle = angle > 180 ? angle-360 : angle;
//    
//    float currAngle = body->getRotation();
//    
//    float d1 = angle < 0 ? (360 + angle) : angle;
//    float d2 = currAngle < 0 ? (360 + currAngle) : currAngle;
//    float dis = fabs(d1-d2);
//    dis = MIN(dis, 360 - MAX(d1,d2) + MIN(d1, d2));
//    
//    
//    float time = dis / bubble->rotateSpeed * 2;
//
//    Action *actionRo = RotateTo::create(time, angle);
//    actionRo->setTag(ACTION_ROTATE);
//    body->runAction(actionRo);
}

bool GBubbleSprite::isRotate()
{
    Action* ac = body->getActionByTag(ACTION_ROTATE);
    if(ac && !ac->isDone())
    {
        return true;
    }
    return false;
}

void GBubbleSprite::move2()
{
    this->stopActionByTag(ACTION_MOVE);
    Vec2 dir = getCurrDir();
    float angle = -180 / M_PI * dir.getAngle() + 90;
    angle = angle < 0 ? (360 + angle) : angle;
    angle = angle > 180 ? angle-360 : angle;
    
    float w = this->getParent()->getParent()->getContentSize().width;
    float h = this->getParent()->getParent()->getContentSize().height;
    Vec2 v;
    if(angle >= -45 && angle <= 45)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+dir,Vec2(0,h),Vec2(w,h));
    }
    else if(angle > 45 && angle <= 135)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+dir,Vec2(w,0),Vec2(w,h));
    }
    else if(angle <= -135 || angle > 135)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+dir,Vec2(0,0),Vec2(w,0));
    }
    else
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+dir,Vec2(0,0),Vec2(0,h));
    }
    
    if(v.x < 0)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+dir,Vec2(0,0),Vec2(0,h));
    }
    else if(v.x > w)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+dir,Vec2(w,0),Vec2(w,h));
    }
    if(v.y < 0)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+dir,Vec2(0,0),Vec2(w,0));
    }
    else if(v.y > h)
    {
        v = Vec2::getIntersectPoint(this->getPosition(),this->getPosition()+dir,Vec2(0,h),Vec2(w,h));
    }
    
    v = getMoveVec(v);
    
    float moveDis = v.getDistance(this->getPosition());
    float moveTime = moveDis/bubble->speed;
//    if(dirPower <= bubble->limiRradius)
//        moveTime = moveDis / bubble->limitSpeed;
    
    Action* action = MoveTo::create(moveTime, v);
    //MoveTo::create(moveTime, v);
    action->setTag(ACTION_MOVE);
    this->runAction(action);
   // updateBody(action);
    
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
    body->stopActionByTag(ACTION_ROTATE);
    
    this->stopActionByTag(ACTION_REDHP);
}

void GBubbleSprite::updateExp(bool up,bool move)
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
    if(bubble->grow != body->getScale())
    {
//        if(bubble->grow > body->getScale())
//        {
//            auto particle = ParticleSystemQuad::create("shengji.plist");
//            //        particle->setPosition(sp->getContentSize().width/2,sp->getContentSize().height/2);
//            this->addChild(particle);
//            particle->runAction(Sequence::create(DelayTime::create(2),RemoveSelf::create(), NULL));
//        }
//        createShapeMode();
//        initBody();
        resetAll();
    }
    if(up)
    {
        addBody();
    }
    if(move)
    {
        removeBody();
    }
    if(isSpeedUp && bubble->exp <= 0)
    {
        this->move(currDir,1,false);
//        changeState(GBubble::State::MOVE);
//        
//        GModeGame::move(currDir,1,bubble);
    }
    if(isSelf)
    {
        GGameScene* game = getSelfGame();
        if(game)
        {
            game->updateExp();
        }
    }
}

Vec2 GBubbleSprite::getCurrDir()
{
    float angle = body->getRotation();
    angle = angle > 180 ? angle - 360 : angle;
    angle = angle < -180 ? angle + 360 : angle;
    
    float currAngle = (angle-90)/(-180 / M_PI);
    Vec2 dir = Vec2::forAngle(currAngle);
    
    return dir;
}


Vec2 GBubbleSprite::getBodyDir(Sprite* body)
{
    float angle = body->getRotation();
    angle = angle > 180 ? angle - 360 : angle;
    angle = angle < -180 ? angle + 360 : angle;
    
    float currAngle = (angle-90)/(-180 / M_PI);
    Vec2 dir = Vec2::forAngle(currAngle);
    
    return dir;
}

Sprite* GBubbleSprite::getBody()
{
    return this->body;
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
    
    body->setRotation(angle);
    
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
   // updateBody(a);

    
    this->stopActionByTag(ACTION_RECHP);
    if(isSelfBubble)
    reduceHp();
    
//    if(bubble->currHp <= bubble->HP*0.1f)
//    {
//        speedUpEnd();
//    }
}

void GBubbleSprite::speedUpEnd()
{
    this->stopActionByTag(ACTION_MOVE);
    this->stopActionByTag(ACTION_SPEEDUP);
    
    changeState(GBubble::State::IDLE);
    
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
    if(GGameController::getInstance()->isSelfBubble(bubble))
    {
        GJsonArray* arr = GJsonArray::create();
        for(int i=0;i<bodys.size();i++)
        {
            GJsonObject* obj = GJsonObject::create();
            obj->putInt("x", bodys.at(i)->getPositionX());
            obj->putInt("y", bodys.at(i)->getPositionY());
            
            arr->add(obj);
        }
        GModeGame::drop(bubble,arr);
    }
    
    this->removeAllBody();
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
    if(isSelf)
        GTools::playSound(SOUND_DIE);
    else
        GTools::playSound(SOUND_SCORE);
}

void GBubbleSprite::addBody()
{
    Sprite* parent = nullptr;
    if(bodys.size() == 0)
    {
        parent = this;
    }
    else
    {
        parent = bodys.at(bodys.size()-1);
    }
    auto b = GBodySprite::create(this,parent,bubble->skinId,(int)bodys.size(),body->getScale());
    b->setRotation(parent->getRotation());
    b->setPosition(parent->getPosition() );//+ getBodyDir(parent)*-14);
    parent->getParent()->addChild(b,--body_z);
    bodys.push_back(b);
}

void GBubbleSprite::removeBody()
{
    if(bodys.size() <= 4)
        return;
    GBodySprite* b = bodys.at(bodys.size()-1);
    b->removeFromParent();
    bodys.erase(bodys.begin() + (bodys.size()-1));
    
    b = bodys.at(bodys.size()-1);
    b->changeEnd();
}

void GBubbleSprite::removeAllBody()
{
    for(int i=0;i<bodys.size();i++)
    {
        GBodySprite* b = bodys.at(i);
        b->removeFromParent();
    }
    bodys.clear();
}

void GBubbleSprite::playRelive()
{
    auto sp = Sprite::create();
    sp->setPosition(this->getPosition());
    this->getParent()->addChild(sp,10);
    sp->runAction(Sequence::create(GTools::createAnimate("tx-fuhuo", 9, 0.1f),
                                   RemoveSelf::create(),
                                   NULL));
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
    bubble->x = this->getUpdatePosition().x;
    bubble->y = this->getUpdatePosition().y;
    currDir = getCurrDir();
    bubble->dirX = currDir.x;
    bubble->dirY = currDir.y;
    bubble->rotate = getUpdateRotation();
    
}

void GBubbleSprite::updateRotation(float rotate)
{
    body->setRotation(rotate);
}
float GBubbleSprite::getUpdateRotation()
{
    return body->getRotation();
}

void GBubbleSprite::coll()
{
    if(this->isColl)
        return;
    this->isColl = true;
    this->runAction(Sequence::create(DelayTime::create(2),
                                     CallFunc::create(CC_CALLBACK_0(GBubbleSprite::collEnd, this)),
                                     NULL));
}

void GBubbleSprite::collEnd()
{
    this->isColl = false;
}

void GBubbleSprite::recoverHp()
{
    this->stopActionByTag(ACTION_RECHP);
//    if(bubble->currHp < bubble->HP)
//    {
//        auto ac = Sequence::create(DelayTime::create(bubble->recoverCD),
//                                   CallFunc::create(CC_CALLBACK_0(GBubbleSprite::recoverHpEnd, this)), NULL);
//        ac->setTag(ACTION_RECHP);
//        this->runAction(ac);
//    }
}

void GBubbleSprite::recoverHpEnd()
{
//    GModeGame::updateHp(bubble, 1);
    recoverHp();
}

void GBubbleSprite::reduceHp()
{
    this->stopActionByTag(ACTION_REDHP);
//    if(bubble->currHp > bubble->HP*0.1f)
//    {
//        auto ac = Sequence::create(DelayTime::create(bubble->reduceCD),
//                                   CallFunc::create(CC_CALLBACK_0(GBubbleSprite::reduceHpEnd, this)), NULL);
//        ac->setTag(ACTION_REDHP);
//        this->runAction(ac);
//    }
//    else{
//        if(isSpeedUp)
//        {
//            speedUpEnd();
//        }
//    }
}

void GBubbleSprite::reduceHpEnd()
{
//    GModeGame::updateHp(bubble, 0);
    reduceHp();
}

void GBubbleSprite::updatePos(float x,float y,float rotate,float time,bool up)
{
//    bubble->resetPosDt = 0;
//    Vec2 p(x,y);
//    Vec2 v = p - this->getPosition();
//    
//    for(int i=0;i<points.size();i++)
//    {
//        points.at(i).p += v;
//    }
//    this->setPosition(p);
    vecs.clear();
    Vec2 p(x,y);
    bool b = true;
    this->angle = rotate;

//    updateRotation(rotate);
    
    if(isCheck && !up && points.size())
    {
        this->isCheck = false;
        std::vector<GPath> ps;
        for (int i = 0; i < points.size()-1; i++) {
            GPath p1 = points.at(i);
            GPath p2 = points.at(i+1);
            
            ps.push_back(p1);
            Vec2 p = (p2.p - p1.p).getNormalized() * p2.p.getDistance(p1.p) / 2;
            ps.push_back(GPath(p+p1.p,p2.angle));
        }
        ps.push_back(points.at(points.size()-1));
        points.clear();
        for (int i=0; i<ps.size(); i++)
        {
            points.push_back(ps.at(i));
        }
    }
    

    for(int i=1;i<=1000;i++)
    {
        int index = (int)points.size() - i;
        if(index>=0)
        {
            float dis = p.getDistance(points.at(index).p);
            if(dis < 2)
            {
                b = false;
                break;
            }
        }
        else
        {
            break;
        }
    }
    if(b)
    {
        isDelay = false;
        if(this->isSpeedUp != up)
        {
            if(up)
            {
                if(bubble->exp > 0)
                {
                    this->isSpeedUp = true;
                    std::vector<GPath> ps;
                    for (int i=0; i<points.size(); i+=2) {
                        ps.push_back(points.at(i));
                    }
                    points.clear();
                    for (int i=0; i<ps.size(); i++) {
                        points.push_back(ps.at(i));
                    }
                }
            }
            else
            {
                this->isSpeedUp = false;
                std::vector<GPath> ps;
                for (int i = 0; i < points.size()-1; i++) {
                    GPath p1 = points.at(i);
                    GPath p2 = points.at(i+1);
                    
                    ps.push_back(p1);
                    Vec2 p = (p2.p - p1.p).getNormalized() * p2.p.getDistance(p1.p) / 2;
                    ps.push_back(GPath(p+p1.p,p2.angle));
                }
                ps.push_back(points.at(points.size()-1));
                points.clear();
                for (int i=0; i<ps.size(); i++)
                {
                    points.push_back(ps.at(i));
                }
                
                for(int i=0;i<bodys.size();i++)
                {
                    bodys.at(i)->showSpeedEnd();
                }
            }
        }
        
        if(isSpeedUp)
            changeState(GBubble::State::SPEEDUP);
        else
            changeState(GBubble::State::MOVE);
        
        vecs.push_back(GPath2(p,rotate,time));
//        body->stopActionByTag(ACTION_ROTATE);
//        Action *actionRo = RotateTo::create(time, rotate);
//        actionRo->setTag(ACTION_ROTATE);
//        body->runAction(actionRo);
        float dis = this->getPosition().getDistance(p);
        
        if(dis > 70)
        {
            isCheck = true;
            if(!isSpeedUp)
            {
                std::vector<GPath> ps;
                for (int i=0; i<points.size(); i+=2) {
                    ps.push_back(points.at(i));
                }
                points.clear();
                for (int i=0; i<ps.size(); i++) {
                    points.push_back(ps.at(i));
                }
            }
            log("isCheck %d",(int)vecs.size());
            log("%f",dis);
        }
        else
        {
            isCheck = false;
        }
    }
}
void GBubbleSprite::move3()
{
    if(vecs.size()>0)
    {
        Action* a = this->getActionByTag(ACTION_MOVE);
        if(a && !a->isDone())
        {
            return;
        }
        this->stopActionByTag(ACTION_MOVE);
//        this->stopActionByTag(ACTION_SPEEDUP);
        GPath2 p = vecs.at(0);
        
//        float time = p.p.getDistance(this->getPosition()) / (bubble->speed*1.15f);
        Vec2 dir = (p.p-this->getPosition()).getNormalized();
        Vec2 v = p.p + dir * bubble->speed*0.1f;
        auto ac = Sequence::create(MoveTo::create(p.time+0.1f, v),
//                                   CallFunc::create(CC_CALLBACK_0(GBubbleSprite::move3, this)),
                                   NULL) ;
        ac->setTag(ACTION_MOVE);
        this->runAction(ac);
        updateRotation(p.angle);
        
        vecs.erase(vecs.begin());
    }
//    else
//    {
//        Vec2 dir = getCurrDir();
//        dir = dir * bubble->speed + this->getPosition();
//        auto ac = Sequence::create(MoveTo::create(1, dir),
//                                   CallFunc::create(CC_CALLBACK_0(GBubbleSprite::move3, this)),
//                                   NULL) ;
//        ac->setTag(ACTION_SPEEDUP);
//        this->runAction(ac);
//        log("%d",random(0, 1000));
//    }
}

float GBubbleSprite::getCollAndBlock()
{
//    float r = MAX(this->getContentSize().width/2,this->getContentSize().height/2)*4.0f;
    return  120;
}

Vec2 GBubbleSprite::getBulletPosition(int type)
{
    Node* bulletLayer = this->getParent()->getParent()->getChildByName("bulletLayer");
    Vec2 v;
    if(type == 6)
    {
        v = body->getParent()->convertToWorldSpace(body->getPosition())-bulletLayer->getParent()->getPosition();
    }
    else
    {
//         v = body->getParent()->convertToWorldSpace(body->getPosition())-bulletLayer->getParent()->getPosition();
        v = body->convertToWorldSpace(tou->getPosition())-bulletLayer->getParent()->getPosition();
    }
    return v;
}
