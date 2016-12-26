//
//  GGameScene2.cpp
//  Snake
//
//  Created by guang on 2016/12/23.
//
//

#include "GGameScene2.h"
#include "ui/GRockView.h"
#include "protocol_model/GModeGame.h"
#include "protocol_model/GModeUser.h"
#include "controller/GGameController.h"
#include "controller/GUserController.h"
#include "data/GCache.h"
#include "common/GCommon.h"
#include "ui/GRelived.h"
#include "ui/GGameOver.h"
#include "tools/GTools.h"

#define UPDATE_DT 0.1

USING_NS_CC;

Scene* GGameScene2::createScene()
{
    auto layer = GGameScene2::create();
    layer->setName("scene2");
    
    return layer;
}

bool GGameScene2::init()
{
    if ( !GScene::init() )
    {
        return false;
    }
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GGameScene2::onKeyReleased, this);
    listenerkeyPad->onKeyPressed = CC_CALLBACK_2(GGameScene2::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
    GGameController::getInstance()->init2(this);
    Size s = Director::getInstance()->getWinSize();
    
    
    char c[7];
    sprintf(c, "%d",GCache::getInstance()->getMapId());
    std::string mapPath = "map/map_";
    mapPath = mapPath + c + std::string(".tmx");
    tmxMap = TMXTiledMap::create(mapPath);
    tmxMap->setName("map");
    this->addChild(tmxMap,0);
    
    gameLayer = Layer::create();
    gameLayer->setName("gameLayer");
    gameLayer->setContentSize(Size(GCache::getInstance()->getRoomWidth(),
                                   GCache::getInstance()->getRoomHeight()));
    gameLayer->setPosition(GCache::getInstance()->getMapPosX(),
                           GCache::getInstance()->getMapPosY());
    tmxMap->addChild(gameLayer,8);
    
    
    
    blockLayer1 = SpriteBatchNode::create("wujian-A1.png");
    gameLayer->addChild(blockLayer1);
    
    blockLayer2 = SpriteBatchNode::create("wujian-A2.png");
    gameLayer->addChild(blockLayer2);
    
    blockLayer3 = SpriteBatchNode::create("wujian-A3.png");
    gameLayer->addChild(blockLayer3);
    
    blockLayer4 = SpriteBatchNode::create("wujian-A4.png");
    gameLayer->addChild(blockLayer4);
    
    blockLayer5 = SpriteBatchNode::create("wujian-A5.png");
    gameLayer->addChild(blockLayer5);
    
    blockLayer6 = SpriteBatchNode::create("wujian-A6.png");
    gameLayer->addChild(blockLayer6);
    
    bubbleLayer = SpriteBatchNode::create("she.png");
    bubbleLayer->setName("bubbleLayer");
    gameLayer->addChild(bubbleLayer);
    
    bubbleHeadLayer = Layer::create();
    bubbleHeadLayer->setName("bubbleHeadLayer");
    gameLayer->addChild(bubbleHeadLayer);
    
    initUI();
    initSprite();
    GTools::playMusic(MUSIC_FIGHT);
    return true;
}

void GGameScene2::initUI()
{
    Size s = Director::getInstance()->getWinSize();
    
    auto zhao = Sprite::create("jm-zz.png");
    zhao->setScale(2);
    zhao->setPosition(s.width/2,s.height/2);
    uiLayer->addChild(zhao);
    
    GRockView* view = GRockView::create("ui-yaogan1.png",
                                        "ui-yaogan2.png",
                                        "ui-yaogan2.png",50,
                                        GROCK_CALLBACK(GGameScene2::rockHandler, this));
    uiLayer->addChild(view);
    
//    auto timeBg = Sprite::create("ui-shijian1.png");
//    timeBg->setAnchorPoint(Vec2(0.5,1));
//    timeBg->setPosition(s.width/2,s.height-10);
//    uiLayer->addChild(timeBg);
//    
//    _countDown = GNumber::create(0);
//    _countDown->setAnchorPoint(Vec2(0,0.5));
//    _countDown->setScale(0.7f);
//    _countDown->useTimeStyle();
//    _countDown->setPosition(154,timeBg->getContentSize().height/2-2);
//    timeBg->addChild(_countDown);
    
    myExp = RichText::create();
    myExp->setAnchorPoint(Vec2(0.5,1));
    myExp->setPosition(Vec2(s.width/2,s.height-20));
    uiLayer->addChild(myExp);
    myExp->pushBackElement(RichElementText::create(0,Color3B::WHITE,255,_T("len"),"",24));
    myExp->pushBackElement(RichElementText::create(1,Color3B(245,230,126),255,"0","",24));
    myExp->pushBackElement(RichElementText::create(2,Color3B::WHITE,255,_T("mi"),"",24));
    
    wifi = Sprite::create("jm-wifi.png");
    wifi->setAnchorPoint(Vec2(0.5,1));
    wifi->setPosition(s.width/2-140, s.height-60);
    wifi->setVisible(false);
    uiLayer->addChild(wifi);
    
    Button* btn = Button::create("ui-anniu1.png","ui-anniu2.png");
    btn->setName("speed");
    btn->setAnchorPoint(Vec2(0.5,0));
    btn->setPosition(Vec2(s.width - btn->getContentSize().width/2 - 20,20));
    btn->addTouchEventListener(CC_CALLBACK_2(GGameScene2::touchEvent, this));
    uiLayer->addChild(btn);
    if(!GCache::isRightHand())
    {
        btn->setPosition(Vec2(btn->getContentSize().width/2 + 20,20));
    }
    rankView = GRankView::create();
    uiLayer->addChild(rankView);
    
    miniMap = GMiniMap::create();
    uiLayer->addChild(miniMap);
}

