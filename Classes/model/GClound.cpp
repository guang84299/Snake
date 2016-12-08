//
//  GClound.cpp
//  OnlineGame
//
//  Created by guang on 16/10/17.
//
//

#include "GClound.h"

USING_NS_CC;

GClound* GClound::create(GJsonObject* obj)
{
    GClound* ball = new GClound();

    ball->_id = obj->getInt("id");
    ball->scale = obj->getDouble("scale");
    ball->x = obj->getDouble("x");
    ball->y = obj->getDouble("y");
    
    ball->autorelease();
    return ball;
}
