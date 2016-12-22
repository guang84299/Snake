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
    void initUI();
    void resetInit();
    void resetInitBlock();
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
    void updateRank(GJsonArray* arr);
    void openRelived(GJsonObject* obj);
    void reloved();
    void openGameOver(GJsonObject* obj);
    void updateBulletSkill(int bulletId);
    void updateSpeedCd(float time,float pro);

    void recConn();
    CREATE_FUNC(GGameScene);
    
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
};


#endif /* GGameScene_h */
