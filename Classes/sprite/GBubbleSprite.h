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

#include <spine/spine-cocos2dx.h>
#include "spine/spine.h"
using namespace spine;

using namespace cocos2d::ui;

class GGameScene;

class GBubbleSprite : public cocos2d::Sprite
{
public:
    ~GBubbleSprite();
public:
    static GBubbleSprite* create(GBubble* bubble);
    bool init(GBubble* bubble);

    void createShapeMode();
    
    void initData();
    
    void move(cocos2d::Vec2 &dir,float power,bool speedUp);
    void move2();
    void stopMove(bool speedUp);
    void updateExp();
    void speedUp();
    void speedUpEnd();
    void speedUpCDEnd();
   
    void die();
    void dieEnd();
    
    void updatePosAndRotate();
    void updateRotation(float rotate);
    float getUpdateRotation();
    cocos2d::Vec2 getUpdatePosition();
    float getCollAndBlock();
    cocos2d::Vec2 getBulletPosition(int type);//得到子弹的发射点坐标
    cocos2d::Vec2 getCurrDir();
    
    bool isCollWall();
    void changeState(GBubble::State state);
    void updateHpBar();
    GGameScene* getSelfGame();
    
    void updatePos(float x,float y,float rotate,int state);
    
    void recoverHp();
    void recoverHpEnd();
    void reduceHp();
    void reduceHpEnd();
private:
    cocos2d::Vec2 getMoveVec(cocos2d::Vec2 &v);
    cocos2d::Layer* modeLayer;
    cocos2d::Label* _name;
    LoadingBar* hpBar;
    cocos2d::Sprite* hpBg;
    SkeletonAnimation * skeleton;
    cocos2d::Sprite* tou;
    cocos2d::ParticleSystemQuad* moveParticle;
    cocos2d::ParticleSystemQuad* upSpeedParticle;
    bool isSelf;
    bool isSelfBubble;
    cocos2d::Vec2 currDir;
    float dirPower;
    bool isSpeedDown;
public:
    GBubble* bubble;
    bool isSpeedUp;
    
};


#endif /* GBubbleSprite_h */
