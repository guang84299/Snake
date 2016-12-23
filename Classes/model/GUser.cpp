//
//  GUser.cpp
//  OnlineGame
//
//  Created by guang on 16/10/1.
//
//

#include "GUser.h"

GUser* GUser::create()
{
    GUser * user = new GUser();
    user->autorelease();
    return user;
}

GUser* GUser::create(GJsonObject* obj)
{
    GUser * user = new GUser();
    
    user->uid = obj->getString("uid");
    user->name = obj->getString("name");
    user->password = obj->getString("password");
    user->serverId = obj->getString("serverId");
    user->serverIp = obj->getString("serverIp");
    user->visitor = obj->getBool("visitor");
    user->maxKill = obj->getInt("maxKill");
    user->cumKill = obj->getInt("cumKill");
    user->maxLen = obj->getInt("maxLen");
    user->skinNum = obj->getInt("skinNum");
    user->mvp = obj->getInt("mvp");
    user->headId = obj->getInt("headId");
    user->crystal = obj->getInt("crystal");
    user->skins = obj->getString("skins");
    user->skinId = obj->getInt("skinId");
    
    user->login = !user->visitor;
    
    user->autorelease();
    return user;
}

void GUser::updateData(GUser* old,GUser* news)
{
    old->name = news->name;
    old->visitor = news->visitor;
    old->maxKill = news->maxKill;
    old->cumKill = news->cumKill;
    old->maxLen = news->maxLen;
    old->skinNum = news->skinNum;
    old->mvp = news->mvp;
    old->headId = news->headId;
    old->skinNumRank = news->skinNumRank;
    old->cumKillRank = news->cumKillRank;
    old->mvpRank = news->mvpRank;
    old->maxKillRank = news->maxKillRank;
    old->crystal = news->crystal;
    old->skins = news->skins;
    old->skinId = news->skinId;
    
    old->login = !news->visitor;
}