GBubbleSprite* GGameScene2::addRobot()
{
    GJsonObject* obj = GJsonObject::create(GCache::getInstance()->getBubble().c_str());
    GBubble* bubble = GBubble::create(obj);
    bubble->uid = StringUtils::format("robot_%d",++robotId);
    bubble->name = GCache::getInstance()->getName();
    bubble->x = random(300, GCache::getInstance()->getRoomWidth()-300);
    bubble->y = random(300, GCache::getInstance()->getRoomHeight()-300);
    bubble->skinId = random(1, GCache::getInstance()->getSkinNum());
    
    GBubbleSprite* robot = GBubbleSprite::create(bubble);
    GGameController::getInstance()->robots.push_back(robot);
    
    robot->setPosition(robot->bubble->x,robot->bubble->y);
    bubbleHeadLayer->addChild(robot,9999999);
    robot->initBody();
    
    this->bubble->bubble->robotUid.push_back(bubble->uid);
    return robot;
}

void GGameScene2::addSelf()
{
    GJsonObject* obj = GJsonObject::create(GCache::getInstance()->getBubble().c_str());
    GBubble* bubble = GBubble::create(obj);
    bubble->robot = false;
    bubble->uid = GCache::getInstance()->getUser()->uid;
    bubble->name = GCache::getInstance()->getUser()->name;
    bubble->x = random(300, GCache::getInstance()->getRoomWidth()-300);
    bubble->y = random(300, GCache::getInstance()->getRoomHeight()-300);
    bubble->skinId = GCache::getInstance()->getUser()->skinId;
    
    this->bubble = GBubbleSprite::create(bubble);
    GGameController::getInstance()->bubble = this->bubble;
    this->bubble->setPosition(this->bubble->bubble->x,this->bubble->bubble->y);
    bubbleHeadLayer->addChild(this->bubble,9999999);
    this->bubble->initBody();
}

void GGameScene2::addBlock()
{
    GJsonObject* obj = GJsonObject::create();
    obj->putInt("id", ++blockId);
    obj->putInt("type", 0);
    obj->putInt("blockType", random(1, 6));
    obj->putInt("exp", 1);
    obj->putDouble("x", random(80.f, GCache::getInstance()->getRoomWidth()-80.f));
    obj->putDouble("y", random(80.f, GCache::getInstance()->getRoomHeight()-80.f));
    obj->putString("state", "IDLE");
    
    GBlock* bl = GBlock::create(obj);
    GBlockSprite* block = GBlockSprite::create(bl);
    block->setPosition(block->block->x,block->block->y);
    
    if(block->block->blockType == 1)
    {
        block->init(blockLayer1->getTexture());
        blockLayer1->addChild(block);
    }
    else if(block->block->blockType == 2)
    {
        block->init(blockLayer2->getTexture());
        blockLayer2->addChild(block);
    }
    else if(block->block->blockType == 3)
    {
        block->init(blockLayer3->getTexture());
        blockLayer3->addChild(block);
    }
    else if(block->block->blockType == 4)
    {
        block->init(blockLayer4->getTexture());
        blockLayer4->addChild(block);
    }
    else if(block->block->blockType == 5)
    {
        block->init(blockLayer5->getTexture());
        blockLayer5->addChild(block);
    }
    else if(block->block->blockType == 6)
    {
        block->init(blockLayer6->getTexture());
        blockLayer6->addChild(block);
    }
    
    GGameController::getInstance()->addBlock(block);
}

void GGameScene2::eatBlock(std::string &uid,int _id,float x,float y)
{
    GBubbleSprite* sp = nullptr;
    bool isSelf = false;
    if(uid == GCache::getInstance()->getUser()->uid)
    {
        sp = bubble;
        isSelf = true;
    }
    else
    {
        sp = GGameController::getInstance()->findRobotByUid(uid);
    }
    
    GBlockSprite* ps = GGameController::getInstance()->findBlock(_id,x,y);
    if(ps)
    {
        int type = ps->block->type;
        int exp = ps->block->exp;
        ps->die(sp);
        
        if(sp)
        {
            if(type == 0 || type == 1)
            {
                sp->bubble->exp += exp;
                int level = GCache::getInstance()->getLevel(sp->bubble->exp);
                float grow = GCache::getInstance()->getGrow(level);
                
                bool up = sp->bubble->level != level;
                sp->bubble->level = level;
                sp->bubble->grow = grow;
                sp->updateExp(up,false);
                updateExp();
                cameraScale();
            }
        }
    }
}

//自定义排序函数
bool SortRank( const GRank &v1, const GRank &v2)
{
    return v1.exp > v2.exp;//降序排列
}

