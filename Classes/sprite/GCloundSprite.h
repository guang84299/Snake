//
//  GCloundSprite.h
//  OnlineGame
//
//  Created by guang on 16/10/17.
//
//

#ifndef GCloundSprite_h
#define GCloundSprite_h

#include "cocos2d.h"

#include "model/GClound.h"

class GCloundSprite : public cocos2d::Sprite
{
public:
    ~GCloundSprite();
public:
    static GCloundSprite* create(GClound* clound);
    bool init(GClound* clound);
    
    
    static GCloundSprite* create(cocos2d::Vec2* points,int size);
    bool init(cocos2d::Vec2* points,int size);
    
    static GCloundSprite* create(int size);
    bool init(int size);

public:
    GClound* clound;
};


#endif /* GCloundSprite_h */
