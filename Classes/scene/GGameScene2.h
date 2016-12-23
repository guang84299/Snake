//
//  GGameScene2.h
//  Snake
//
//  Created by guang on 2016/12/23.
//
//

#ifndef GGameScene2_h
#define GGameScene2_h

#include "GScene.h"
#include "sprite/GBubbleSprite.h"
#include "sprite/GBlockSprite.h"
#include "ui/GRankView.h"
#include "ui/GMiniMap.h"
#include "ui/GNumber.h"

class GRank {
public:
    int rank;
    std::string uid;
    std::string name;
    int exp;
    int kill;
    int die;
    int rewardNum;
};

class GGameScene2 : public GScene
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    void initUI();
    void initSprite();
    
    void start();
    void update(float dt);
    void end();
    GBubbleSprite* addRobot();
    void addSelf();
    void addBlock();
    void eatBlock(std::string &uid,int _id,float x,float y);
    void coll(GBubbleSprite * bubble,GBubbleSprite * target);
    void drop(GBubbleSprite * bubble);
    void leaveRoom();
    void rockHandler(cocos2d::Vec2 &dir,float power,const char *data);
    
    //更新蛇头和敌人蛇身的碰撞
    void updateCollBubble(float dt);
    //更新泡泡和水滴的碰撞检测
    void updateCollBubbleAndBlock(float dt);
    //更新机器人
    void updateRobot(float dt);
    //更新机器人状态
    void updateRobotState(GBubbleSprite * bubble);
    //机器人躲避
    void updateRobotAvoid();
    //机器人吃掉落水晶
    void updateRobotEat();
    //更新所有位置
    void updateAllPos(float dt);
    //ui
    void updateCountDown(int time);
    void updateExp();
    void rollBg(float dt);
    void updateRank(float dt);
    void openRelived(GJsonObject* obj);
    void reloved();
    void robotReloved(GBubbleSprite * robot);
    void openGameOver(GJsonObject* obj);
    
    void recConn();
    CREATE_FUNC(GGameScene2);
    
public:
    GMiniMap* miniMap;
    void cameraScale();
    //截屏用到
    void cameraReset();
    void cameraResetClose();
    bool isStart();
private:
    cocos2d::Layer * gameLayer;
    cocos2d::TMXTiledMap* tmxMap;
    cocos2d::Layer* bubbleHeadLayer;
    cocos2d::SpriteBatchNode* bubbleLayer;
    
    cocos2d::SpriteBatchNode* blockLayer1;
    cocos2d::SpriteBatchNode* blockLayer2;
    cocos2d::SpriteBatchNode* blockLayer3;
    cocos2d::SpriteBatchNode* blockLayer4;
    cocos2d::SpriteBatchNode* blockLayer5;
    cocos2d::SpriteBatchNode* blockLayer6;
    
    GRankView* rankView;
    
    void touchEvent(Ref *pSender, Widget::TouchEventType type);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
private:
    int robotId;
    int blockId;
    bool _start;
    GBubbleSprite* bubble;
    GNumber* _countDown;
    RichText* myExp;
    cocos2d::Sprite* wifi;
    float crystalsDt;
    float robotDt;
    float robotAvoidDt;
    int robotNum;
    float robotCollDt;
    float updatePosDt;
    float roomTime;
    bool speedUpBtn;
    bool moveBtn;
    cocos2d::Vec2 lastPos;
    cocos2d::Sprite* bulletSkill;
    cocos2d::ProgressTimer* speedCd;
    GJsonObject* obj;
    
    float mapRollDt;
    
    int cameraZ;
    float cameraZDt;
    float rankDt;
    float blockDt;
};

#endif /* GGameScene2_h */