void GGameScene2::coll(GBubbleSprite * bubble,GBubbleSprite * target)
{
    if(bubble)
    {
        bubble->bubble->kill += 1;
        long time = GTools::getNowTime();
        if(time - bubble->bubble->killTime < 5*1000)
        {
            bubble->bubble->killNum += 1;
        }
        else
        {
            bubble->bubble->killNum = 1;
        }
        bubble->bubble->killTime = time;
    }
    if(target)
    {
        target->changeState(GBubble::State::DIE);
        target->bubble->die += 1;
        
        //先计算排名
        std::vector<GRank> ranks;
        for(int i=0;i<=GGameController::getInstance()->robots.size();i++)
        {
            GBubbleSprite* sp = nullptr;
            if(i==GGameController::getInstance()->robots.size())
            {
                sp = this->bubble;
            }
            else
            {
                sp = GGameController::getInstance()->robots.at(i);
            }
            GRank rank;
            rank.uid = sp->bubble->uid;
            rank.name = sp->bubble->name;
            rank.exp = sp->bubble->exp;
            rank.kill = sp->bubble->killNum;
            rank.die = sp->bubble->die;
            rank.rewardNum = 0;
            rank.rank = 0;
            ranks.push_back(rank);
        }
        std::sort(ranks.begin(), ranks.end(), SortRank);
        
        int rank = 1;
        for(int i=0;i<ranks.size();i++)
        {
            if(ranks.at(i).uid == target->bubble->uid)
            {
                rank = i+1;
                break;
            }
        }
        
        GJsonObject* dieData = GJsonObject::create();
        if(bubble)
        {
            dieData->putString("killMe", bubble->bubble->uid);
            dieData->putString("killMeName", bubble->bubble->name);
            dieData->putInt("killNum", bubble->bubble->killNum);
        }
        else
        {
            dieData->putString("killMe", "0");
            dieData->putString("killMeName", "0");
            dieData->putInt("killNum", 1);
        }
        dieData->putInt("rank", rank);
        dieData->putInt("exp", target->bubble->exp);
        dieData->putInt("kill", target->bubble->killNum);

        
        target->die2();
        updateRobotEat();
        if(target == this->bubble)
        {
            openRelived(dieData);
        }
        else
        {
            //先判断排名是否小于10
            if(rank <= 10)
            {
                std::string killName = target->bubble->name;
                std::string killMeName = dieData->getString("killMeName");
                std::string killMe = dieData->getString("killMe");
                //是否是自己击杀的
                if(killMe == GCache::getInstance()->getUser()->uid)
                {
                    int killNum = dieData->getInt("killNum");
                    if(killNum > 1 && killNum <= 5)
                    {
                        char c[7];
                        sprintf(c, "%d",killNum);
                        std::string t = "prompt_kill_";
                        t += c;
                        GTools::showTost2(nullptr, _T(t));
                    }
                    else if(killNum > 5)
                    {
                        GTools::showTost2(nullptr, _T("prompt_kill"));
                    }
                    else
                        GTools::showTost2(nullptr, _T("prompt_2")+killName);
                    
                    //判断是否是上次击杀自己的人
                    if(GCache::getInstance()->getKillMeUid() != ""
                       && GCache::getInstance()->getKillMeUid() == target->bubble->uid)
                    {
                        GCache::getInstance()->setKillMeUid("");
                        GTools::showTost(nullptr, _T("prompt_3"),0.7f);
                    }
                }
                else
                {
                    GTools::showTost2(nullptr, killName+_T("prompt_1")+killMeName);
                }
            }
        }
    }
}

void GGameScene2::drop(GBubbleSprite * bubble)
{
    int size = ((int)bubble->bodys.size() / 2);
    int exp = bubble->bubble->exp / size;
    if(exp < 1)
        exp = 1;
    
    for(int i=0;i<size;i++)
    {
        Vec2 v = bubble->bodys.at(random(0, (int)bubble->bodys.size()-1))->getPosition();
        
        GJsonObject* obj = GJsonObject::create();
        obj->putInt("id", ++blockId);
        obj->putInt("type", 1);
        obj->putInt("blockType", random(1, 6));
        obj->putInt("exp", exp);
        int r = random(0, 100);
        if(r < 50)
        {
            v.x -= random(0, 20);
            v.y += random(0, 20);
        }
        else
        {
            v.x += random(0, 20);
            v.y -= random(0, 20);
        }
        obj->putDouble("x", v.x);
        obj->putDouble("y", v.y);
        obj->putString("state", "IDLE");
        
        GBlock* bl = GBlock::create(obj);
        GBlockSprite* block = GBlockSprite::create(bl);
        block->setPosition(block->block->x,block->block->y);
        
        if(block->block->blockType == 1)
        {
            block->init(blockLayer1->getTexture());
            blockLayer1->addChild(block);
        }
        else if(block->block->blockType == 2)
        {
            block->init(blockLayer2->getTexture());
            blockLayer2->addChild(block);
        }
        else if(block->block->blockType == 3)
        {
            block->init(blockLayer3->getTexture());
            blockLayer3->addChild(block);
        }
        else if(block->block->blockType == 4)
        {
            block->init(blockLayer4->getTexture());
            blockLayer4->addChild(block);
        }
        else if(block->block->blockType == 5)
        {
            block->init(blockLayer5->getTexture());
            blockLayer5->addChild(block);
        }
        else if(block->block->blockType == 6)
        {
            block->init(blockLayer6->getTexture());
            blockLayer6->addChild(block);
        }
        
        GGameController::getInstance()->addBlock(block);
    }
}

void GGameScene2::initSprite()
{
    GGameController::getInstance()->blocks.clear();
    GGameController::getInstance()->robots.clear();
    GGameController::getInstance()->bubbles.clear();
    for(int i=0;i<GCache::getMapPlitNum()+4;i++)
    {
        std::vector<GBlockSprite*> blocks1;
        GGameController::getInstance()->blocks.push_back(blocks1);
    }
    GCache::getInstance()->setKillMeUid("");
    
    robotId = 0;
    blockId = 0;

    
    blockLayer1->removeAllChildren();
    blockLayer2->removeAllChildren();
    blockLayer3->removeAllChildren();
    blockLayer4->removeAllChildren();
    blockLayer5->removeAllChildren();
    blockLayer6->removeAllChildren();
    bubbleLayer->removeAllChildren();
    bubbleHeadLayer->removeAllChildren();

    //自己泡泡
    addSelf();
    //机器人
    for(int i=0;i<10;i++)
    {
        addRobot();
    }
    //水滴
    for(int i=0;i<3000;i++)
    {
        addBlock();
    }
    
    startHeartBeat();
    _start = false;
    lastPos = Vec2(-1000,-1000);
    robotDt = 1000;
    robotNum = 0;
    robotCollDt = 0;
    crystalsDt = 0;
    blockDt = 0;
    rankDt = 0;
    miniMap->initDta();

    rollBg(0);
    
    start();
}

