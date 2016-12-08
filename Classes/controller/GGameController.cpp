//
//  GGameController.cpp
//  OnlineGame
//
//  Created by guang on 16/10/3.
//
//

#include "GGameController.h"
#include "data/GJson.h"
#include "data/GCache.h"
#include "sprite/GBulletSprite.h"
#include "protocol_model/GModeGame.h"
#include "tools/GTools.h"
#include "common/GCommon.h"

USING_NS_CC;

static GGameController* _instance = nullptr;

GGameController* GGameController::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new GGameController();
        _instance->retain();
    }
    return _instance;
}

GGameController::~GGameController()
{
    this->scene = nullptr;
}

void GGameController::init(GGameScene *scene)
{
    this->scene = scene;
}

void GGameController::start(const char* data)
{
    GJsonObject* obj = GJsonObject::create(data);
    std::string uid = obj->getString("uid");
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        bubble->changeState(GBubble::State::IDLE);
    }
    else
    {
        GBubbleSprite* bubble = findByUid(uid);
        if(bubble)
        {
            bubble->changeState(GBubble::State::IDLE);
        }
    }
    scene->start();
    log("start game");
}

void GGameController::joinRoom(const char* data)
{
    GJsonObject* obj = GJsonObject::create(data);
    GJsonObject* bubble_data = obj->getObject("bubble");
    GBubbleSprite* bubble = GBubbleSprite::create(GBubble::create(bubble_data));
    this->bubbles.push_back(bubble);
    
    scene->joinRoom(bubble);
}

void GGameController::roomTimeOut(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    bubble->changeState(GBubble::State::DIE);
    scene->openGameOver(obj);
}

void GGameController::leaveRoom(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    std::string uid = obj->getString("uid");
    for(int i=0;i<bubbles.size();i++)
    {
        GBubbleSprite* ps = bubbles.at(i);
        if(ps->bubble->uid == uid)
        {
            if(uid == GCache::getInstance()->getUser()->uid)
            {
                scene->leaveRoom();
            }
            else
            {
                ps->die();
            }
            
            break;
        }
    }
}

void GGameController::questLeaveRoom(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    std::string uid = obj->getString("uid");
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        scene->leaveRoom();
    }
    else
    {
        GBubbleSprite* ps = findByUid(uid);
        if(ps)
        {
             ps->die();
        }
    }
}

void GGameController::move(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    float dirX = obj->getDouble("dirX");
    float dirY = obj->getDouble("dirY");
    float x = obj->getDouble("x");
    float y = obj->getDouble("y");
    float power = obj->getDouble("power");
    float rotate = obj->getDouble("rotate");
    std::string uid = obj->getString("uid");
    bool robot = obj->getBool("robot");
    bool speedUp = obj->getBool("speedUp");
    bool isSelf = false;
    Vec2 dir(dirX,dirY);
    Vec2 pos(x,y);
//    Vec2 lastDir(obj->getDouble("lastDirX"),obj->getDouble("lastDirY"));
//    float resetPosDt = obj->getDouble("resetPosDt");
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        isSelf = true;
    }
//    bool isselfRobot = false;
//    if(robot)
//    {
//        GBubbleSprite* bubble = findRobotByUid(uid);
//        isselfRobot = isSelfBubble(bubble->bubble);
//    }
//    if(isSelf || isselfRobot)
    scene->move(isSelf,dir,power,pos,rotate, uid,robot,speedUp);
}

void GGameController::stopMove(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    float x = obj->getDouble("x");
    float y = obj->getDouble("y");
    float rotate = obj->getDouble("rotate");
    std::string uid = obj->getString("uid");
    bool speedUp = obj->getBool("speedUp");
    bool isSelf = false;

    if(speedUp)
    {
        int hp = obj->getInt("hp");
        GBubbleSprite* bubble = nullptr;
        if(uid == GCache::getInstance()->getUser()->uid)
        {
            bubble = this->bubble;
        }
        else{
            bubble = findByUid(uid);
            if(!bubble)
                bubble = findRobotByUid(uid);
        }
        if(bubble)
        {
//            bubble->setPosition(x, y);
//            bubble->updateRotation(rotate);
            bubble->bubble->currHp = hp;
            bubble->changeState(GBubble::State::IDLE);
            bubble->updateHpBar();
            bubble->speedUpEnd();
        }
    }
    
    Vec2 pos(x,y);
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        isSelf = true;
    }
    scene->stopMove(isSelf,pos,rotate, uid,speedUp);
}

void GGameController::updateCountDown(int time)
{
    scene->updateCountDown(time);
}

void GGameController::addBlock(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    int x = obj->getInt("x");
    int y = obj->getInt("y");
    GJsonArray* arr = obj->getArray("list");
    for (int i=0; i<arr->size(); i++) {
        GJsonObject* val = arr->at(i);
        
        GBlock* cry = GBlock::create(val);
        
        GBlockSprite* sp = GBlockSprite::create(cry);
        this->blocks.push_back(sp);
        scene->addBlock(sp,x,y);
    }
}

