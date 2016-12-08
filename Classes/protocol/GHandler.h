//
//  GHandler.h
//  OnlineGame
//
//  Created by guang on 16/9/29.
//
//

#ifndef GHandler_h
#define GHandler_h

#include "cocos2d.h"

class GHandler : public cocos2d::Ref
{
public:
    static GHandler* getInstance();
    
    void onOpen();
    void onMessage(std::string &message);
    void onClose();
    void onError();
};

#endif /* GHandler_h */