void GGameScene2::start()
{
    _start = true;
    this->schedule(SEL_SCHEDULE(&GGameScene::update),1/60.f);
    
    GTools::playSound(SOUND_START);
    
    Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
    bubble->changeState(GBubble::State::MOVE);
    bubble->playRelive();
    
    cameraScale();
}

void GGameScene2::update(float dt)
{
    updateCollBubble(dt);
    updateCollBubbleAndBlock(dt);
    updateRobot(dt);
    updateAllPos(dt);
    updateRank(dt);
    miniMap->update(dt);
    
    rollBg(dt);
}

void GGameScene2::rollBg(float dt)
{
    Size s = Director::getInstance()->getWinSize();
    
    float x = bubble->getPositionX() + gameLayer->getPositionX();
    float y = bubble->getPositionY() + gameLayer->getPositionY();
    //    tmxMap->runAction(MoveTo::create(dt, Vec2(-x+s.width/2, -y+s.height/2)));
    //    tmxMap->setPosition(-x+s.width/2, -y+s.height/2);
    
    this->getDefaultCamera()->setPosition(x, y);
    uiLayer->setPosition(x-s.width/2, y-s.height/2);
    
    int z = this->getDefaultCamera()->getPositionZ();
    if(z != cameraZ)
    {
        cameraZDt += dt;
        if(cameraZDt > 1/30.f)
        {
            cameraZDt = 0;
            if(z < cameraZ)
                z ++;
            else
                z --;
            this->getDefaultCamera()->setPositionZ(z);
            uiLayer->setScale(z/622.f);
        }
    }
    
}

void GGameScene2::cameraScale()
{
    int sc = bubble->bubble->grow;
    if(sc > 0)
    {
        cameraZ = 622.f*(1+sc/6.f);
    }
    else
    {
        cameraZ = 622;
    }
}

void GGameScene2::cameraReset()
{
    Size s = Director::getInstance()->getWinSize();
    float x = bubble->getPositionX() + gameLayer->getPositionX();
    float y = bubble->getPositionY() + gameLayer->getPositionY();
    
    tmxMap->setPosition(-x+s.width/2, -y+s.height/2);
    this->getDefaultCamera()->setPosition(s.width/2, s.height/2);
    uiLayer->setPosition(0, 0);
}

void GGameScene2::cameraResetClose()
{
    Size s = Director::getInstance()->getWinSize();
    float x = bubble->getPositionX() + gameLayer->getPositionX();
    float y = bubble->getPositionY() + gameLayer->getPositionY();
    
    tmxMap->setPosition(0,0);
    this->getDefaultCamera()->setPosition(x, y);
    uiLayer->setPosition(x-s.width/2, y-s.height/2);
}

bool GGameScene2::isStart()
{
    return this->_start;
}

void GGameScene2::updateRank(float dt)
{
    rankDt += dt;
    if(rankDt < 1)
        return;
    rankDt = 0;
    //先计算排名
    std::vector<GRank> ranks;
    for(int i=0;i<=GGameController::getInstance()->robots.size();i++)
    {
        GBubbleSprite* sp = nullptr;
        if(i==GGameController::getInstance()->robots.size())
        {
            sp = this->bubble;
        }
        else
        {
            sp = GGameController::getInstance()->robots.at(i);
        }
        GRank rank;
        rank.uid = sp->bubble->uid;
        rank.name = sp->bubble->name;
        rank.exp = sp->bubble->exp;
        rank.kill = sp->bubble->killNum;
        rank.die = sp->bubble->die;
        rank.rewardNum = 0;
        rank.rank = 0;
        ranks.push_back(rank);
    }
    std::sort(ranks.begin(), ranks.end(), SortRank);
    
    GJsonArray* arr = GJsonArray::create();
    for(int i=0;i<ranks.size();i++)
    {
        GRank rank = ranks.at(i);
        rank.rank = i+1;
        
        GJsonObject* obj = GJsonObject::create();
        obj->putString("uid", rank.uid);
        obj->putString("name", rank.name);
        obj->putInt("exp", rank.exp);
        obj->putInt("kill", rank.kill);
        obj->putInt("die", rank.die);
        obj->putInt("rewardNum", rank.rewardNum);
        obj->putInt("rank", rank.rank);
        
        arr->add(obj);
    }

    rankView->updateData(arr);
}

void GGameScene2::end()
{
    this->unschedule(SEL_SCHEDULE(&GGameScene::update));
    _start = false;
    
    for(int i=0;i< bubble->bubble->robotUid.size();i++)
    {
        std::string uid = bubble->bubble->robotUid.at(i);
        GBubbleSprite * bubble = GGameController::getInstance()->findRobotByUid(uid);
        if(bubble)
        {
            //            bubble->stopAttack();
        }
    }
    //    this->bubble->stopAttack();
}

void GGameScene2::recConn()
{
    end();
    GModeUser::recConn("game2");
}

void GGameScene2::leaveRoom()
{
    GGameController::getInstance()->init2(nullptr);
    end();
    GUserController::getInstance()->enterHomeScene();
}

