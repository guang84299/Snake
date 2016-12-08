//
//  GServer.h
//  OnlineGame
//
//  Created by guang on 16/9/29.
//
//

#ifndef GServer_h
#define GServer_h

#include "cocos2d.h"
#include "protocol/GData.h"
#include "extensions/cocos-ext.h"
#include "network/WebSocket.h"


class GServer : public cocos2d::Ref, public cocos2d::network::WebSocket::Delegate
{
public:
    static GServer* getInstance();
    
    GServer();
    virtual ~GServer();
    
    void start();
    void send(GData* data);
    void autoSend();
    virtual void onOpen(cocos2d::network::WebSocket* ws)override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data)override;
    virtual void onClose(cocos2d::network::WebSocket* ws)override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error)override;
    
    void close();
    bool isOpen();
    void setRecConn(bool rec);
protected:
    bool recConnn;
    cocos2d::network::WebSocket* _wsiSendText;
    std::vector<std::string> datas;
    std::mutex   dataMutex;
};

#endif /* GServer_h */
