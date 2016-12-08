//
//  GUserController.h
//  OnlineGame
//
//  Created by guang on 16/10/1.
//
//

#ifndef GUserController_h
#define GUserController_h

#include "cocos2d.h"

class GUserController : public cocos2d::Ref
{
private:
    GUserController(){};
public:
    static GUserController* getInstance();
    
    void enterHomeScene();
    void enterGameScene();
private:
    
};
#endif /* GUserController_h */