void GGameScene2::rockHandler(cocos2d::Vec2 &dir,float power,const char *data)
{
    if(_start)
    {
        //更新自己方向
        if(data == std::string("move"))
        {
            moveBtn = true;
            bubble->bubble->dirX = dir.x;
            bubble->bubble->dirY = dir.y;
            if(speedUpBtn)
                bubble->changeState(GBubble::State::SPEEDUP);
            else
                bubble->changeState(GBubble::State::MOVE);
            bubble->updateAngle(dir);
        }
        //        else if(data == std::string("rotate"))
        //        {
        //            bubble->move(dir, 0);
        //        }
        else
        {
            //            moveBtn = false;
            //            if(!speedUpBtn)
            //            GModeGame::stopMove(bubble->bubble);
        }
    }
}

void GGameScene2::updateCountDown(int time)
{
    if(_start)
    {
        roomTime = time/1000;
        _countDown->setNum(roomTime);
        
    }
    
    if(pingNum > 260)
    {
        wifi->runAction(Sequence::create(FadeIn::create(0.1f),
                                         Blink::create(0.5f, 2),
                                         FadeOut::create(0.1f),
                                         NULL));
    }
}
void GGameScene2::updateExp()
{
    char c[7];
    sprintf(c, "%d",bubble->bubble->exp);
    myExp->removeElement(1);
    myExp->insertElement(RichElementText::create(1, Color3B(245,230,126), 255, c, "", 24),1);
}

void GGameScene2::openRelived(GJsonObject* obj)
{
    this->obj = obj;
    Node * node = uiLayer->getChildByName("relived");
    if(!node)
    {
        this->obj->retain();
        this->runAction(Sequence::create(DelayTime::create(3),
                                         CallFunc::create(
                                                          // lambda
                                                          [&](){
                                                              GRelived* relived = GRelived::create();
                                                              relived->setGame2(true);
                                                              relived->initUI(this->obj);
                                                              relived->setName("relived");
                                                              uiLayer->addChild(relived);
                                                              this->obj->release();
                                                              
                                                          }  ), NULL));
        
    }
}

void GGameScene2::reloved()
{
    std::vector<std::string> robotUid = bubble->bubble->robotUid;
    int kill = bubble->bubble->kill;
    int die = bubble->bubble->die;
    int killNum = bubble->bubble->killNum;
    
    bubble->removeFromParent();
    bubble = nullptr;
    
    addSelf();
    bubble->bubble->robotUid = robotUid;
    bubble->bubble->kill = kill;
    bubble->bubble->die = die;
    bubble->bubble->killNum = killNum;
    bubble->playRelive();
    Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
    bubble->changeState(GBubble::State::MOVE);
    bubble->updateAngle(dir);
    
    miniMap->updateTarget();
    
    GTools::playSound(SOUND_START);
}

void GGameScene2::robotReloved(GBubbleSprite * robot)
{
    int kill = robot->bubble->kill;
    int die = robot->bubble->die;
    int killNum = robot->bubble->killNum;
    int skinId = robot->bubble->skinId;
    std::string uid = robot->bubble->uid;
    std::string name = robot->bubble->name;
    
    GJsonObject* obj = GJsonObject::create(GCache::getInstance()->getBubble().c_str());
    GBubble* bubble = GBubble::create(obj);
    bubble->uid = uid;
    bubble->name = name;
    bubble->x = random(300, GCache::getInstance()->getRoomWidth()-300);
    bubble->y = random(300, GCache::getInstance()->getRoomHeight()-300);
    bubble->skinId = skinId;
    bubble->killNum = killNum;
    bubble->kill = kill;
    bubble->die = die;
    
    GBubbleSprite* robot2 = GBubbleSprite::create(bubble);
    GGameController::getInstance()->robots.push_back(robot2);
    
    robot2->setPosition(robot2->bubble->x,robot2->bubble->y);
    bubbleHeadLayer->addChild(robot2,9999999);
    robot2->initBody();
    robot2->playRelive();
    
    this->bubble->bubble->robotUid.push_back(bubble->uid);
    
    miniMap->add(robot2);
}

void GGameScene2::openGameOver(GJsonObject* obj)
{
    Node * node = uiLayer->getChildByName("gameover");
    if(!node)
    {
        end();
        
        uiLayer->removeChildByName("relived");
        auto over = GGameOver::create();
        over->updateData(obj);
        over->setName("gameover");
        uiLayer->addChild(over);
    }
}

