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
    void aim(GBubbleSprite* target);
    void aimEnd();
    void stopMove(bool speedUp);
    void hurt(cocos2d::Vec2 dir,float damageCD);
    void hurtEnd();
    void updateExp();
    void speedUp();
    void speedUpEnd();
    void speedUpCDEnd();
    void speedDown(float downSpeed,float time);
    void speedDownEnd();
    void startAttack();
    void stopAttack();
    void updateAttack(float dt);
    void attack(GBulletSprite* bullet);
    void attackEnd();
    void playSkillAnimate();
    void stopSkillAnimate();
    void die();
    void dieEnd();
    void changeSkillBullet(int bulletId);
    float getBulletRange();
    void updatePosAndRotate();
    void updateRotation(float rotate);
    float getUpdateRotation();
    cocos2d::Vec2 getUpdatePosition();
    float getCollAndBlock();
    cocos2d::Vec2 getBulletPosition(int type);//得到子弹的发射点坐标
    cocos2d::Vec2 getCurrDir();
    
    bool isCollWall();
    void removeBody();
    void addBody();
    void changeState(GBubble::State state);
    void loadBullet();
    void fillBullet();
    void updateBulletBar();
    void updateSkillBulletBarEnd();
    void updateHpBar();
    void updateSkillIcon(int bulletId);
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
    cocos2d::Sprite* skillIcon;
    LoadingBar* bulletBar;
    LoadingBar* expBar;
    SkeletonAnimation * skeleton;
    cocos2d::Sprite* tou;
//    cocos2d::Sprite* fuzhu;
    cocos2d::ParticleSystemQuad* moveParticle;
    cocos2d::ParticleSystemQuad* upSpeedParticle;
    bool isSelf;
    bool isSelfBubble;
    int currBulletNum;
    int skillNum;
    cocos2d::Vec2 currDir;
    float dirPower;
    bool isSkill;
    bool isSpeedDown;
    float downSpeed;
    bool lastSpeedUp;//上次是否是加速
//    bool isPlaySpeedUp;
    GBubbleSprite* target;
public:
    GBubble* bubble;
    GBullet* bullet;
    GBullet* skillBullet;
    bool isSkill_4;
    float skill_4_rotateSpeed;
    bool isHurt;
    bool isSpeedUp;
    
    bool isSkill_Coll;
    float skillCollTime;
//    long lt;
    
};


#endif /* GBubbleSprite_h */
