//
//  GModeGame.h
//  OnlineGame
//
//  Created by guang on 16/10/3.
//
//

#ifndef GModeGame_h
#define GModeGame_h

#include "cocos2d.h"

class GBullet;
class GBubble;
class GBlock;

class GModeGame : public cocos2d::Ref
{
public:
    static void startGame();
    static void startGameResult(const char* data);
    static void joinRoomResult(const char* data);
    static void move(cocos2d::Vec2 &dir,float power,GBubble* bubble);
    static void moveResult(const char* data);
    static void stopMove(GBubble* bubble);
    static void stopMoveResult(const char* data);
    static void roomCountDownResult(const char* data);
    static void roomTimeOutResult(const char* data);
    static void leaveRoomResult(const char* data);
    static void questLeaveRoom();
    static void questLeaveRoomResult(const char* data);
    static void addBlockResult(const char* data);
    static void eatBlock(GBlock* block,GBubble* bubble);
    static void eatBlockResult(const char* data);
    static void attack(GBubble* bubble,GBullet* bullet);
    static void attackResult(const char* data);
    static void bulletCollision(GBullet* bullet,GBubble* target);
    static void bulletCollisionResult(const char* data);
    static void bulletChangeTarget(GBullet* bullet,GBubble* target);
    static void bulletChangeTargetResult(const char* data);
    static void speedUp(GBubble* bubble);
    static void speedUpResult(const char* data);
    static void stopSpeedUp(GBubble* bubble);
    static void stopSpeedUpResult(const char* data);
    static void allotRobotResult(const char* data);
    static void addRobotResult(const char* data);
    static void rankingResult(const char* data);
    static void relived();
    static void relivedResult(const char* data);
    static void drop(GBubble* target);
    static void dropResult(const char* data);
    static void coll(GBubble* bubble,const cocos2d::Vec2 &collPos);
    static void collResult(const char* data);
    static void updatePos(GBubble* target);
    static void updatePosResult(const char* data);
    static void updateHp(GBubble* target,int type);
    static void updateHpResult(const char* data);
};


#endif /* GModeGame_h */