void GGameScene2::updateCollBubble(float dt)
{
    float headDis = 30;
    if(bubble->bubble->state != GBubble::State::DIE && !bubble->isColl && !bubble->isNew)
    {
        //和机器人
        for(int i=0;i<GGameController::getInstance()->robots.size();i++)
        {
            GBubbleSprite* sp = GGameController::getInstance()->robots.at(i);
            if(sp->bubble->state != GBubble::State::DIE && !sp->isColl && GGameController::getInstance()->isContain(bubble, sp))
            {
                for(int j=0;j<sp->bodys.size();j++)
                {
                    GBodySprite* body = sp->bodys.at(j);
                    float dis = bubble->getUpdatePosition().getDistance(body->getPosition());
                    if(dis < headDis + body->getCollSize().width/4*body->getScale())
                    {
                        //                        bubble->changeState(GBubble::State::DIE);
                        bubble->coll();
                        this->coll(sp, this->bubble);
                        break;
                    }
                }
            }
            if(bubble->bubble->state == GBubble::State::DIE || bubble->isColl)
                break;
        }
        //和边界
        if(bubble->isCollWall())
        {
            bubble->changeState(GBubble::State::DIE);
            this->coll(nullptr, this->bubble);
        }
    }
    
    //自己的机器人
    if(bubble->bubble->robotUid.size() > 0)
    {
        for(int i=0;i<bubble->bubble->robotUid.size();i++)
        {
            std::string uid = bubble->bubble->robotUid.at(i);
            GBubbleSprite * sp = GGameController::getInstance()->findRobotByUid(uid);
            if(sp && sp->bubble->state != GBubble::State::DIE && !sp->isColl && !sp->isNew)
            {
                //和自己
                if(bubble->bubble->state != GBubble::State::DIE && !bubble->isColl && GGameController::getInstance()->isContain(sp, this->bubble))
                {
                    for(int j=0;j<bubble->bodys.size();j++)
                    {
                        GBodySprite* body = bubble->bodys.at(j);
                        float dis = sp->getUpdatePosition().getDistance(body->getPosition());
                        if(dis < headDis + body->getCollSize().width/4*body->getScale())
                        {
                            //                            sp->changeState(GBubble::State::DIE);
                            sp->coll();
                            this->coll(this->bubble, sp);
                            break;
                        }
                    }
                }
                
                //和机器人
                for(int i=0;i<GGameController::getInstance()->robots.size();i++)
                {
                    GBubbleSprite* sp2 = GGameController::getInstance()->robots.at(i);
                    if(sp2->bubble->state != GBubble::State::DIE && sp != sp2 && !sp2->isColl &&GGameController::getInstance()->isContain(sp, sp2))
                    {
                        for(int j=0;j<sp2->bodys.size();j++)
                        {
                            GBodySprite* body = sp2->bodys.at(j);
                            float dis = sp->getUpdatePosition().getDistance(body->getPosition());
                            if(dis < headDis + body->getCollSize().width/4*body->getScale())
                            {
                                //                                sp->changeState(GBubble::State::DIE);
                                sp->coll();
                                this->coll(sp2, sp);
                                break;
                            }
                        }
                    }
                    if(sp->bubble->state == GBubble::State::DIE || sp->isColl)
                        break;
                }
                
            }
        }
    }
}

void GGameScene2::updateCollBubbleAndBlock(float dt)
{
    crystalsDt += dt;
    if(crystalsDt > 1/40.f)
    {
        crystalsDt = 0;
        
        float w = GCache::getInstance()->getRoomWidth()/GCache::getMapPlitNum();
        int pnum = GCache::getMapPlitNum();
        int row = bubble->getPositionX()/w;
        
        //自己
        if(bubble->bubble->state != GBubble::State::DIE)
        {
            for(int i=row-1;i<row+2;i++)
            {
                if(i >= 0 && i < pnum)
                {
                    for(int j=0;j<GGameController::getInstance()->blocks[i].size();j++)
                    {
                        GBlockSprite* sp = GGameController::getInstance()->blocks[i].at(j);
                        if(sp->block->state != GBlock::State::DIE)
                        {
                            float dis = bubble->getUpdatePosition().getDistance(sp->getPosition());
                            if(dis < bubble->getCollAndBlock())
                            {
                                sp->block->state = GBlock::State::DIE;
                                eatBlock(bubble->bubble->uid,sp->block->_id,sp->block->x,sp->block->y);
                            }
                        }
                    }
                }
            }
            
        }
        
        //机器人
        if(bubble->bubble->robotUid.size() > 0)
        {
            for(int i=0;i<bubble->bubble->robotUid.size();i++)
            {
                std::string uid = bubble->bubble->robotUid.at(i);
                GBubbleSprite * bubble = GGameController::getInstance()->findRobotByUid(uid);
                if(bubble && bubble->bubble->state != GBubble::State::DIE)
                {
                    int row = bubble->getPositionX()/w;
                    if(row < 0 || row >= pnum)
                        return;
                    
                    for(int j=0;j<GGameController::getInstance()->blocks[row].size();j++)
                    {
                        GBlockSprite* sp = GGameController::getInstance()->blocks[row].at(j);
                        if(sp->block->state != GBlock::State::DIE)
                        {
                            float dis = bubble->getUpdatePosition().getDistance(sp->getPosition());
                            if(dis < bubble->getCollAndBlock())
                            {
                                sp->block->state = GBlock::State::DIE;
                                eatBlock(bubble->bubble->uid,sp->block->_id,sp->block->x,sp->block->y);
                                break;
                            }
                        }
                    }
                }
                
            }
        }
        
    }
    
    blockDt += dt;
    if(blockDt > 5)
    {
        blockDt = 0;
        
        int num = 0;
        for(int i=0;i<GGameController::getInstance()->blocks.size();i++)
        {
            num += GGameController::getInstance()->blocks[i].size();
        }
        num = 3000 - num;
        if(num > 0)
        {
            for(int i=0;i<num;i++)
            {
                addBlock();
            }
        }
        
    }
}

void GGameScene2::updateRobot(float dt)
{
    int size = (int)bubble->bubble->robotUid.size();
    if(size > 0)
    {
        robotDt+=dt;
        robotCollDt+=dt;
        robotAvoidDt+=dt;
        if(robotDt > 0.5f)
        {
            robotDt = 0;
            
            if(robotNum < size)
            {
                std::string uid = bubble->bubble->robotUid.at(robotNum);
                GBubbleSprite * bubble = GGameController::getInstance()->findRobotByUid(uid);
                if(bubble && bubble->bubble->state != GBubble::State::DIE)
                {
                    updateRobotState(bubble);
                }
            }
            else
            {
                robotNum = -1;
            }
            robotNum++;
        }
        
        //        if(robotCollDt > 0.3)
        //        {
        //            robotCollDt = 0;
        //
        //            for(int i=0;i< bubble->bubble->robotUid.size();i++)
        //            {
        //                std::string uid = bubble->bubble->robotUid.at(i);
        //                GBubbleSprite * bubble = GGameController::getInstance()->findRobotByUid(uid);
        //                if(bubble && bubble->bubble->state != GBubble::State::DIE)
        //                {
        //                    updateRobotState(bubble);
        //                }
        //            }
        //        }
        
        if(robotAvoidDt > 0.1f)
        {
            robotAvoidDt = 0;
            
            updateRobotAvoid();
        }
        
    }
}

