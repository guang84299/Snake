//
//  GData.h
//  OnlineGame
//
//  Created by guang on 16/9/28.
//
//

#ifndef GData_h
#define GData_h

#include "cocos2d.h"

class GJsonObject;

class GData : public cocos2d::Ref
{
public:
    static GData* create(std::string mode,GJsonObject *jsbody);
    std::string pack();
    std::string toString();
public:
    GJsonObject *jsbody;
    std::string mode;
    std::string body;
    
    int length;
    int bodyLength;
private:
    std::string str;
    
};

#endif /* GData_h */
