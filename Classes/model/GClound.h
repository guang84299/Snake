//
//  GClound.h
//  OnlineGame
//
//  Created by guang on 16/10/17.
//
//

#ifndef GClound_h
#define GClound_h

#include "cocos2d.h"
#include "data/GJson.h"

class GClound : public cocos2d::Ref
{
public:
    static GClound* create(GJsonObject* obj);

public:
    int _id;
    float x;
    float y;
    float scale;
};


#endif /* GClound_h */