void GGameController::eatBlock(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    int _id = obj->getInt("id");
    int type = obj->getInt("type");
    bool robot = obj->getBool("robot");
    std::string uid = obj->getString("uid");
    GBubbleSprite* sp = nullptr;
    bool isSelf = false;
    if(robot)
    {
        sp = findRobotByUid(uid);
    }
    else
    {
        if(uid == GCache::getInstance()->getUser()->uid)
        {
            sp = bubble;
            isSelf = true;
        }
        else{
            sp = findByUid(uid);
        }
    }
    

    GBlockSprite* ps = findBlock(_id);
    if(ps)
    {
        ps->die(sp);
    }
    if(sp)
    {
        if(type == 0)
        {
            int level = obj->getInt("level");
            int exp = obj->getInt("exp");
            float grow = obj->getDouble("grow");
            if(sp)
            {
                sp->bubble->level = level;
                sp->bubble->exp = exp;
                sp->bubble->grow = grow;
                sp->updateExp();
            }
        }
        else if(type == 1)
        {
            int bulletId = obj->getInt("bulletId");
            if(bulletId > 1)
            {
                sp->changeSkillBullet(bulletId);
            }
        }
        else if(type == 2)
        {
            int currHp = obj->getInt("currHp");
            sp->bubble->currHp = currHp;
            sp->updateHpBar();
            if(isSelf)
            GTools::playSound(SOUND_GETHP);
        }
    }
}

void GGameController::attack(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    std::string uid = obj->getString("uid");
    bool robot = obj->getBool("robot");
    std::string tids = obj->getString("tids");
    int bulletId = obj->getInt("bulletId");
    int tid = obj->getInt("tid");
    float dirX = obj->getDouble("dirX");
    float dirY = obj->getDouble("dirY");
    
    GBulletSprite* bu = nullptr;
    GBubbleSprite* sp = nullptr;
    if(bulletId == 6)//爆裂弹类型的子弹发出的攻击
    {
        bu = findBullet(tid);
        if(!bu)
            return;
    }
    else
    {
        if(robot)
        {
            sp = findRobotByUid(uid);
        }
        else
        {
            if(uid == GCache::getInstance()->getUser()->uid)
            {
                sp = bubble;
            }
            else{
                sp = findByUid(uid);
            }
        }
        if(!sp || sp->bubble->state == GBubble::State::DIE)
            return;
    }
    std::vector<std::string> vec = GTools::subString(tids, ",");
    for(int i=0;i<vec.size();i++)
    {
        std::string uid_str = vec.at(i);
        GBullet* bulletData = GCache::getInstance()->getBullet(bulletId);
        int tid = std::atoi(uid_str.c_str());
        if(bulletData)
        {
            bulletData = GBullet::create(bulletData);
            bulletData->tid = tid;
            bulletData->uid = uid;
            Vec2 dir(dirX,dirY);
            float num = ceil(i/2.f);
            float angle = 3.14f/bulletData->onceNum*2;
            if(bulletData->_id==1)
            {
                angle = 3.14f/180*bulletData->fireAngle;
            }
            else if(bulletData->_id==6)
            {
                dir.rotate(Vec2::ZERO, 3.14f/180*((int)bu->getUpdateRotation()%360));
            }
            if(i%2 == 0)
            {
                dir.rotate(Vec2::ZERO, angle*num);
            }
            else
            {
                dir.rotate(Vec2::ZERO, -angle*num);
            }
            
            GBulletSprite* bullet = GBulletSprite::create(bulletData, dir);
            //            if(isSelfBullet(bullet->bullet))
            bullets.push_back(bullet);
            //攻击
            if(bulletId == 6)
            {
                bu->attack(bullet);
            }
            else
            {
                sp->attack(bullet);
            }
        }
        
    }
}

