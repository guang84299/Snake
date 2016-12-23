//
//  GBodySprite.h
//  Snake
//
//  Created by guang on 2016/12/9.
//
//

#ifndef GBodySprite_h
#define GBodySprite_h

#include "cocos2d.h"


class GBubbleSprite;

class GPath
{
public:
    GPath(cocos2d::Vec2 p,float angle){
        this->p = p;
        this->angle = angle;
    }
    cocos2d::Vec2 p;
    float angle;
};

class GBodySprite : public cocos2d::Sprite
{
public:
    static GBodySprite* create();
    void init(int skinId,int tag,float sc,bool isEnd);

    
    void resetScale(float sc);
    void showSpeed();
    void showSpeedEnd();
    
    const cocos2d::Size& getCollSize();
public:
    cocos2d::Sprite* color;
    cocos2d::Sprite* speed;
    bool isTou;
    bool isEnd;
    int tag;
    int skinId;
    float op;
    float op2;
    float sc;
};

#endif /* GBodySprite_h */
