//
//  GModeUser.h
//  OnlineGame
//
//  Created by guang on 16/9/28.
//
//

#ifndef GModeUser_h
#define GModeUser_h

#include "cocos2d.h"


class GModeUser : public cocos2d::Ref
{
public:
    static void login();
    static void loginResult(const char* data);
    static void heartBeat();
    static void heartBeatResult(const char* data);
    static void enterRoom();
    static void enterRoomResult(const char* data);
    static void recConn(const char* type);
    static void recConnResult(const char* data);
    static void updateName(std::string name);
    static void updateNameResult(const char* data);
    static void updateHead(int headId);
    static void updateHeadResult(const char* data);
    static void infos();
    static void infosResult(const char* data);
    static void buySkin(int skinId);
    static void buySkinResult(const char* data);
    static void useSkin(int skinId);
    static void useSkinResult(const char* data);
    static void share();
    static void shareResult(const char* data);
    
    static void closeServer();
};

#endif /* GModeUser_h */