void GGameScene2::updateRobotState(GBubbleSprite * bubble)
{
    int state = random(0, 10);
    //吃水滴
    if(state < 12)
    {
        //根据水滴找到方向
        int size = (int)GGameController::getInstance()->blocks.size() - 1;
        if(size <= 0)
            return;
        int r = random(0, size);
        int size2 = (int)GGameController::getInstance()->blocks[r].size() - 1;
        if(size2 <= 0)
            return;
        int r2 = random(0, size2);
        GBlockSprite* block = GGameController::getInstance()->blocks[r].at(r2);
        
        Vec2 dir = (block->getPosition() - bubble->getPosition()).getNormalized();
        
        bubble->bubble->dirX = dir.x;
        bubble->bubble->dirY = dir.y;
        bubble->changeState(GBubble::State::MOVE);
        if(state == 5)
            bubble->changeState(GBubble::State::SPEEDUP);
        bubble->updateAngle(dir);
    }
}

void GGameScene2::updateRobotAvoid()
{
    //躲避
    //自己的机器人
    if(bubble->bubble->robotUid.size() > 0)
    {
        for(int i=0;i<bubble->bubble->robotUid.size();i++)
        {
            std::string uid = bubble->bubble->robotUid.at(i);
            GBubbleSprite * sp = GGameController::getInstance()->findRobotByUid(uid);
            if(sp && sp->bubble->state != GBubble::State::DIE)
            {
                bool b = false;
                //和墙壁
                if(sp->isCollWall())
                {
                    int size = (int)GGameController::getInstance()->blocks.size() - 1;
                    if(size <= 0)
                        continue;
                    int r = random(0, size);
                    int size2 = (int)GGameController::getInstance()->blocks[r].size() - 1;
                    if(size2 <= 0)
                        continue;
                    int r2 = random(0, size2);
                    GBlockSprite* block = GGameController::getInstance()->blocks[r].at(r2);
                    Vec2 dir = (block->getPosition() - sp->getPosition()).getNormalized();
                    
                    sp->bubble->dirX = dir.x;
                    sp->bubble->dirY = dir.y;
                    sp->changeState(GBubble::State::MOVE);
                    sp->updateAngle(dir);
                    b = true;
                    continue;
                }
                //和自己
                if(GGameController::getInstance()->isContain(sp, this->bubble))
                {
                    for(int j=0;j<bubble->bodys.size();j++)
                    {
                        GBodySprite* body = bubble->bodys.at(j);
                        float dis = sp->getUpdatePosition().getDistance(body->getPosition());
                        if(dis < sp->getContentSize().width/2 + body->getCollSize().width/2*body->getScale() * 5)
                        {
                            Vec2 dir = (sp->getPosition() - body->getPosition()).getNormalized();
                            
                            sp->bubble->dirX = dir.x;
                            sp->bubble->dirY = dir.y;
                            sp->changeState(GBubble::State::MOVE);
                            sp->updateAngle(dir);
                            b = true;
                            break;
                        }
                    }
                }
                if(b)
                    continue;
                //和机器人
                for(int i=0;i<GGameController::getInstance()->robots.size();i++)
                {
                    GBubbleSprite* sp2 = GGameController::getInstance()->robots.at(i);
                    if(sp2->bubble->state != GBubble::State::DIE && sp != sp2 && GGameController::getInstance()->isContain(sp, sp2))
                    {
                        for(int j=0;j<sp2->bodys.size();j++)
                        {
                            GBodySprite* body = sp2->bodys.at(j);
                            float dis = sp->getUpdatePosition().getDistance(body->getPosition());
                            if(dis < sp->getContentSize().width/2 + body->getCollSize().width/2*body->getScale() * 5)
                            {
                                Vec2 dir = (sp->getPosition() - body->getPosition()).getNormalized();
                                
                                sp->bubble->dirX = dir.x;
                                sp->bubble->dirY = dir.y;
                                sp->changeState(GBubble::State::MOVE);
                                sp->updateAngle(dir);
                                b = true;
                                break;
                            }
                        }
                    }
                    if(sp->bubble->state == GBubble::State::DIE || b)
                        break;
                }
                //如果没有闪躲
                if(!b)
                {
                    //先找经验是2的水晶
                    float w = GCache::getInstance()->getRoomWidth()/GCache::getMapPlitNum();
                    int pnum = GCache::getMapPlitNum();
                    int row = bubble->getPositionX()/w;
                    
                    GBlockSprite* block = nullptr;
                    
                    for(int i=row-2;i<row+2;i++)
                    {
                        if(i >= 0 && i < pnum)
                        {
                            for(int j=0;j<GGameController::getInstance()->blocks[i].size();j++)
                            {
                                GBlockSprite* sp = GGameController::getInstance()->blocks[i].at(j);
                                if(sp->block->state != GBlock::State::DIE && sp->block->type == 1)
                                {
                                    float dis = bubble->getUpdatePosition().getDistance(sp->getPosition());
                                    if(dis < 800)
                                    {
                                        block = sp;
                                        break;
                                    }
                                }
                            }
                        }
                        if(block)
                            break;
                    }
                    if(block)
                    {
                        Vec2 dir = (block->getPosition() - sp->getPosition()).getNormalized();
                        
                        sp->bubble->dirX = dir.x;
                        sp->bubble->dirY = dir.y;
                        sp->changeState(GBubble::State::SPEEDUP);
                        sp->updateAngle(dir);
                    }
                }
                
                
            }
        }
    }
    
}

