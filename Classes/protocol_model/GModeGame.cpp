//
//  GModeGame.cpp
//  OnlineGame
//
//  Created by guang on 16/10/3.
//
//

#include "GModeGame.h"
#include "protocol/GProtocol.h"
#include "protocol/GData.h"
#include "data/GJson.h"
#include "network/GServer.h"
#include "controller/GGameController.h"
#include "controller/GUserController.h"
#include "model/GBullet.h"

USING_NS_CC;

void GModeGame::startGame()
{
    GJsonObject* val = GJsonObject::create();
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_STARTGAME, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::startGameResult(const char *data)
{
   GGameController::getInstance()->start(data);
}

void GModeGame::joinRoomResult(const char *data)
{
    //更新加入房间的人
    GGameController::getInstance()->joinRoom(data);
}

void GModeGame::move(cocos2d::Vec2 &dir,float power,GBubble* bubble)
{
    GJsonObject* val = GJsonObject::create();
    val->putString("uid", bubble->uid);
    val->putBool("robot", bubble->robot);
    val->putDouble("dirX", dir.x);
    val->putDouble("dirY", dir.y);
    val->putDouble("power", power);
    val->putDouble("x", bubble->x);
    val->putDouble("y", bubble->y);
    val->putDouble("rotate", bubble->rotate);
    
    val->putBool("speedUp", (bubble->state == GBubble::State::SPEEDUP));
    
//    val->putDouble("lastDirX", bubble->lastDirX);
//    val->putDouble("lastDirY", bubble->lastDirY);
//    val->putDouble("resetPosDt", bubble->resetPosDt);
    
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_MOVE, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::moveResult(const char *data)
{
    if(GGameController::getInstance()->scene)
        GGameController::getInstance()->move(data);
}

void GModeGame::stopMove(GBubble* bubble)
{
    GJsonObject* val = GJsonObject::create();
    val->putDouble("x", bubble->x);
    val->putDouble("y", bubble->y);
    val->putDouble("rotate", bubble->rotate);
    val->putBool("speedUp", (bubble->state == GBubble::State::SPEEDUP));
    
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_STOPMOVE, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::stopMoveResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->stopMove(data);
}

void GModeGame::roomCountDownResult(const char *data)
{
    GJsonObject* val = GJsonObject::create(data);
    int time = val->getInt("time");
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->updateCountDown(time);
}

void GModeGame::roomTimeOutResult(const char *data)
{
    if(GGameController::getInstance()->scene)
        GGameController::getInstance()->roomTimeOut(data);
//    GUserController::getInstance()->enterHomeScene();
}

void GModeGame::leaveRoomResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->leaveRoom(data);
}

void GModeGame::questLeaveRoom()
{
    GJsonObject* val = GJsonObject::create();
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_QUESTLEAVEROOM, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::questLeaveRoomResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->questLeaveRoom(data);
}

void GModeGame::addBlockResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->addBlock(data);
}

void GModeGame::eatBlock(GBlock* block,GBubble* bubble)
{
    GJsonObject* val = GJsonObject::create();
    val->putInt("id", block->_id);
    val->putInt("type", block->type);
    val->putBool("robot", bubble->robot);
    val->putString("uid", bubble->uid);
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_EATBLOCK, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::eatBlockResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->eatBlock(data);
}

void GModeGame::attack(GBubble* bubble,GBullet* bullet)
{
    GJsonObject* val = GJsonObject::create();
    val->putString("uid", bubble->uid);
    val->putBool("robot", bubble->robot);
    val->putInt("bulletId", bullet->_id);
    val->putInt("tid", bullet->tid);
    
    val->putDouble("dirX", bullet->dirX);
    val->putDouble("dirY", bullet->dirY);
    
//    if(bullet->_id == 6)
//    {
//        val->putDouble("dirX", bullet->dirX);
//        val->putDouble("dirY", bullet->dirY);
//    }
//    else
//    {
//        val->putDouble("dirX", bubble->dirX);
//        val->putDouble("dirY", bubble->dirY);
//    }

    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_ATTACK, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::attackResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->attack(data);
}

void GModeGame::bulletCollision(GBullet *bullet,GBubble* target)
{
    GJsonObject* val = GJsonObject::create();
    val->putInt("id", bullet->_id);
    val->putInt("damage", bullet->damage);
    val->putInt("tid", bullet->tid);
    val->putString("uid", bullet->uid);
    
    if(target)
    {
        val->putBool("target_robot", target->robot);
        val->putString("target", target->uid);
    }
    else
    {
        val->putBool("target_robot", false);
        val->putString("target", "0");
    }

    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_BULLETCOLLISION, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::bulletCollisionResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->bulletCollision(data);
}

