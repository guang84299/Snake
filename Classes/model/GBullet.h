//
//  GBullet.h
//  OnlineGame
//
//  Created by guang on 16/10/12.
//
//

#ifndef GBullet_h
#define GBullet_h

#include "cocos2d.h"
#include "data/GJson.h"

class GBullet : public cocos2d::Ref
{
public:
    static GBullet* create(GJsonObject* obj);
    static GBullet* create(GBullet* obj);
    enum State
    {
        IDLE,//待机
        DIE//死亡
    };

    
public:
    State state;
    int _id;
    int tid;
    std::string name;
    float bulletSpeed;//子弹飞行速度
    float bulletTime;//子弹飞行时间
    float bulletAccelerated;//子弹加速度
    float rotateSpeed;//子弹旋转速度
    int damage;//子弹伤害
    float buffTime;//buff持续时间
    float downSpeed;
    int num;//最大子弹数
    int range;//子弹射程
    int targetRange;//寻找目标的范围
    float CD;//发射间隔时间
    float fillBullet;//填满子弹时间
    float loadBullet;//恢复子弹时间
    std::string description;//特殊功能描述
    
    int type;//子弹类型id
    int continueNum;//连续次数
    int onceNum;//一次发射数量
    float chanceFrom;//出现几率
    float chanceTo;//出现几率
    float changeTargetTime;//改变目标时间
    int twoBulletId;//二段子弹id
    float damageCD;//伤害cd时间
    float splitCD;//分裂cd
    int fireAngle;//开火角度
    
    //客户端独有
    std::string uid;
    float dirX;//方向
    float dirY;
};

#endif /* GBullet_h */
