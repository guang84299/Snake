//
//  GBubble.h
//  OnlineGame
//
//  Created by guang on 16/10/2.
//
//

#ifndef GBubble_h
#define GBubble_h

#include "cocos2d.h"
#include "data/GJson.h"

class GBubble : public cocos2d::Ref
{
public:
    static GBubble* create();
    static GBubble* create(GJsonObject* obj);
    enum State
    {
        BORN,//出生
        IDLE,//待机
        MOVE,//移动
        SPEEDUP,//加速
        DIE//死亡
    };
    
    GBubble::State toState(std::string &state);
public:
    
    std::string uid;
    std::string name;
    
    State state;
    float speed;
    float accelerated;//移动加速度
    float sSpeed;//冲刺速度
    float sAccelerated;//冲刺加速度
    float sCD;//冲刺CD时间
    int sDistance;//冲刺距离
    float limitSpeed;//限制移动速度
    float limiRradius;//半径限制
    float rotateSpeed;//旋转速度
    float expendHP;//生命消耗速度
    
    float x;
    float y;
    float rotate;
    float dirX;//方向
    float dirY;
    
    float initHp;
    int HP;
    int currHp;
    int level;
    int exp;
    float grow;
    int kill;
    int die;
    int recoverHP;//恢复血量
    float recoverCD;//回复CD
    int reduceHP;//消耗血量
    float reduceCD;//消耗CD
    
    bool robot;
    int bubbleId;
    std::vector<std::string> robotUid;
    int skinId;
    //客户端独有属性
//    float lastDirX;//方向
//    float lastDirY;
    float resetPosDt;
//    float currSp;
//    float rotateTime;
    bool isf;
};

#endif /* GBubble_h */