void GGameController::bulletCollision(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
//    std::string uid = obj->getString("uid");
    std::string targetUid = obj->getString("target");
    bool target_robot = obj->getBool("target_robot");
    int tid = obj->getInt("tid");
    int bulletType = 1;
    //找到tid子弹
    GBulletSprite* bullet = findBullet(tid);
    bool selfBullet = false;
    if(bullet)
    {
        selfBullet = isSelfBullet(bullet->bullet);
        bulletType = bullet->bullet->type;
        if(bulletType == 1 || bulletType == 2 || bulletType == 3 || bulletType == 5)
            bullet->die();
    }
    
    if(targetUid == std::string("0"))
    {
        return;
    }
    
    GBubbleSprite* target = nullptr;
    bool isSelf = false;
    if(target_robot)
    {
        target = findRobotByUid(targetUid);
    }
    else
    {
        if(targetUid == GCache::getInstance()->getUser()->uid)
        {
            target = bubble;
            isSelf = true;
        }
        else{
            target = findByUid(targetUid);
        }
    }
    
    if(target)
    {
        if(bulletType == 6)
        {
            float downSpeed = bullet->bullet->downSpeed;
            float buffTime = bullet->bullet->buffTime;
            target->speedDown(downSpeed, buffTime);
        }
        int currHp = obj->getInt("currHp");
        bool die = obj->getBool("die");
        
        target->bubble->currHp = currHp;
        target->updateHpBar();
        if(isSelf || (target_robot && isSelfBubble(target->bubble)))
        {
            target->recoverHp();
        }
        if(bullet)
        {
            if(bulletType == 1 || bulletType == 2 || bulletType == 3 || bulletType == 5)
                target->hurt(bullet->dir, 0);
        }
        if(die)
        {
            if(target->bubble->state != GBubble::State::DIE)
            {
                target->die();
                if(isSelf)
                {
                    GJsonObject* dieData = obj->getObject("dieData");
                    scene->openRelived(dieData);
                }
                else
                {
                    if(selfBullet)
                    {
                        GTools::playSound(SOUND_BUTTON);
                    }
                }
            }
        }
    }
}

void GGameController::bulletChangeTarget(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
//    std::string uid = obj->getString("uid");
    std::string targetUid = obj->getString("target");
    bool target_robot = obj->getBool("target_robot");
    int _id = obj->getInt("id");
    
    GBubbleSprite* target = nullptr;
    if(target_robot)
    {
        target = findRobotByUid(targetUid);
    }
    else
    {
        if(targetUid == GCache::getInstance()->getUser()->uid)
        {
            target = bubble;
        }
        else{
            target = findByUid(targetUid);
        }
    }
    
    if(target)
    {
        //找到id子弹
        GBulletSprite* bullet = findBullet(_id);
        if(bullet)
        {
            Vec2 dir;
            bullet->changeTarget(dir);
        }
    }
}

void GGameController::speedUp(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    float x = obj->getDouble("x");
    float y = obj->getDouble("y");
    float rotate = obj->getDouble("rotate");
    std::string uid = obj->getString("uid");
    int hp = obj->getInt("hp");
    GBubbleSprite* bubble = nullptr;
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        bubble = this->bubble;
    }
    else{
        bubble = findByUid(uid);
    }
    if(bubble)
    {
        bubble->setPosition(x, y);
        bubble->updateRotation(rotate);
        bubble->bubble->currHp = hp;
        bubble->changeState(GBubble::State::SPEEDUP);
        bubble->updateHpBar();
        bubble->speedUp();
    }
    
}

void GGameController::stopSpeedUp(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    float x = obj->getDouble("x");
    float y = obj->getDouble("y");
    float rotate = obj->getDouble("rotate");
    std::string uid = obj->getString("uid");
    int hp = obj->getInt("hp");
    GBubbleSprite* bubble = nullptr;
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        bubble = this->bubble;
    }
    else{
        bubble = findByUid(uid);
        if(!bubble)
            bubble = findRobotByUid(uid);
    }
    if(bubble)
    {
        bubble->setPosition(x, y);
        bubble->updateRotation(rotate);
        bubble->bubble->currHp = hp;
        bubble->changeState(GBubble::State::IDLE);
        bubble->updateHpBar();
        bubble->speedUpEnd();
    }

}

void GGameController::allotRobot(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    std::string uid = obj->getString("uid");
    std::string robotUid = obj->getString("robotUid");
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        bubble->bubble->robotUid.push_back(robotUid);
    }
}

void GGameController::addRobot(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
//    std::string uid = obj->getString("uid");
    GJsonObject* robot = obj->getObject("robot");
    GBubble *bubble = GBubble::create(robot);
    GBubbleSprite* bubblesp = GBubbleSprite::create(bubble);
    this->robots.push_back(bubblesp);
    scene->joinRoom(bubblesp);
}

void GGameController::ranking(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    GJsonArray* arr = obj->getArray("ranks");
    if(arr)
    {
        scene->updateRank(arr);
    }
}

void GGameController::relived(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    GJsonObject* bubble = obj->getObject("bubble");
    GBubbleSprite* sp = GBubbleSprite::create(GBubble::create(bubble));
    sp->retain();
    this->bubble = sp;
    scene->reloved();
}

void GGameController::coll(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    float x = obj->getDouble("x");
    float y = obj->getDouble("y");
    float collx = obj->getDouble("collx");
    float colly = obj->getDouble("colly");
    float rotate = obj->getDouble("rotate");
    std::string uid = obj->getString("uid");
    bool isSelf = false;
    Vec2 pos(x,y);
    Vec2 collPos(collx,colly);
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        isSelf = true;
    }
    scene->coll(isSelf,pos,collPos,rotate, uid);
}

