//
//  GHandler.cpp
//  OnlineGame
//
//  Created by guang on 16/9/29.
//
//

#include "GHandler.h"
#include "data/GJson.h"
#include "GReflect.h"
#include "GData.h"

static GHandler* _instance = nullptr;

GHandler* GHandler::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new GHandler();
        _instance->autorelease();
        _instance->retain();
    }
    return _instance;
}

void GHandler::onOpen()
{
    
}

void GHandler::onMessage(std::string &message)
{
    GJsonObject* json = GJsonObject::create(message);
    if(json)
    {
        std::string mode = json->getString("mode");
        std::string body = json->getString("body");
//        std::string method = mode.substr(mode.find("_")+1,mode.length());
        GReflect::getInstance()->invoke(mode, body.c_str());
    }
    else
    {
        CCLOG("-----onMessage null ------");
    }
}

void GHandler::onClose()
{
    CCLOG("-----onClose2------");
}

void GHandler::onError()
{
    CCLOG("-----onError------");
}
