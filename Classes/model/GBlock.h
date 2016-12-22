//
//  GBlock.h
//  OnlineGame
//
//  Created by guang on 16/10/9.
//
//

#ifndef GBlock_h
#define GBlock_h

#include "cocos2d.h"
#include "data/GJson.h"
class GBlock : public cocos2d::Ref
{
public:
    static GBlock* create();
    static GBlock* create(GJsonObject* obj);
    enum State
    {
        IDLE,//待机
        DIE//死亡
    };

    GBlock::State toGBlockState(std::string &state);
public:
    State state;
    int _id;
    int type;//0:普通水滴  1：掉落水晶 2：血袋
    int blockType;
    int exp;
    float x;
    float y;
};

#endif /* GBlock_h */
