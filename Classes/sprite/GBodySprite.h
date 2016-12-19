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
    static GBodySprite* create(GBubbleSprite* bubble,cocos2d::Sprite* parent,int skinId,int tag,float sc);
    bool init(GBubbleSprite* bubble,cocos2d::Sprite* parent,int skinId,int tag,float sc);

    void setChild(GBodySprite* child);
    GBodySprite* getChild();
    
    void resetScale(float sc);
    void showSpeed();
    void showSpeedEnd();
    void changeEnd();
public:
    cocos2d::Sprite* parent;
    GBodySprite* child;
    GBubbleSprite* bubble;
    cocos2d::Sprite* color;
    cocos2d::Sprite* speed;
    bool isTou;
    int tag;
    int skinId;
    float op;
    float sc;
};

#endif /* GBodySprite_h */
