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
#include "common/GCommon.h"
#include "data/GCache.h"

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
    this->schedule(SEL_SCHEDULE(&GScene::updateProtocol),1/30.f);
    return true;
}

void GScene::updatePing(int pin)
{
    char c[7];
    sprintf(c, "%d",pin);
    std::string str = "Ping: ";
    str += c;
    ping->setString(str);
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
