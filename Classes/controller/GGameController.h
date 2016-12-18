//
//  GGameController.h
//  OnlineGame
//
//  Created by guang on 16/10/3.
//
//

#ifndef GGameController_h
#define GGameController_h

#include "cocos2d.h"

#include "sprite/GBubbleSprite.h"
#include "sprite/GBlockSprite.h"
#include "sprite/GBulletSprite.h"
#include "sprite/GCloundSprite.h"
#include "scene/GGameScene.h"


class GGameController : public cocos2d::Ref
{
private:
    GGameController(){};
    ~GGameController();
public:
    static GGameController* getInstance();
    void init(GGameScene * scene);
    
    void start(const char* data);
    void joinRoom(const char* data);
    void roomTimeOut(const char* data);
    void leaveRoom(const char* data);
    void questLeaveRoom(const char* data);
    void move(const char *data);
    void stopMove(const char *data);
    void updateCountDown(int time);
    void addBlock(const char *data);
    void eatBlock(const char *data);
    void attack(const char *data);
    void bulletCollision(const char *data);
    void bulletChangeTarget(const char *data);
    void speedUp(const char *data);
    void stopSpeedUp(const char *data);
    
    void allotRobot(const char *data);
    void addRobot(const char *data);
    
    void ranking(const char *data);
    void relived(const char *data);
    void coll(const char *data);
    void updatePos(const char *data);
    void updateHp(const char *data);

    GBubble* getSelfBubble();
    bool isSelfBullet(GBullet* bullet);
    bool isSelfBubble(GBubble* bubble);
    bool isContain(GBubbleSprite* bubble,GBubbleSprite* target);
public:
    void addBlock(GBlockSprite* block);
    
    void deleteBlock(int _id,float x,float y);
    void deleteBullet(int tid);
    void deleteBubble(std::string &uid);
    void deleteRobot(std::string &uid);
    
    GBubbleSprite* findByUid(std::string &uid);
    GBubbleSprite* findRobotByUid(std::string &uid);
    GBulletSprite* findBullet(int tid);
    GBlockSprite* findBlock(int _id,float x,float y);
public:
    GGameScene * scene;
    GBubbleSprite* bubble;
    std::vector<GBubbleSprite* > bubbles;
    
    std::vector<std::vector<GBlockSprite* >> blocks;

    std::vector<GCloundSprite* > clounds;
    std::vector<GBulletSprite* > bullets;
    std::vector<GBubbleSprite* > robots;
};

#endif /* GGameController_h */