void GGameController::updatePos(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    
    float x = obj->getDouble("x");
    float y = obj->getDouble("y");
    float rotate = obj->getDouble("rotate");
    int state = obj->getInt("state");
    std::string uid = obj->getString("uid");
    
    if(uid != GCache::getInstance()->getUser()->uid)
    {
        GBubbleSprite* target = findRobotByUid(uid);
        if(!target)
        {
            target = findByUid(uid);
        }
        else
        {
            //是机器人的话，需要判断是否是自己的
            if(isSelfBubble(target->bubble))
                return;
        }
        if(target)
        {
            target->updatePos(x, y, rotate,  state);
        }
    }
}

void GGameController::updateHp(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    
    int currHp = obj->getInt("currHp");
    std::string uid = obj->getString("uid");
    
    GBubbleSprite* target = nullptr;
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        target = this->bubble;
    }
    else
    {
        target = findRobotByUid(uid);
        if(!target)
        {
            target = findByUid(uid);
        }
    }

    if(target)
    {
        target->bubble->currHp = currHp;
        target->updateHpBar();
    }
}

bool GGameController::isSelfBullet(GBullet* bullet)
{
    if(GCache::getInstance()->getUser()->uid == bullet->uid)
    {
        return true;
    }
    for(int i=0;i<bubble->bubble->robotUid.size();i++)
    {
        if(bubble->bubble->robotUid.at(i) == bullet->uid)
        {
            return true;
        }
    }
    return false;
}

bool GGameController::isSelfBubble(GBubble* bubble)
{
    if(GCache::getInstance()->getUser()->uid == bubble->uid)
    {
        return true;
    }
    for(int i=0;i<this->bubble->bubble->robotUid.size();i++)
    {
        if(this->bubble->bubble->robotUid.at(i) == bubble->uid)
        {
            return true;
        }
    }
    return false;
}

GBubble* GGameController::getSelfBubble()
{
    return bubble->bubble;
}

GBubbleSprite* GGameController::findByUid(std::string &uid)
{
    for(int i=0;i<bubbles.size();i++)
    {
        GBubbleSprite* p = bubbles.at(i);
        if(p->bubble->uid == uid)
        {
            return p;
        }
    }
    return nullptr;
}

GBubbleSprite* GGameController::findRobotByUid(std::string &uid)
{
    for(int i=0;i<robots.size();i++)
    {
        GBubbleSprite* p = robots.at(i);
        if(p->bubble->uid == uid)
        {
            return p;
        }
    }
    return nullptr;
}

GBulletSprite* GGameController::findBullet(int tid)
{
    for (int i=0; i<bullets.size(); i++) {
        GBulletSprite* b = bullets.at(i);
        if(b->bullet->tid == tid)
        {
            return b;
        }
    }
    return nullptr;
}

GBlockSprite* GGameController::findBlock(int _id)
{
    for (int i=0; i<blocks.size(); i++) {
        GBlockSprite* block = blocks.at(i);
        if(block->block->_id == _id)
        {
            return block;
        }
    }
    return nullptr;
}

void GGameController::deleteBlock(int _id)
{
    for (int i=0; i<blocks.size(); i++) {
        GBlockSprite* block = blocks.at(i);
        if(block->block->_id == _id)
        {
            blocks.erase(blocks.begin()+i);
            if(block->block->bulletType > 1 || block->block->type == 2)
            {
                scene->miniMap->remove(_id);
            }
            break;
        }
    }

}

void GGameController::deleteBullet(int tid)
{
    for (int i=0; i<bullets.size(); i++) {
        GBulletSprite* bullet = bullets.at(i);
        if(bullet->bullet->tid == tid)
        {
            bullets.erase(bullets.begin()+i);
            break;
        }
    }
}

void GGameController::deleteBubble(std::string &uid)
{
    for (int i=0; i<bubbles.size(); i++) {
        GBubbleSprite* bubble = bubbles.at(i);
        if(bubble->bubble->uid == uid)
        {
            bubbles.erase(bubbles.begin()+i);
            break;
        }
    }
    
    scene->miniMap->remove(uid);
}

void GGameController::deleteRobot(std::string &uid)
{
    for (int i=0; i<robots.size(); i++) {
        GBubbleSprite* bubble = robots.at(i);
        if(bubble->bubble->uid == uid)
        {
            robots.erase(robots.begin()+i);
            break;
        }
    }
    for(int i=0;i<bubble->bubble->robotUid.size();i++)
    {
        if(bubble->bubble->robotUid.at(i) == uid)
        {
            bubble->bubble->robotUid.erase(bubble->bubble->robotUid.begin()+i);
            break;
        }
    }
    scene->miniMap->remove(uid);
}
