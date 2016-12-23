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
    float sSpeed;//冲刺速度
    float rotateSpeed;//旋转速度
    float x;
    float y;
    float rotate;
    float angle;//将要旋转的方向
    float dirX;//方向
    float dirY;
    
    int level;
    int exp;
    float grow;
    int kill;
    int die;
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
    long killTime;
    int killNum;
};

#endif /* GBubble_h */
