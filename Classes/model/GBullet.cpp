//
//  GBullet.cpp
//  OnlineGame
//
//  Created by guang on 16/10/12.
//
//

#include "GBullet.h"

USING_NS_CC;

GBullet* GBullet::create(GJsonObject* obj)
{
    GBullet* bullet = new GBullet();
    
    bullet->state = State::IDLE;
    
    bullet->_id = obj->getInt("id");
    bullet->tid = obj->getInt("tid");
    bullet->name = obj->getString("name");
    bullet->bulletSpeed = obj->getDouble("bulletSpeed");
    bullet->bulletTime = obj->getDouble("bulletTime");
    bullet->bulletAccelerated = obj->getDouble("bulletAccelerated");
    bullet->rotateSpeed = obj->getDouble("rotateSpeed");
    bullet->damage = obj->getInt("damage");
    bullet->buffTime = obj->getDouble("buffTime");
    bullet->downSpeed = obj->getDouble("downSpeed");
    bullet->num = obj->getInt("num");
    bullet->range = obj->getInt("range");
    bullet->targetRange = obj->getInt("targetRange");
    bullet->CD = obj->getDouble("CD");
    bullet->fillBullet = obj->getDouble("fillBullet");
    bullet->loadBullet = obj->getDouble("loadBullet");
    bullet->description = obj->getString("description");
    
    bullet->type = obj->getInt("type");
    bullet->continueNum = obj->getInt("continueNum");
    bullet->onceNum = obj->getInt("onceNum");
    bullet->chanceFrom = obj->getDouble("chanceFrom");
    bullet->chanceTo = obj->getDouble("chanceTo");
    bullet->changeTargetTime = obj->getDouble("changeTargetTime");
    bullet->twoBulletId = obj->getInt("twoBulletId");
    bullet->damageCD = obj->getDouble("damageCD");
    bullet->splitCD = obj->getDouble("splitCD");
    bullet->fireAngle = obj->getInt("fireAngle");

    bullet->autorelease();
    return bullet;
}

GBullet* GBullet::create(GBullet* obj)
{
    GBullet* bullet = new GBullet();
    
    bullet->state = obj->state;
    
    bullet->_id = obj->_id;
    bullet->tid = obj->tid;
    bullet->name = obj->name;
    bullet->bulletSpeed = obj->bulletSpeed;
    bullet->bulletTime = obj->bulletTime;
    bullet->bulletAccelerated = obj->bulletAccelerated;
    bullet->rotateSpeed = obj->rotateSpeed;
    bullet->damage = obj->damage;
    bullet->buffTime = obj->buffTime;
    bullet->downSpeed = obj->downSpeed;
    bullet->num = obj->num;
    bullet->range = obj->range;
    bullet->targetRange = obj->targetRange;
    bullet->CD = obj->CD;
    bullet->fillBullet = obj->fillBullet;
    bullet->loadBullet = obj->loadBullet;
    bullet->description = obj->description;
    
    bullet->type = obj->type;
    bullet->continueNum = obj->continueNum;
    bullet->onceNum = obj->onceNum;
    bullet->chanceFrom = obj->chanceFrom;
    bullet->chanceTo = obj->chanceTo;
    bullet->changeTargetTime = obj->changeTargetTime;
    bullet->twoBulletId = obj->twoBulletId;
    bullet->damageCD = obj->damageCD;
    bullet->splitCD = obj->splitCD;
    bullet->fireAngle = obj->fireAngle;
    
    bullet->autorelease();
    return bullet;

}
