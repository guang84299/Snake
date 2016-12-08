//
//  GReflect.h
//  OnlineGame
//
//  Created by guang on 16/9/28.
//
//

#ifndef GReflect_h
#define GReflect_h

#include "cocos2d.h"

class GData;

typedef void (*GProtocol)(const char *data);

class GReflect : public cocos2d::Ref
{
private:
    GReflect(){};
public:
    static GReflect* getInstance();
    void registerAll();
    void registerMethod(std::string name,GProtocol pro);
    void invoke(std::string name,const char* data);
private:
    std::map<std::string,GProtocol> maps;
};


#endif /* GReflect_h */
