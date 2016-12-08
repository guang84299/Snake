//
//  GGameScene.h
//  OnlineGame
//
//  Created by guang on 16/10/2.
//
//

#ifndef GGameScene_h
#define GGameScene_h

#include "GScene.h"
#include "sprite/GBubbleSprite.h"
#include "sprite/GBlockSprite.h"
#include "ui/GRankView.h"
#include "ui/GMiniMap.h"
#include "ui/GNumber.h"

class GGameScene : public GScene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    void onContactSeparate(cocos2d::PhysicsContact& contact);
    bool onContactPreSolve(cocos2d::PhysicsContact& contact, cocos2d::PhysicsContactPreSolve& solve);
    void initUI();
    void resetInit();
    void initSprite();
    
    void start();
    void update(float dt);
    void end();
    void joinRoom(GBubbleSprite* bubble);
    void leaveRoom();
    void addBlock(GBlockSprite* block,int x,int y);
    void move(bool isSelf,cocos2d::Vec2 &dir,float power,cocos2d::Vec2 &pos,float rotate,std::string &uid,bool robot,bool speedUp);
    void stopMove(bool isSelf,cocos2d::Vec2 &pos,float rotate,std::string &uid,bool speedUp);
    void coll(bool isSelf,cocos2d::Vec2 &pos,cocos2d::Vec2 &collPos,float rotate,std::string &uid);
    void rockHandler(cocos2d::Vec2 &dir,float power,const char *data);
    
    //更新泡泡和障碍的碰撞
    void updateCollBubbleAndMap(float dt);
    //更新泡泡和泡泡的碰撞
    void updateCollBubbleAndBubble(float dt);
    //更新泡泡目标
    void updateBubbleTarget(float dt);
    //更新泡泡和水滴的碰撞检测
    void updateCollBubbleAndBlock(float dt);
    //更新泡泡和子弹碰撞
    void updateCollBubbleAndBullet(float dt);
    //更新子弹和障碍的碰撞
    void updateCollBulletAndMap(float dt);
    //更新机器人
    void updateRobot(float dt);
    //更新机器人状态
    void updateRobotState(GBubbleSprite * bubble);
    //更新所有位置
    void updateAllPos(float dt);
    //ui
    void updateCountDown(int time);
    void rollBg();
    void updateRank(GJsonArray* arr);
    void openRelived(GJsonObject* obj);
    void reloved();
    void openGameOver(GJsonObject* obj);
    void updateBulletSkill(int bulletId);
    void updateSpeedCd(float time,float pro);
    void updateAttackCd(float time,float pro);

    void recConn();
    CREATE_FUNC(GGameScene);
    
public:
    GMiniMap* miniMap;
    //截屏用到
    void cameraReset();
    void cameraResetClose();
private:
    cocos2d::Layer * gameLayer;
    cocos2d::TMXTiledMap* tmxMap;
    cocos2d::Layer* cloundLayer;
    cocos2d::Layer* hideLayer;
    cocos2d::Layer* blockLayer;
    cocos2d::Layer* bulletLayer;
    cocos2d::Layer* bubbleLayer;
    std::vector<cocos2d::Sprite*> colls;
    
    GRankView* rankView;
    
    void touchEvent(Ref *pSender, Widget::TouchEventType type);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
private:
    bool _start;
    GBubbleSprite* bubble;
    GNumber* _countDown;
    float crystalsDt;
    float robotDt;
    int robotNum;
    float robotCollDt;
    float updatePosDt;
    float roomTime;
    bool speedUpBtn;
    bool moveBtn;
    cocos2d::Vec2 lastPos;
    cocos2d::Sprite* bulletSkill;
    cocos2d::ProgressTimer* speedCd;
    cocos2d::ProgressTimer* attackCd;
    
    GJsonObject* obj;
};


#endif /* GGameScene_h */
