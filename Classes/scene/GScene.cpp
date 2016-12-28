//
//  GScene.cpp
//  OnlineGame
//
//  Created by guang on 16/10/1.
//
//

#include "GScene.h"
#include "protocol_model/GModeUser.h"
#include "network/GServer.h"
#include "ui/GTouchLayer.h"
#include "GHomeScene.h"
#include "GGameScene.h"
#include "GGameScene2.h"
#include "common/GCommon.h"
#include "data/GCache.h"
#include "tools/GJNITools.h"

USING_NS_CC;

bool GScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }
    uiLayer = Layer::create();
    uiLayer->setName("uiLayer");
    uiLayer->setTag(GCommon::NO_COLL);
    this->addChild(uiLayer,1000);
    
    ping = Label::createWithSystemFont("Ping: 0", "", 24);
    ping->setName("ping");
    ping->setAnchorPoint(Vec2(0,1));
    ping->setPosition(2, 100);
    ping->setVisible(Director::getInstance()->isDisplayStats());
    uiLayer->addChild(ping,10000001);
    
    this->_exit = false;
    pingDt = 0;
    uploadPingDt = 0;
    isUpdatePing = false;
    this->schedule(SEL_SCHEDULE(&GScene::updateProtocol),1/30.f);
    if(ping->isVisible())
        this->schedule(SEL_SCHEDULE(&GScene::updatePing),1/30.f);
    return true;
}

void GScene::updatePing(float dt)
{
    if(isUpdatePing)
        this->pingNum += 33;
    pingDt += dt;
    if(pingDt > 0.5f)
    {
        pingDt = 0;
        
        char c[7];
        sprintf(c, "%d",this->pingNum);
        std::string str = "Ping: ";
        str += c;
        ping->setString(str);
    }
    
    uploadPingDt += dt;
}

void GScene::setPing(int pin)
{
    this->pingNum = pin;
    if(pin == 0)
        isUpdatePing = true;
    else
    {
        isUpdatePing = false;
        if(uploadPingDt > 20)
        {
            uploadPingDt = 0;
            GJNITools::ping(pin);
        }
    }
}

void GScene::startHeartBeat()
{
    float heart = 8.f;
    if(ping->isVisible())
        heart = 1.f;
    this->schedule(SEL_SCHEDULE(&GScene::updateServerHeartBeat),heart);
}

void GScene::stopHeartBeat()
{
    this->unschedule(SEL_SCHEDULE(&GScene::updateServerHeartBeat));
}

void GScene::updateServerHeartBeat(float dt)
{
    GModeUser::heartBeat();
}

void GScene::updateProtocol(float dt)
{
    GServer::getInstance()->autoSend();
}

void GScene::reConn()
{
    GServer::getInstance()->setRecConn(true);
    auto seq = Sequence::create(DelayTime::create(0.8f),
                                CallFunc::create(CC_CALLBACK_0(GScene::reConnCallback, this)), NULL);
    seq->setTag(1);
    this->stopActionByTag(1);
    this->runAction(seq);
    
    auto lay = GTouchLayer::create();
    lay->setName("reconn");
    uiLayer->addChild(lay,100);
    
    Size s = Director::getInstance()->getWinSize();
    auto lable = Label::createWithSystemFont(_T("recConn"), "", 24);
    lable->setPosition(s.width/2, s.height/2);
    lay->addChild(lable);
}

void GScene::reConnCallback()
{
    GServer::getInstance()->start();
    
    auto seq = Sequence::create(DelayTime::create(0.5f),
                                CallFunc::create(CC_CALLBACK_0(GScene::reConnEnd, this)), NULL);
    seq->setTag(2);
    this->stopActionByTag(2);
    this->runAction(seq);
}

void GScene::reConnEnd()
{
    GServer::getInstance()->setRecConn(false);
    
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    GGameScene* game = dynamic_cast<GGameScene*>(sc);
    GHomeScene* home = dynamic_cast<GHomeScene*>(sc);
    GGameScene2* game2 = dynamic_cast<GGameScene2*>(sc);
    if(game)
    {
        log("game");
        game->recConn();
        
    }
    else if(home)
    {
        log("home");
        GModeUser::recConn("home");
    }
    else if(game2)
    {
        log("game2");
        GModeUser::recConn("game2");
    }
    
    uiLayer->removeChildByName("reconn");
}

void GScene::onExit()
{
    this->_exit = true;
    Scene::onExit();
}

bool GScene::isExit()
{
    return this->_exit;
}
