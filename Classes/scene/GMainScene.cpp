#include "GMainScene.h"
#include "protocol/GReflect.h"
#include "protocol/GData.h"
#include "data/GJson.h"
#include "network/GServer.h"
#include "network/GHttp.h"
#include "data/GCache.h"
#include "protocol_model/GModeUser.h"
#include "tools/GTools.h"
//#include "ui/GShaderView.h"
#include "tools/GJNITools.h"
#include "common/GCommon.h"
USING_NS_CC;

Scene* GMainScene::createScene()
{
    auto layer = GMainScene::create();
    layer->setName("scene");

    return layer;
}

bool GMainScene::init()
{
    if ( !GScene::init() )
    {
        return false;
    }
    
    GCache::getInstance()->initFont();
    
    auto s = Director::getInstance()->getWinSize();
    auto bg1 = Sprite::create("jm-souyeBJ1.png");
    bg1->setScale(2);
    bg1->setPosition(s.width/2, s.height/2);
    uiLayer->addChild(bg1, 1);
    
    auto bg = Layer::create();
    uiLayer->addChild(bg, 1);
    
    auto logo = Sprite::create("logo.png");
    logo->setPosition(Vec2(s.width/2,s.height*0.7f));
    bg->addChild(logo);
    
    auto barBg = Sprite::create("jm-qidong1.png");
    barBg->setPosition(Vec2(s.width/2, s.height*0.2f));
    bg->addChild(barBg);
    
    bar = LoadingBar::create("jm-qidong2.png");
    bar->setAnchorPoint(Vec2(0,0));
    bar->setPosition(Vec2(0, 0));
    bar->setDirection(LoadingBar::Direction::LEFT);
    bar->setPercent(0);
    barBg->addChild(bar);
    
    
    auto load = Sprite::create("jm-qidong3.png");
    load->setAnchorPoint(Vec2(0,0));
    load->setPosition(Vec2(barBg->getPositionX()-barBg->getContentSize().width/2, barBg->getPositionY()+10));
    bg->addChild(load);
    
    dian1 = Sprite::create("jm-qidong4.png");
    dian1->setAnchorPoint(Vec2(0,0));
    dian1->setPosition(Vec2(load->getPositionX()+load->getContentSize().width+1, load->getPositionY()+3));
    bg->addChild(dian1);
    
    dian2 = Sprite::create("jm-qidong4.png");
    dian2->setAnchorPoint(Vec2(0,0));
    dian2->setPosition(Vec2(load->getPositionX()+load->getContentSize().width+13, load->getPositionY()+3));
    bg->addChild(dian2);
    
    dian3 = Sprite::create("jm-qidong4.png");
    dian3->setAnchorPoint(Vec2(0,0));
    dian3->setPosition(Vec2(load->getPositionX()+load->getContentSize().width+13*2, load->getPositionY()+3));
    bg->addChild(dian3);
    
    label = Text::create("%0", "", 24);
//    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2(1,0));
    label->setPosition(Vec2(barBg->getPositionX() + barBg->getContentSize().width/2,load->getPositionY()));
    bg->addChild(label);
    
    
    GReflect::getInstance()->registerAll();
    
    resNum = 0;
    this->schedule(SEL_SCHEDULE(&GMainScene::updateRes),0.1f);
    
    if(GJNITools::isSign())
    {
        GTools::preload();
        this->runAction(Sequence::create(DelayTime::create(3),
                                         CallFunc::create(CC_CALLBACK_0(GMainScene::login, this)) ,
                                         nullptr));
    }
    return true;
}

void GMainScene::updateRes(float dt)
{
    resNum+=dt;
    if(resNum <= 2.1f)
    {
        int per = resNum/2.f*100;
        bar->setPercent(per);
        char c[7];
        sprintf(c, "%d",per);
        label->setString(c + std::string("%"));
        
        int p = per%4;
        if(p == 0)
        {
            dian1->setVisible(false);
            dian2->setVisible(false);
            dian3->setVisible(false);
        }
        else if(p == 1)
        {
            dian1->setVisible(true);
            dian2->setVisible(false);
            dian3->setVisible(false);
        }
        else if(p == 2)
        {
            dian1->setVisible(true);
            dian2->setVisible(true);
            dian3->setVisible(false);
        }
        else if(p == 3)
        {
            dian1->setVisible(true);
            dian2->setVisible(true);
            dian3->setVisible(true);
        }
    }
}

void GMainScene::login()
{
    GHttp * http = GHttp::create();
    GJsonObject* val = GJsonObject::create();
    std::string uid = GCache::getInstance()->getUid();
    if(uid == "")
    {
        uid = GJNITools::getDeviceId();
        GCache::getInstance()->setUid(uid);
        GCache::setMusic(true);
        GCache::setSound(true);
        GCache::setHandStyle(true);
    }
    val->putString("uid", uid.c_str());
    std::string data = "data="+val->toString();
    http->request(_T("login ip").c_str(), data.c_str(),GHTTP_CALLBACK(GMainScene::loginCallback, this));
}

void GMainScene::updateServerState(float dt)
{
    if(GServer::getInstance()->isOpen())
    {
        this->unschedule(SEL_SCHEDULE(&GMainScene::updateServerState));
        GModeUser::login();
    }
}

void GMainScene::loginCallback(bool state, const char *data)
{
    if(state)
    {
        GJsonObject* val = GJsonObject::create(data);
        int state = val->getInt("state");
        if(state == 1)
        {
            val = val->getObject("data");
            GUser* user = GUser::create(val);
            
            GCache::getInstance()->setUser(user);
            GServer::getInstance()->start();
         //   cocos2d::log("登录成功!");
            this->schedule(SEL_SCHEDULE(&GMainScene::updateServerState),0.5f);
        }
        else if(state == 0)
        {
		//	cocos2d::log("注册失败！");
        }
        else
        {
		//	cocos2d::log("登录失败！");
        }
    }
    else
    {
		//cocos2d::log("登录失败！");
    }
}


void GMainScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
