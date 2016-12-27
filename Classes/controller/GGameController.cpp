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
        _instance->scene = nullptr;
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

void GGameController::init2(GGameScene2 *scene2)
{
    this->scene2 = scene2;
}

void GGameController::start(const char* data)
{
    GJsonObject* obj = GJsonObject::create(data);
    std::string uid = obj->getString("uid");
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        bubble->changeState(GBubble::State::IDLE);
        scene->start();
    }
    else
    {
        GBubbleSprite* bubble = findByUid(uid);
        if(bubble)
        {
            bubble->changeState(GBubble::State::IDLE);
        }
    }
    log("start game");
}

void GGameController::joinRoom(const char* data)
{
    GJsonObject* obj = GJsonObject::create(data);
    
    bool clear = obj->getBool("clear");
    if(clear)
    {
        std::string uid = obj->getString("uid");
        GBubbleSprite* ps = findByUid(uid);
        if(ps)
        {
            ps->leave();
        }
    }
    
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
                ps->leave();
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
             ps->leave();
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
//            bubble->bubble->currHp = hp;
            bubble->changeState(GBubble::State::IDLE);
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
        addBlock(sp);
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
    float x = obj->getDouble("x");
    float y = obj->getDouble("y");
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
    

    GBlockSprite* ps = findBlock(_id,x,y);
    if(ps)
    {
        ps->die(sp);
    }
    if(sp)
    {
        if(type == 0 || type == 1)
        {
            int level = obj->getInt("level");
            int exp = obj->getInt("exp");
            float grow = obj->getDouble("grow");
            if(sp)
            {
                bool up = sp->bubble->level != level;
                sp->bubble->level = level;
                sp->bubble->exp = exp;
                sp->bubble->grow = grow;
                sp->updateExp(up,false);
            }
        }
        else if(type == 2)
        {
//            int currHp = obj->getInt("currHp");
//            sp->bubble->currHp = currHp;
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
//                sp->attack(bullet);
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
        target->die();
        scene->updateRobotEat();
        GJsonObject* dieData = obj->getObject("dieData");
        if(isSelf)
        {
            scene->openRelived(dieData);
        }
        else
        {
            int rank = dieData->getInt("rank");
            //先判断排名是否小于10
            if(rank <= 10)
            {
                std::string killName = target->bubble->name;
                std::string killMeName = dieData->getString("killMeName");
                std::string killMe = dieData->getString("killMe");
                //是否是自己击杀的
                if(killMe == GCache::getInstance()->getUser()->uid)
                {
                    int killNum = dieData->getInt("killNum");
                    if(killNum > 1 && killNum <= 5)
                    {
                        char c[7];
                        sprintf(c, "%d",killNum);
                        std::string t = "prompt_kill_";
                        t += c;
                        GTools::showTost2(nullptr, _T(t));
                    }
                    else if(killNum > 5)
                    {
                        GTools::showTost2(nullptr, _T("prompt_kill"));
                    }
                    else
                    GTools::showTost2(nullptr, _T("prompt_2")+killName);
                    
                    //判断是否是上次击杀自己的人
                    if(GCache::getInstance()->getKillMeUid() != ""
                       && GCache::getInstance()->getKillMeUid() == target->bubble->uid)
                    {
                        GCache::getInstance()->setKillMeUid("");
                        GTools::showTost(nullptr, _T("prompt_3"),0.7f);
                    }
                }
                else
                {
                    if(GCache::getInstance()->getLang() == "en")
                        GTools::showTost2(nullptr, killMeName+_T("prompt_1")+killName);
                    else
                        GTools::showTost2(nullptr, killName+_T("prompt_1")+killMeName);
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
//    int hp = obj->getInt("hp");
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
//        bubble->bubble->currHp = hp;
        bubble->changeState(GBubble::State::SPEEDUP);
//        bubble->updateHpBar();
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
//    int hp = obj->getInt("hp");
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
//        bubble->bubble->currHp = hp;
        bubble->changeState(GBubble::State::IDLE);
//        bubble->updateHpBar();
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
    GBubbleSprite* b = findRobotByUid(bubble->uid);
    if(b)
    {
        deleteRobot(bubble->uid);
    }
    GBubbleSprite* bubblesp = GBubbleSprite::create(bubble);
    bubblesp->retain();
    this->robots.push_back(bubblesp);
    scene->joinRoom(bubblesp);
    bubblesp->release();
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
    GJsonArray* arr = GJsonArray::create(data);
    for(int i=0;i<arr->size();i++)
    {
        GJsonObject* obj = arr->at(i);
        
        float x = obj->getDouble("x");
        float y = obj->getDouble("y");
//        float dirX = obj->getDouble("dirX");
//        float dirY = obj->getDouble("dirY");
        float rotate = obj->getDouble("rotate");
//        int state = obj->getInt("state");
        float time = obj->getDouble("time");
        std::string uid = obj->getString("uid");
        bool up = obj->getBool("up");
        
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
            target->updatePos(x, y, rotate,time,up);
        }
//        if(uid != GCache::getInstance()->getUser()->uid)
//        {
//            GBubbleSprite* target = findRobotByUid(uid);
//            if(!target)
//            {
//                target = findByUid(uid);
//            }
//            else
//            {
//                //是机器人的话，需要判断是否是自己的
//                if(isSelfBubble(target->bubble))
//                    return;
//            }
//            if(target)
//            {
//                if(target->bubble->resetPosDt>3 || !isContain(this->bubble, target))
//                    target->updatePos(x, y, rotate,  2);
//            }
//        }
    }
}

void GGameController::updateHp(const char *data)
{
    GJsonObject* obj = GJsonObject::create(data);
    
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
        int level = obj->getInt("level");
        int exp = obj->getInt("exp");
        float grow = obj->getDouble("grow");
        
        bool up = target->bubble->level != level;
        target->bubble->level = level;
        target->bubble->exp = exp;
        target->bubble->grow = grow;
        target->updateExp(false,up);
//        target->bubble->currHp = currHp;
//        target->updateHpBar();
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

bool GGameController::isContain(GBubbleSprite* bubble,GBubbleSprite* target)
{
    Size s = Director::getInstance()->getWinSize();
    float r = sqrtf(s.width*s.width + s.height*s.height);
    if(bubble != target)
    {
        int num = (int)target->bodys.size();
        int i=0;
        if(i<num)
        {
            float dis = target->bodys.at(i)->getPosition().getDistance(bubble->getPosition());
            if(dis < r)
            {
                return true;
            }
        }
        i = (int)(num*0.3f);
        if(i<num)
        {
            float dis = target->bodys.at(i)->getPosition().getDistance(bubble->getPosition());
            if(dis < r)
            {
                return true;
            }
        }
        i = (int)(num*0.5f);
        if(i<num)
        {
            float dis = target->bodys.at(i)->getPosition().getDistance(bubble->getPosition());
            if(dis < r)
            {
                return true;
            }
        }
        i = (int)(num*0.8f);
        if(i<num)
        {
            float dis = target->bodys.at(i)->getPosition().getDistance(bubble->getPosition());
            if(dis < r)
            {
                return true;
            }
        }
        i = num-1;
        if(i<num && i>=0)
        {
            float dis = target->bodys.at(i)->getPosition().getDistance(bubble->getPosition());
            if(dis < r)
            {
                return true;
            }
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

GBlockSprite* GGameController::findBlock(int _id,float x,float y)
{
    float w = GCache::getInstance()->getRoomWidth()/GCache::getMapPlitNum();
    int row = x/w;
    
    for (int i=0; i<blocks[row].size(); i++) {
        GBlockSprite* block = blocks[row].at(i);
        if(block->block->_id == _id)
        {
            return block;
        }
    }
    return nullptr;
}

void GGameController::addBlock(GBlockSprite* block)
{
    float w = GCache::getInstance()->getRoomWidth()/GCache::getMapPlitNum();
    
    int row = block->block->x/w;
    
    blocks.at(row).push_back(block);
}

void GGameController::deleteBlock(int _id,float x,float y)
{
    float w = GCache::getInstance()->getRoomWidth()/GCache::getMapPlitNum();
    int row = x/w;

    for (int i=0; i<blocks[row].size(); i++) {
        GBlockSprite* block = blocks[row].at(i);
        if(block->block->_id == _id)
        {
            blocks[row].erase(blocks[row].begin()+i);
//            if(block->block->bulletType > 1 || block->block->type == 2)
//            {
//                scene->miniMap->remove(_id);
//            }
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
    if(scene)
        scene->miniMap->remove(uid);
    else if(scene2)
        scene2->miniMap->remove(uid);
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
    if(scene)
        scene->miniMap->remove(uid);
    else if(scene2)
        scene2->miniMap->remove(uid);
}
