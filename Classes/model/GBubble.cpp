//
//  GBubble.cpp
//  OnlineGame
//
//  Created by guang on 16/10/2.
//
//

#include "GBubble.h"
#include "data/GCache.h"

USING_NS_CC;

GBubble* GBubble::create()
{
    GBubble* bubble = new GBubble();
    bubble->autorelease();
    return bubble;
}


GBubble* GBubble::create(GJsonObject* obj)
{
    GBubble* bubble = new GBubble();
    
    bubble->uid = obj->getString("uid");
    bubble->name = obj->getString("name");
    std::string state = obj->getString("state");
    bubble->speed = obj->getDouble("speed");
    bubble->accelerated = obj->getDouble("accelerated");
    bubble->sSpeed = obj->getDouble("sSpeed");
    bubble->sAccelerated = obj->getDouble("sAccelerated");
    bubble->sCD = obj->getDouble("sCD");
    bubble->sDistance = obj->getInt("sDistance");
    bubble->limitSpeed = obj->getDouble("limitSpeed");
    bubble->limiRradius = obj->getDouble("limiRradius");
    bubble->rotateSpeed = obj->getDouble("rotateSpeed");
    bubble->expendHP = obj->getDouble("expendHP");
    
    bubble->x = obj->getDouble("x");
    bubble->y = obj->getDouble("y");
    bubble->rotate = obj->getDouble("rotate");
    bubble->dirX = obj->getDouble("dirX");
    bubble->dirY = obj->getDouble("dirY");
    
    bubble->initHp = obj->getDouble("initHp");
    bubble->HP = obj->getInt("HP");
    bubble->currHp = obj->getInt("currHp");
    bubble->level = obj->getInt("level");
    bubble->exp = obj->getInt("exp");
    bubble->grow = obj->getDouble("grow");
    bubble->kill = obj->getInt("kill");
    bubble->die = obj->getInt("die");
    bubble->recoverHP = obj->getInt("recoverHP");
    bubble->recoverCD = obj->getDouble("recoverCD");
    bubble->reduceHP = obj->getInt("reduceHP");
    bubble->reduceCD = obj->getDouble("reduceCD");
    
    bubble->robot = obj->getBool("robot");
    bubble->bubbleId = obj->getInt("bubbleId");
    bubble->skinId = obj->getInt("skinId");
    
    GJsonArray* robotUids = obj->getArray("robotUid");
    std::vector<std::string> vec_robotUid;
    for(int i=0;i<robotUids->size();i++)
    {
        GJsonObject* robotJson = robotUids->at(i);
        std::string buff = robotJson->toString();
        buff = buff.substr(1,buff.length()-2);
        vec_robotUid.push_back(buff);
    }
    
    bubble->state = bubble->toState(state);
    bubble->robotUid = vec_robotUid;
    
    bubble->isf = true;
    bubble->autorelease();
    return bubble;
}

GBubble::State GBubble::toState(std::string &state)
{
    GBubble::State s = GBubble::State::BORN;
    if(state == "IDLE")
        s = GBubble::State::IDLE;
    else if(state == "MOVE")
        s = GBubble::State::MOVE;
    else if(state == "SPEEDUP")
        s = GBubble::State::SPEEDUP;
    else if(state == "DIE")
        s = GBubble::State::DIE;
    return s;
}

