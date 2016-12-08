//
//  GBlockSprite.h
//  OnlineGame
//
//  Created by guang on 16/10/9.
//
//

#ifndef GBlockSprite_h
#define GBlockSprite_h

#include "cocos2d.h"

#include "model/GBlock.h"

class GBubbleSprite;

class GBlockSprite : public cocos2d::Sprite
{
public:
    ~GBlockSprite();
public:
    static GBlockSprite* create(GBlock* block);
    bool init(GBlock* block);
    
    void init(cocos2d::Texture2D* texture);
    
    void die(GBubbleSprite* bubble);
    void dieEnd();
    
public:
    GBlock* block;
};


#endif /* GBlockSprite_h */
