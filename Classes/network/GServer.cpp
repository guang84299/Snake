//
//  GServer.cpp
//  OnlineGame
//
//  Created by guang on 16/9/29.
//
//

#include "GServer.h"
#include "protocol/GHandler.h"
#include "data/GCache.h"
#include "scene/GScene.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace  cocos2d::network;

static GServer* _instance = nullptr;
GServer::GServer()
: _wsiSendText(nullptr),
recConnn(false)
{
}


GServer::~GServer(void)
{
    if (_wsiSendText)
        _wsiSendText->close();
}

GServer* GServer::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new GServer();
        _instance->autorelease();
        _instance->retain();
    }
    return _instance;
}

void GServer::start()
{
    _wsiSendText = new network::WebSocket();
    if (!_wsiSendText->init(*this, "ws://"+GCache::getInstance()->getUser()->serverIp))
    {
        CC_SAFE_DELETE(_wsiSendText);
    }
    
}

void GServer::send(GData* data)
{
//    std::lock_guard<std::mutex> lk(dataMutex);
//    log("%d", std::this_thread::get_id());
    if(_wsiSendText && !recConnn)
    {
        datas.push_back(data->toString());
    }
    else
    {
        log("server no open");
    }
}

void GServer::autoSend()
{
//    std::lock_guard<std::mutex> lk(dataMutex);
    if(_wsiSendText && datas.size() > 0)
    {
        std::string data = "";
        for(int i=0;i<datas.size();i++)
        {
            data += datas.at(i);
        }
        datas.clear();
        _wsiSendText->send(data);
    }
}

void GServer::onOpen(network::WebSocket* ws)
{
    GHandler::getInstance()->onOpen();
}

void GServer::onMessage(network::WebSocket* ws, const network::WebSocket::Data& data)
{
//    std::string message = "";
//    for (int i = 0; i < data.len; ++i) {
//        if (data.bytes[i] != '\0')
//        {
//            message += data.bytes[i];
//        }
//        else
//        {
//            message += "\'\\0\'";
//        }
//    }
    std::string message = data.bytes;
    message = message.substr(0,data.len);
    GHandler::getInstance()->onMessage(message);
}

void GServer::onClose(network::WebSocket* ws)
{
    recConnn = true;
    log("onClose");
    _wsiSendText = nullptr;
    // Delete websocket instance.
    CC_SAFE_DELETE(ws);
    datas.clear();
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    if(sc && !sc->isExit())
        sc->reConn();
}

void GServer::onError(network::WebSocket* ws, const network::WebSocket::ErrorCode& error)
{
    GHandler::getInstance()->onError();
}

void GServer::close()
{
    if(_wsiSendText && _wsiSendText->getReadyState() == WebSocket::State::OPEN)
        _wsiSendText->close();
}
bool GServer::isOpen()
{
    return (_wsiSendText && _wsiSendText->getReadyState() == WebSocket::State::OPEN);
}

void GServer::setRecConn(bool rec)
{
    this->recConnn = rec;
}
