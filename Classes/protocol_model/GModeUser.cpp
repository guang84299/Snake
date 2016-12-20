//
//  GModeUser.cpp
//  OnlineGame
//
//  Created by guang on 16/9/28.
//
//

#include "GModeUser.h"
#include "protocol/GData.h"
#include "data/GJson.h"
#include "protocol/GProtocol.h"
#include "network/GServer.h"
#include "controller/GUserController.h"
#include "data/GCache.h"
#include "sprite/GBubbleSprite.h"
#include "controller/GGameController.h"
#include "tools/GTools.h"
#include "scene/GScene.h"
#include "scene/GHomeScene.h"

using namespace std;
USING_NS_CC;

void GModeUser::login()
{
    GJsonObject* val = GJsonObject::create();
    val->putString("uid", GCache::getInstance()->getUser()->uid);
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_LOGIN, val2);
    GServer::getInstance()->send(data);
}

void GModeUser::loginResult(const char* data)
{
    GJsonObject* val = GJsonObject::create(data);
    if(val->getBool("result"))
    {
        //成功缓存公用数据
        int bulletId = val->getInt("bulletId");
        std::string exps = val->getString("exps");
        std::string grows = val->getString("grows");
        std::string skins = val->getArray("skins")->toString();
        int shareAward = val->getInt("shareAward");
        
        std::vector<GBullet*> bullets;
        GJsonArray* arr = val->getArray("bullets");
        for(int i=0;i<arr->size();i++)
        {
            GBullet* bullet = GBullet::create(arr->at(i));
            bullets.push_back(bullet);
        }
        GCache::getInstance()->setBulletId(bulletId);
        GCache::getInstance()->setBullets(bullets);
        GCache::getInstance()->setExps(exps);
        GCache::getInstance()->setGrows(grows);
        GCache::getInstance()->setSkins(skins);
        GCache::getInstance()->setShareAward(shareAward);
        
        GUserController::getInstance()->enterHomeScene();
        log("login success");
    }
    else
    {
        log("login fail");
    }
}

static long lt = 0;
void GModeUser::heartBeat()
{
    GJsonObject* val = GJsonObject::create();
    val->putString("heartBeat", "heartBeat");
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_HERTBEAT, val2);
    GServer::getInstance()->send(data);
    
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time_sec = ((long long)tv.tv_sec) * 1000+ tv.tv_usec / 1000;
    lt = time_sec;
}

void GModeUser::heartBeatResult(const char* data)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long time_sec = ((long long)tv.tv_sec) * 1000+ tv.tv_usec / 1000;
    
    int ping = (int)(time_sec-lt);
    
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    if(sc)
    {
        sc->updatePing(ping);
    }
}

void GModeUser::enterRoom()
{
    GJsonObject* val = GJsonObject::create();
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_ENTERROOM, val2);
    GServer::getInstance()->send(data);
}

void GModeUser::enterRoomResult(const char *data)
{
    GJsonObject* val = GJsonObject::create(data);
    bool result = val->getBool("result");
    std::string uid = val->getString("uid");
    if(result)
    {
        if(uid == GCache::getInstance()->getUser()->uid)
        {
            GGameController::getInstance()->blocks.clear();
            GGameController::getInstance()->clounds.clear();
            GGameController::getInstance()->robots.clear();
            GGameController::getInstance()->bubbles.clear();
            GGameController::getInstance()->bullets.clear();
            
            for(int i=0;i<GCache::getMapPlitNum()+4;i++)
            {
                std::vector<GBlockSprite*> blocks1;
                GGameController::getInstance()->blocks.push_back(blocks1);
            }
   
            int roomWidth = val->getInt("roomWidth");
            int roomHeight = val->getInt("roomHeight");
            int mapId = val->getInt("mapId");
            float mapPosX = val->getDouble("mapPosX");
            float mapPosY = val->getDouble("mapPosY");
            GCache::getInstance()->setRoomWidth(roomWidth);
            GCache::getInstance()->setRoomHeight(roomHeight);
            GCache::getInstance()->setMapId(mapId);
            GCache::getInstance()->setMapPosX(mapPosX);
            GCache::getInstance()->setMapPosY(mapPosY);
            GCache::getInstance()->setKillMeUid("");
            //水晶需要分批加载
//            GJsonArray* blocks_data = val->getArray("blocks");
//            for(int i=0;i<blocks_data->size();i++)
//            {
//                GBlockSprite* sp = GBlockSprite::create(GBlock::create(blocks_data->at(i)));
//                sp->retain();
//                GGameController::getInstance()->blocks.push_back(sp);
//            }
            
//            GJsonArray* clounds_data = val->getArray("clounds");
//            for(int i=0;i<clounds_data->size();i++)
//            {
//                GCloundSprite* sp = GCloundSprite::create(GClound::create(clounds_data->at(i)));
//                sp->retain();
//                GGameController::getInstance()->clounds.push_back(sp);
//            }
            
            GJsonArray* robots_data = val->getArray("robots");
            for(int i=0;i<robots_data->size();i++)
            {
                GBubbleSprite* sp = GBubbleSprite::create(GBubble::create(robots_data->at(i)));
                sp->retain();
                GGameController::getInstance()->robots.push_back(sp);
            }
            
            GJsonArray* bubbles_data = val->getArray("bubbles");
            for(int i=0;i<bubbles_data->size();i++)
            {
                GBubbleSprite* sp = GBubbleSprite::create(GBubble::create(bubbles_data->at(i)));
                sp->retain();
                if(sp->bubble->uid != GCache::getInstance()->getUser()->uid)
                    GGameController::getInstance()->bubbles.push_back(sp);
                else
                {
                    GGameController::getInstance()->bubble = sp;
                }
            }
            GUserController::getInstance()->enterGameScene();
            GModeUser::addBlock();
        }        
        log("addBlock");
    }
    else
    {
        log("user enterRoom error!");
    }
}