void GModeGame::bulletChangeTarget(GBullet* bullet,GBubble* target)
{
//    GJsonObject* val = GJsonObject::create();
//    val->putString("target", target->uid);
//    val->putBool("target_robot", target->robot);
//    val->putInt("id", bullet->_id);
//    
//    GJsonObject* val2 = GJsonObject::create();
//    val2->putObject("body", val);
//    GData* data = GData::create(gprotocol::MODE_GAME_BULLETCHANGETARGET, val2);
//    GServer::getInstance()->send(data);
}

void GModeGame::bulletChangeTargetResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->bulletChangeTarget(data);
}

void GModeGame::speedUp(GBubble* bubble)
{
    GJsonObject* val = GJsonObject::create();
    val->putDouble("x", bubble->x);
    val->putDouble("y", bubble->y);
    val->putDouble("rotate", bubble->rotate);
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_SPEEDUP, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::speedUpResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->speedUp(data);
}

void GModeGame::stopSpeedUp(GBubble* bubble)
{
    GJsonObject* val = GJsonObject::create();
    val->putDouble("x", bubble->x);
    val->putDouble("y", bubble->y);
    val->putDouble("rotate", bubble->rotate);
    val->putString("uid", bubble->uid);
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_STOPSPEEDUP, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::stopSpeedUpResult(const char *data)
{
    if(GGameController::getInstance()->scene)
        GGameController::getInstance()->stopSpeedUp(data);
}

void GModeGame::allotRobotResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->allotRobot(data);
}

void GModeGame::addRobotResult(const char *data)
{
    if(GGameController::getInstance()->scene)
    GGameController::getInstance()->addRobot(data);
}

void GModeGame::rankingResult(const char *data)
{
    if(GGameController::getInstance()->scene)
        GGameController::getInstance()->ranking(data);
}
//type 复活
void GModeGame::relived()
{
    GJsonObject* val = GJsonObject::create();
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_RELIVED, val2);
    GServer::getInstance()->send(data);

}

void GModeGame::relivedResult(const char* data)
{
    if(GGameController::getInstance()->scene)
        GGameController::getInstance()->relived(data);
}

void GModeGame::drop(GBubble* target)
{
    GJsonObject* val = GJsonObject::create();
    val->putBool("robot", target->robot);
    val->putString("target", target->uid);
    val->putDouble("x", target->x);
    val->putDouble("y", target->y);
    
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_DROP, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::dropResult(const char* data)
{
    
}

void GModeGame::coll(GBubble* bubble,const Vec2 &collPos)
{
    GJsonObject* val = GJsonObject::create();
    val->putDouble("x", bubble->x);
    val->putDouble("y", bubble->y);
    val->putDouble("rotate", bubble->rotate);
    val->putDouble("collx", collPos.x);
    val->putDouble("colly", collPos.y);
    
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_COLL, val2);
    GServer::getInstance()->send(data);
}

void GModeGame::collResult(const char *data)
{
    if(GGameController::getInstance()->scene)
        GGameController::getInstance()->coll(data);
}

void GModeGame::updatePos(GBubble* target)
{
    GJsonObject* val = GJsonObject::create();
    
    val->putDouble("rotate", target->rotate);
    val->putString("uid", target->uid);
    int state = 0;
    if(target->state == GBubble::State::IDLE)
        state = 1;
    else if(target->state == GBubble::State::MOVE)
        state = 2;
    else if(target->state == GBubble::State::SPEEDUP)
        state = 3;
    else if(target->state == GBubble::State::DIE)
        state = 4;
    val->putInt("state", state);
    
    val->putDouble("x", target->x);
    val->putDouble("y", target->y);
    
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_UPDATEPOS, val2);
    GServer::getInstance()->send(data);
}
void GModeGame::updatePosResult(const char* data)
{
    if(GGameController::getInstance()->scene)
        GGameController::getInstance()->updatePos(data);
}

void GModeGame::updateHp(GBubble* target,int type)
{
    GJsonObject* val = GJsonObject::create();
    
    val->putString("uid", target->uid);
    val->putInt("type", type);
    
    GJsonObject* val2 = GJsonObject::create();
    val2->putObject("body", val);
    GData* data = GData::create(gprotocol::MODE_GAME_UPDATEHP, val2);
    GServer::getInstance()->send(data);
}
void GModeGame::updateHpResult(const char* data)
{
    if(GGameController::getInstance()->scene)
        GGameController::getInstance()->updateHp(data);
}

