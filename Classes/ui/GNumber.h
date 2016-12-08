//
//  GNumber.h
//  PaoPao
//
//  Created by guang on 2016/11/28.
//
//

#ifndef GNumber_h
#define GNumber_h

#include "cocos2d.h"

class GNumber : public cocos2d::Sprite
{
public:
    static GNumber* create(int num);
    
    bool init(int num);
    void setNum(int num);
    
    void setColor(cocos2d::Color3B color);
    
    void useRankStyle();
    void useTimeStyle();
private:
    cocos2d::Color3B _color;
    int _num;
    bool _styleTime;
};

#endif /* GNumber_h */