void GModeUser::addBlock()
{
    GJsonObject* val = GJsonObject::create();
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_ADDBLOCK, val2);
    GServer::getInstance()->send(data);
}
void GModeUser::addBlockResult(const char* data)
{
    GJsonObject* val = GJsonObject::create(data);
    bool result = val->getBool("result");
    GJsonArray* blocks_data = val->getArray("list");
    
    for(int i=0;i<blocks_data->size();i++)
    {
        GBlockSprite* sp = GBlockSprite::create(GBlock::create(blocks_data->at(i)));
        sp->retain();
        GGameController::getInstance()->addBlock(sp);
    }
    if(result)
    {
        log("user enterRoom");
        if(GGameController::getInstance()->scene)
            GGameController::getInstance()->scene->resetInitBlock();
    }
}

void GModeUser::recConn(const char* type)
{
    GJsonObject* val = GJsonObject::create();
    val->putString("uid", GCache::getInstance()->getUser()->uid);
    val->putString("type", type);
    if(std::string(type) == std::string("game"))
    {
        GBubble* bubble = GGameController::getInstance()->bubble->bubble;
        val->putDouble("x", bubble->x);
        val->putDouble("y", bubble->y);
        val->putDouble("rotate", bubble->rotate);
        val->putDouble("dirX", bubble->dirX);
        val->putDouble("dirY", bubble->dirY);
//        val->putDouble("currHp", bubble->currHp);
    }
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_RECCONN, val2);
    GServer::getInstance()->send(data);
    log("GModeUser::recConn");
}

void GModeUser::recConnResult(const char *data)
{
    log("GModeUser::recConnResult");
    GJsonObject* val = GJsonObject::create(data);
    bool result = val->getBool("result");
    std::string type = val->getString("type");
    if(result)
    {
        if(type == std::string("game"))
        {
            GGameController::getInstance()->blocks.clear();
            GGameController::getInstance()->clounds.clear();
            GGameController::getInstance()->robots.clear();
            GGameController::getInstance()->bubbles.clear();
            GGameController::getInstance()->bullets.clear();
            
            for(int i=0;i<GCache::getMapPlitNum()+4;i++)
            {
                std::vector<GBlockSprite*> blocks1;
                GGameController::getInstance()->blocks.push_back(blocks1);
            }
            
//            GJsonArray* blocks_data = val->getArray("blocks");
//            for(int i=0;i<blocks_data->size();i++)
//            {
//                GBlockSprite* sp = GBlockSprite::create(GBlock::create(blocks_data->at(i)));
//                sp->retain();
//                GGameController::getInstance()->blocks.push_back(sp);
//            }
            
//            GJsonArray* clounds_data = val->getArray("clounds");
//            for(int i=0;i<clounds_data->size();i++)
//            {
//                GCloundSprite* sp = GCloundSprite::create(GClound::create(clounds_data->at(i)));
//                sp->retain();
//                GGameController::getInstance()->clounds.push_back(sp);
//            }
            
            GJsonArray* robots_data = val->getArray("robots");
            for(int i=0;i<robots_data->size();i++)
            {
                GBubbleSprite* sp = GBubbleSprite::create(GBubble::create(robots_data->at(i)));
                sp->retain();
                GGameController::getInstance()->robots.push_back(sp);
            }
            
            GJsonArray* bubbles_data = val->getArray("bubbles");
            for(int i=0;i<bubbles_data->size();i++)
            {
                GBubbleSprite* sp = GBubbleSprite::create(GBubble::create(bubbles_data->at(i)));
                sp->retain();
                if(sp->bubble->uid != GCache::getInstance()->getUser()->uid)
                    GGameController::getInstance()->bubbles.push_back(sp);
                else
                {
                    GGameController::getInstance()->bubble = sp;
                }
            }
            GModeUser::addBlock();
            if(GGameController::getInstance()->scene)
                GGameController::getInstance()->scene->resetInit();
        }
        
        else
        {
            
        }
    
    }
}

