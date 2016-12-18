//
//  GReflect.cpp
//  OnlineGame
//
//  Created by guang on 16/9/28.
//
//

#include "GReflect.h"
#include "GData.h"
#include "protocol_model/GModeUser.h"
#include "protocol_model/GModeGame.h"

using namespace std;
USING_NS_CC;

static GReflect* _instance = nullptr;

GReflect* GReflect::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new GReflect();
    }
    return _instance;
}

void GReflect::registerMethod(std::string name, GProtocol pro)
{
    maps.insert(map<string,GProtocol>::value_type(name,pro));
}

void GReflect::invoke(std::string name, const char* data)
{
    GProtocol pro = maps[name];
    if(pro != nullptr)
    {
        pro(data);
    }
    else
    {
        CCLOG("数据解析失败！");
    }
}

void GReflect::registerAll()
{
    registerMethod("GModeUser_loginResult",&GModeUser::loginResult);
    registerMethod("GModeUser_heartBeatResult",&GModeUser::heartBeatResult);
    registerMethod("GModeUser_enterRoomResult",&GModeUser::enterRoomResult);
    registerMethod("GModeUser_addBlockResult",&GModeUser::addBlockResult);
    registerMethod("GModeUser_recConnResult",&GModeUser::recConnResult);
    registerMethod("GModeUser_updateNameResult",&GModeUser::updateNameResult);
    registerMethod("GModeUser_updateHeadResult",&GModeUser::updateHeadResult);
    registerMethod("GModeUser_infosResult",&GModeUser::infosResult);
    registerMethod("GModeUser_buySkinResult",&GModeUser::buySkinResult);
    registerMethod("GModeUser_useSkinResult",&GModeUser::useSkinResult);
    registerMethod("GModeUser_shareResult",&GModeUser::shareResult);

    registerMethod("GModeGame_startGameResult",&GModeGame::startGameResult);
    registerMethod("GModeGame_joinRoomResult",&GModeGame::joinRoomResult);
    registerMethod("GModeGame_moveResult",&GModeGame::moveResult);
    registerMethod("GModeGame_stopMoveResult",&GModeGame::stopMoveResult);
    registerMethod("GModeGame_roomCountDownResult",&GModeGame::roomCountDownResult);
    registerMethod("GModeGame_roomTimeOutResult",&GModeGame::roomTimeOutResult);
    registerMethod("GModeGame_leaveRoomResult",&GModeGame::leaveRoomResult);
    registerMethod("GModeGame_addBlockResult",&GModeGame::addBlockResult);
    registerMethod("GModeGame_eatBlockResult",&GModeGame::eatBlockResult);
    registerMethod("GModeGame_attackResult",&GModeGame::attackResult);
    registerMethod("GModeGame_bulletCollisionResult",&GModeGame::bulletCollisionResult);
    registerMethod("GModeGame_questLeaveRoomResult",&GModeGame::questLeaveRoomResult);
    registerMethod("GModeGame_bulletChangeTargetResult",&GModeGame::bulletChangeTargetResult);
    registerMethod("GModeGame_speedUpResult",&GModeGame::speedUpResult);
    registerMethod("GModeGame_stopSpeedUpResult",&GModeGame::stopSpeedUpResult);
    registerMethod("GModeGame_allotRobotResult",&GModeGame::allotRobotResult);
    registerMethod("GModeGame_addRobotResult",&GModeGame::addRobotResult);
    registerMethod("GModeGame_rankingResult",&GModeGame::rankingResult);
    registerMethod("GModeGame_relivedResult",&GModeGame::relivedResult);
    registerMethod("GModeGame_dropResult",&GModeGame::dropResult);
    registerMethod("GModeGame_collResult",&GModeGame::collResult);
    registerMethod("GModeGame_updatePosResult",&GModeGame::updatePosResult);
    registerMethod("GModeGame_updateHpResult",&GModeGame::updateHpResult);

}
