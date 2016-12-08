//
//  GUser.h
//  OnlineGame
//
//  Created by guang on 16/10/1.
//
//

#ifndef GUser_h
#define GUser_h

#include "cocos2d.h"
#include "data/GJson.h"

class GUser : public cocos2d::Ref
{
public:
    static GUser* create();
    static GUser* create(GJsonObject* obj);
    static void updateData(GUser* old,GUser* news);
public:
    std::string uid;
    std::string name;
    std::string password;
    std::string serverId;
    std::string serverIp;
    bool visitor;
    int maxKill;
    int cumKill;//累计击杀
    int skinNum;
    int mvp;
    int headId;
    int crystal;
    std::string skins;
    int skinId;
    //客户端独有
    bool login;
    float skinNumRank = 1;
    float cumKillRank = 1;
    float mvpRank = 1;
    float maxKillRank = 1;
};

#endif /* GUser_h */
