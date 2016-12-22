//
//  GBubbleSprite.h
//  OnlineGame
//
//  Created by guang on 16/10/2.
//
//

#ifndef GBubbleSprite_h
#define GBubbleSprite_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "model/GBubble.h"
#include "GBulletSprite.h"
#include "sprite/GBodySprite.h"

#include <spine/spine-cocos2dx.h>
#include "spine/spine.h"
using namespace spine;

using namespace cocos2d::ui;

class GGameScene;

class GPath2
{
public:
    GPath2(cocos2d::Vec2 p,float angle,float time){
        this->p = p;
        this->angle = angle;
        this->time = time;
    }
    cocos2d::Vec2 p;
    float angle;
    float time;
};

class GBubbleSprite : public cocos2d::Sprite
{
public:
    ~GBubbleSprite();
public:
    static GBubbleSprite* create(GBubble* bubble);
    bool init(GBubble* bubble);

    void createShapeMode();
    void resetAll();
    
    void initData();
    void initBody();
    
    void update(float dt);
    void updateBody(cocos2d::Action* ac);
    
    void move(cocos2d::Vec2 &dir,float power,bool speedUp);
    void move2();
    void stopMove(bool speedUp);
    void updateExp(bool up,bool move);
    void speedUp();
    void speedUpEnd();
    void speedUpCDEnd();
   
    void die();
    void dieEnd();
    
    void addBody();
    void addBodyEnd();
    void removeBody();
    void removeBodyEnd();
    void removeAllBody();
    
    void playRelive();
    
    void updatePosAndRotate();
    void updateRotation(float rotate);
    float getUpdateRotation();
    cocos2d::Vec2 getUpdatePosition();
    float getCollAndBlock();
    cocos2d::Vec2 getBulletPosition(int type);//得到子弹的发射点坐标
    cocos2d::Vec2 getCurrDir();
    cocos2d::Vec2 getBodyDir(cocos2d::Sprite* body);
    cocos2d::Sprite* getBody();
    
    bool isCollWall();
    bool isRotate();
    void changeState(GBubble::State state);
    GGameScene* getSelfGame();
    
    void updatePos(float x,float y,float rotate,float time,bool up);
    void move3();
    void coll();
    void collEnd();
    void recoverHp();
    void recoverHpEnd();
    void reduceHp();
    void reduceHpEnd();
private:
    cocos2d::Vec2 getMoveVec(cocos2d::Vec2 &v);
    cocos2d::Layer* modeLayer;
    cocos2d::Label* _name;
    cocos2d::Sprite* body;
    bool isSelf;
    bool isSelfBubble;
    float dirPower;
    bool isSpeedDown;
    std::vector<int> showSpeedNums;
    float showSpeedDt;
public:
    GBubble* bubble;
    bool isSpeedUp;
    bool isDelay;
    bool isCheck;
    bool isColl;
    float delayDt;
    float angle;
    std::vector<GBodySprite*> bodys;
    std::vector<GPath> points;
    std::vector<GPath2> vecs;
    int body_z;
    float sSpeed;
    cocos2d::Vec2 currDir;
    cocos2d::Vec2 lastDir;
};


#endif /* GBubbleSprite_h */