void GModeUser::updateName(std::string name)
{
    GJsonObject* val = GJsonObject::create();
    val->putString("name", name);
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_UPDATENAME, val2);
    GServer::getInstance()->send(data);
}

void GModeUser::updateNameResult(const char *data)
{
    GJsonObject* val = GJsonObject::create(data);
    bool result = val->getBool("result");
    std::string name = val->getString("name");
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    GHomeScene* home = dynamic_cast<GHomeScene*>(sc);
    if(result)
    {
        GCache::getInstance()->getUser()->name = name;
        GCache::getInstance()->getUser()->visitor = false;
        GTools::showTost(sc, _T("update success"));
        if(home)
        {
            home->updateUI();
        }
    }
    else
    {
        GTools::showTost(sc, _T("update fail"));
    }
}

void GModeUser::updateHead(int headId)
{
    GJsonObject* val = GJsonObject::create();
    val->putInt("headId", headId);
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_UPDATHEAD, val2);
    GServer::getInstance()->send(data);
}

void GModeUser::updateHeadResult(const char *data)
{
    GJsonObject* val = GJsonObject::create(data);
    bool result = val->getBool("result");
    int headId = val->getInt("headId");
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    GHomeScene* home = dynamic_cast<GHomeScene*>(sc);
    if(result)
    {
        GCache::getInstance()->getUser()->headId = headId;
        GTools::showTost(sc, _T("update success"));
        if(home)
        {
            home->updateUI();
        }
    }
    else
    {
        GTools::showTost(sc, _T("update fail"));
    }
}

void GModeUser::infos()
{
    GJsonObject* val = GJsonObject::create();
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_INFOS, val2);
    GServer::getInstance()->send(data);

}

void GModeUser::infosResult(const char *data)
{
    GJsonObject* val = GJsonObject::create(data);
    GJsonObject* userObj = val->getObject("user");
    
    GUser* user = GUser::create(userObj);
    user->skinNumRank = val->getDouble("skinNumRank");
    user->cumKillRank = val->getDouble("cumKillRank");
    user->mvpRank = val->getDouble("mvpRank");
    user->maxKillRank = val->getDouble("maxKillRank");
    
    GUser::updateData(GCache::getInstance()->getUser(), user);
}

void GModeUser::buySkin(int skinId)
{
    GJsonObject* val = GJsonObject::create();
    val->putInt("skinId", skinId);
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_BUYSKIN, val2);
    GServer::getInstance()->send(data);
}
void GModeUser::buySkinResult(const char* data)
{
    GJsonObject* val = GJsonObject::create(data);
    bool result = val->getBool("result");
    int crystal = val->getInt("crystal");
    int skinNum = val->getInt("skinNum");
    std::string skins = val->getString("skins");
    
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    GHomeScene* home = dynamic_cast<GHomeScene*>(sc);
    if(result)
    {
        GCache::getInstance()->getUser()->crystal = crystal;
        GCache::getInstance()->getUser()->skinNum = skinNum;
        GCache::getInstance()->getUser()->skins = skins;
        GTools::showTost(sc, _T("buy success"));
        if(home)
        {
            home->updateUI();
        }
    }
    else
    {
        GTools::showTost(sc, _T("buy fail"));
    }
}
void GModeUser::useSkin(int skinId)
{
    GJsonObject* val = GJsonObject::create();
    val->putInt("skinId", skinId);
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_USESKIN, val2);
    GServer::getInstance()->send(data);
}
void GModeUser::useSkinResult(const char* data)
{
    GJsonObject* val = GJsonObject::create(data);
    bool result = val->getBool("result");
    int skinId = val->getInt("skinId");
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    GHomeScene* home = dynamic_cast<GHomeScene*>(sc);
    if(result)
    {
        GCache::getInstance()->getUser()->skinId = skinId;
//        GTools::showTost(sc, _T("buy success"));
        if(home)
        {
            home->updateUI();
        }
    }
    else
    {
//        GTools::showTost(sc, _T("buy fail"));
    }

}

void GModeUser::share()
{
    GJsonObject* val = GJsonObject::create();
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_USER_SHARE, val2);
    GServer::getInstance()->send(data);
}
void GModeUser::shareResult(const char* data)
{
    GJsonObject* val = GJsonObject::create(data);
    bool result = val->getBool("result");
    int crystal = val->getInt("crystal");
    
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    GHomeScene* home = dynamic_cast<GHomeScene*>(sc);
    if(result)
    {
        GCache::getInstance()->getUser()->crystal = crystal;
        char c[7];
        sprintf(c, "%d",GCache::getInstance()->getShareAward());
        GTools::showTost(nullptr, _T("share success") + _T("share award") +c);
        if(home)
        {
            home->updateUI();
        }
    }
    else
    {
        GTools::showTost(sc, _T("share repeat"));
    }
}

void GModeUser::closeServer()
{
    GServer::getInstance()->close();
}