void GGameScene2::updateRobotEat()
{
    //自己的机器人
    if(bubble->bubble->robotUid.size() > 0)
    {
        for(int i=0;i<bubble->bubble->robotUid.size();i++)
        {
            std::string uid = bubble->bubble->robotUid.at(i);
            GBubbleSprite * sp = GGameController::getInstance()->findRobotByUid(uid);
            
            if(sp && sp->bubble->state != GBubble::State::DIE)
            {
                updateRobotState(sp);
            }
        }
    }
}

void GGameScene2::updateAllPos(float dt)
{
    updatePosDt+=dt;
    
//    for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
//    {
//        GBubbleSprite * bubble = GGameController::getInstance()->bubbles.at(i);
//        if(bubble->bubble->state != GBubble::State::DIE)
//        {
//            bubble->update2(dt);
//            bubble->updatePosAndRotate();
//            bubble->bubble->resetPosDt += dt;
//        }
//    }
    for(int i=0;i<GGameController::getInstance()->robots.size();i++)
    {
        GBubbleSprite * bubble = GGameController::getInstance()->robots.at(i);
        if(bubble->bubble->state != GBubble::State::DIE)
        {
            bubble->update2(dt);
            bubble->updatePosAndRotate();
            bubble->bubble->resetPosDt += dt;
        }
    }
    bubble->update2(dt);
    bubble->updatePosAndRotate();
    bubble->bubble->resetPosDt += dt;
    
    //    if(updatePosDt > 1)
    //    {
    //        updatePosDt = 0;
    //        log("robots=%d  %d",(int)GGameController::getInstance()->robots.size(),(int)bubble->bubble->robotUid.size());
    //    }
    
    //    if(updatePosDt > UPDATE_DT)
    //    {
    //        updatePosDt = 0;
    //
    //        GModeGame::updatePos(bubble->bubble);
    //
    //        for(int j=0;j<GGameController::getInstance()->robots.size();j++)
    //        {
    //            GBubbleSprite * bubble = GGameController::getInstance()->robots.at(j);
    //            if(GGameController::getInstance()->isSelfBubble(bubble->bubble))
    //            {
    //                GModeGame::updatePos(bubble->bubble);
    //            }
    //        }
    //    }
}


void GGameScene2::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    std::string name = btn->getName();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            //            GTools::playSound(SOUND_BUTTON);
            if(name == "speed")
            {
                speedUpBtn = true;
                if(bubble->bubble->state != GBubble::State::DIE)
                {
                    //                    GModeGame::speedUp(bubble->bubble);
                    Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
                    bubble->changeState(GBubble::State::SPEEDUP);
                    bubble->updateAngle(dir);
                    
                    GTools::playSound(SOUND_SPEED);
                }
            }
            else if(name == "attack")
            {
                //                bubble->startAttack();
            }
            
            break;
            
        case Widget::TouchEventType::MOVED:
            
            break;
            
        case Widget::TouchEventType::ENDED:
            
            if(name == "speed")
            {
                speedUpBtn = false;
                if(bubble->bubble->state != GBubble::State::DIE)
                {
                    //                    GModeGame::stopSpeedUp(bubble->bubble);
                    if(moveBtn)
                    {
                        Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
                        bubble->changeState(GBubble::State::MOVE);
                        bubble->updateAngle(dir);
                    }
                    else
                    {
                        //                        GModeGame::stopMove(bubble->bubble);
                    }
                }
            }
            else if(name == "attack")
            {
                //                bubble->stopAttack();
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            if(name == "speed")
            {
                speedUpBtn = false;
                if(bubble->bubble->state != GBubble::State::DIE)
                {
                    //                    GModeGame::stopSpeedUp(bubble->bubble);
                    if(moveBtn)
                    {
                        Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
                        bubble->changeState(GBubble::State::MOVE);
                        bubble->updateAngle(dir);
                    }
                    else
                    {
                        //                        GModeGame::stopMove(bubble->bubble);
                    }
                }
            }
            else if(name == "attack")
            {
                //                bubble->stopAttack();
            }
            
            break;
            
        default:
            break;
    }
}
void GGameScene2::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* unused_event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_Z)
    {
        //       bubble->startAttack();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_X)
    {
        speedUpBtn = true;
        if(bubble->bubble->state != GBubble::State::DIE)
        {
            //                    GModeGame::speedUp(bubble->bubble);
            Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
            bubble->changeState(GBubble::State::SPEEDUP);
            bubble->updateAngle(dir);
            
            GTools::playSound(SOUND_SPEED);
        }
    }
    
}
void GGameScene2::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_Z)
    {
        //       bubble->stopAttack();
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_X)
    {
        speedUpBtn = false;
        if(bubble->bubble->state != GBubble::State::DIE)
        {
            //            GModeGame::stopSpeedUp(bubble->bubble);
            if(moveBtn)
            {
                Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
                bubble->changeState(GBubble::State::MOVE);
                bubble->updateAngle(dir);
            }
            else
            {
                //                GModeGame::stopMove(bubble->bubble);
            }
            
        }
    }
}
