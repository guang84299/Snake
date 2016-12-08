//
//  GBulletSprite.h
//  OnlineGame
//
//  Created by guang on 16/10/12.
//
//

#ifndef GBulletSprite_h
#define GBulletSprite_h

#include "cocos2d.h"
#include "model/GBullet.h"

class GBubbleSprite;

class GBulletSprite : public cocos2d::Layer
{
public:
    ~GBulletSprite();
public:
    static GBulletSprite* create(GBullet* bullet,cocos2d::Vec2 &dir);
    bool init(GBullet* bullet,cocos2d::Vec2 &dir);
    
    //水球
    void createBullet_1();
    //连发子弹
    void createBullet_2();
    //跟踪导弹
    void createBullet_3();
    //激光
    void createBullet_4();
    //分裂爆弹
    void createBullet_5();
    //分裂爆弹的子弹
    void createBullet_6();
    //辐射弹
    void createBullet_7();
    //喷射弹
    void createBullet_8();

    void run();
    void runBullet_1();
    void runBullet_2();
    void runBullet_2_resetDir();
    void runBullet_3();
    void runBullet_4();
    void runBullet_5();
    void runBullet_6();
    void runBullet_7();
    void runBullet_8();
    
    void die();
    void dieEnd();
    
    void updateBullet_3(float dt);
    void updateBullet_4(float dt);
    void updateBullet_5(float dt);
    void updateBullet_7(float dt);
    void updateBullet_8(float dt);
    void findTarget();
    void changeTarget(cocos2d::Vec2 &dir);
    void attack(GBulletSprite* bullet);
public:
    void judgeColl(GBubbleSprite* target);
    float getUpdateRotation();
    void setUpdateRotation(float rotation);
    GBullet* bullet;
    cocos2d::Vec2 dir;
    cocos2d::Vec2 lastDir;
    bool isSelf;
private:
    cocos2d::Node* _body;
    float runTime;
    float currSpeed;
    GBubbleSprite* bubble_4;
    GBubbleSprite* bubble_5;
    GBubbleSprite* bubble_7;
    GBubbleSprite* bubble_8;
    GBullet* bullet_5;
    std::vector<cocos2d::Sprite*> collSp;
};


#endif /* GBulletSprite_h */
