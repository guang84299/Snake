//
//  GScene.h
//  OnlineGame
//
//  Created by guang on 16/10/1.
//
//

#ifndef GScene_h
#define GScene_h

#include "cocos2d.h"

class GScene : public cocos2d::Scene
{
public:
    
    virtual bool init();
    
    void startHeartBeat();
    void stopHeartBeat();
    void updateServerHeartBeat(float dt);
    void updateProtocol(float dt);
    
    void reConn();
    void reConnCallback();
    void reConnEnd();
    
    virtual void onExit();
   
    bool isExit();
    CREATE_FUNC(GScene);
    
    void updatePing(int ping);
protected:
    cocos2d::Layer * uiLayer;
    cocos2d::Label* ping;
private:
    bool _exit;
};

#endif /* GScene_h */
