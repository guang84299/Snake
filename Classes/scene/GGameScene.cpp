//
//  GGameScene.cpp
//  OnlineGame
//
//  Created by guang on 16/10/2.
//
//

#include "GGameScene.h"
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

#define UPDATE_DT 5

USING_NS_CC;

Scene* GGameScene::createScene()
{
    auto layer = GGameScene::create();
    layer->setName("scene");
    
    return layer;
}

bool GGameScene::init()
{
    if ( !GScene::init() )
    {
        return false;
    }

    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GGameScene::onKeyReleased, this);
    listenerkeyPad->onKeyPressed = CC_CALLBACK_2(GGameScene::onKeyPressed, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
    GGameController::getInstance()->init(this);
    
    Size s = Director::getInstance()->getWinSize();

    char c[7];
    sprintf(c, "%d",GCache::getInstance()->getMapId());
    std::string mapPath = "map/map_";
    mapPath = mapPath + c + std::string(".tmx");
    tmxMap = TMXTiledMap::create(mapPath);
    tmxMap->setName("map");
    this->addChild(tmxMap,0);
    
    for(Node* node : tmxMap->getChildren())
    {
        node->setTag(GCommon::NO_COLL);
    }
    
    gameLayer = Layer::create();
    gameLayer->setName("gameLayer");
    gameLayer->setContentSize(Size(GCache::getInstance()->getRoomWidth(),
                                   GCache::getInstance()->getRoomHeight()));
    gameLayer->setPosition(GCache::getInstance()->getMapPosX(),
                           GCache::getInstance()->getMapPosY());
    tmxMap->addChild(gameLayer,2);
    
    
//    cloundLayer = Layer::create();
//    gameLayer->addChild(cloundLayer,0);
    
    
    hideLayer = Layer::create();
    hideLayer->setTag(GCommon::NO_COLL);
    hideLayer->setName("hideLayer");
    gameLayer->addChild(hideLayer);
    
    blockLayer = Layer::create();
    blockLayer->setTag(GCommon::NO_COLL);
    gameLayer->addChild(blockLayer);
    
    bulletLayer = Layer::create();
    bulletLayer->setName("bulletLayer");
    bulletLayer->setTag(GCommon::NO_COLL);
    gameLayer->addChild(bulletLayer);
    
    bubbleLayer = Layer::create();
    bubbleLayer->setName("bubbleLayer");
//    bubbleLayer->setTag(GCommon::NO_COLL);
    gameLayer->addChild(bubbleLayer);
    
    
    initUI();
    resetInit();
    
    GTools::playMusic(MUSIC_FIGHT);
    return true;
}

void GGameScene::initUI()
{
    Size s = Director::getInstance()->getWinSize();
    
    GRockView* view = GRockView::create("ui-yaogan1.png",
                                        "ui-yaogan2.png",
                                        "ui-yaogan2.png",50,
                                        GROCK_CALLBACK(GGameScene::rockHandler, this));
    uiLayer->addChild(view);
    
    auto timeBg = Sprite::create("ui-shijian1.png");
    timeBg->setAnchorPoint(Vec2(0.5,1));
    timeBg->setPosition(s.width/2,s.height-10);
    uiLayer->addChild(timeBg);
    
    _countDown = GNumber::create(0);
    _countDown->setAnchorPoint(Vec2(0,0.5));
    _countDown->setScale(0.7f);
    _countDown->useTimeStyle();
    _countDown->setPosition(154,timeBg->getContentSize().height/2-2);
    timeBg->addChild(_countDown);
    
    Button* btn = Button::create("ui-anniu1-1.png","ui-anniu1-1.png");
    btn->setName("speed");
    btn->setAnchorPoint(Vec2(0.5,0));
    btn->setPosition(Vec2(s.width - btn->getContentSize().width/2 - 20,20));
    btn->addTouchEventListener(CC_CALLBACK_2(GGameScene::touchEvent, this));
    uiLayer->addChild(btn);
    auto speed = Sprite::create("ui-anniu-jiasu1.png");
    speed->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    btn->addChild(speed);
    
    speedCd = ProgressTimer::create(Sprite::create("ui-anniu1-2.png"));
    speedCd->setType( ProgressTimer::Type::RADIAL );
    speedCd->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    speedCd->setPercentage(100);
    btn->addChild(speedCd);
    
//    btn = Button::create("ui-anniu1-1.png","ui-anniu1-1.png");
//    btn->setName("attack");
//    btn->setAnchorPoint(Vec2(0.5,0));
//    btn->setPosition(Vec2(s.width - btn->getContentSize().width/2 - 20,20));
//    btn->addTouchEventListener(CC_CALLBACK_2(GGameScene::touchEvent, this));
//    uiLayer->addChild(btn);
//    
//    bulletSkill = Sprite::create("ui-anniu-zidan1.png");
//    bulletSkill->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
//    btn->addChild(bulletSkill);
//    
//    attackCd = ProgressTimer::create(Sprite::create("ui-anniu1-2.png"));
//    attackCd->setType( ProgressTimer::Type::RADIAL );
//    attackCd->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
//    attackCd->setPercentage(100);
//    btn->addChild(attackCd);
    
//    attackCd->runAction(ProgressTo::create(5, 100));
    
    rankView = GRankView::create();
    uiLayer->addChild(rankView);
    
//    miniMap = GMiniMap::create();
//    uiLayer->addChild(miniMap);
}

void GGameScene::updateBulletSkill(int bulletId)
{
    char c[7];
    sprintf(c, "%d",bulletId);
    std::string path = "ui-anniu-zidan";
    path = path + c + std::string(".png");
    bulletSkill->initWithFile(path);
    
    if(bulletId != 1)
    {
        GTools::playSound(SOUND_GETBULLETS);
    }
}

void GGameScene::updateSpeedCd(float time,float pro)
{
    speedCd->runAction(ProgressTo::create(time, pro));
}

void GGameScene::updateAttackCd(float time,float pro)
{
    attackCd->stopAllActions();
    if(time == 0)
        attackCd->setPercentage(pro);
    else
        attackCd->runAction(ProgressTo::create(time, pro));
}

void GGameScene::resetInit()
{
//    cloundLayer->removeAllChildren();
    hideLayer->removeAllChildren();
    blockLayer->removeAllChildren();
    bulletLayer->removeAllChildren();
    bubbleLayer->removeAllChildren();
    
    initSprite();
    
//    miniMap->initDta();
    
    GModeGame::startGame();
    startHeartBeat();
    _start = false;
    lastPos = Vec2(-1000,-1000);
    robotDt = 1000;
    robotNum = 0;
    robotCollDt = 0;
}

void GGameScene::initSprite()
{
    //云
//    for(int i=0;i<GGameController::getInstance()->clounds.size();i++)
//    {
//        GCloundSprite* clound = GGameController::getInstance()->clounds.at(i);
//        clound->release();
//        clound->setPosition(clound->clound->x,clound->clound->y);
//        cloundLayer->addChild(clound);
//    }
    //水滴
    for(int i=0;i<GGameController::getInstance()->blocks.size();i++)
    {
        GBlockSprite* block = GGameController::getInstance()->blocks.at(i);
        block->release();
        block->setPosition(block->block->x,block->block->y);
        blockLayer->addChild(block);
    }
    //机器人
    for(int i=0;i<GGameController::getInstance()->robots.size();i++)
    {
        GBubbleSprite* robot = GGameController::getInstance()->robots.at(i);
        robot->release();
        robot->setPosition(robot->bubble->x,robot->bubble->y);
        bubbleLayer->addChild(robot);
    }
    //泡泡
    for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
    {
        GBubbleSprite* bubble = GGameController::getInstance()->bubbles.at(i);
        bubble->release();
        bubble->setPosition(bubble->bubble->x,bubble->bubble->y);
        bubbleLayer->addChild(bubble);
    }
    //自己泡泡
    GGameController::getInstance()->bubble->release();
    bubble = GGameController::getInstance()->bubble;
    bubble->setPosition(bubble->bubble->x,bubble->bubble->y);
    bubbleLayer->addChild(bubble);
    
    rollBg();
}

void GGameScene::start()
{
    _start = true;
    this->schedule(SEL_SCHEDULE(&GGameScene::update));
    
    GTools::playSound(SOUND_START);
    
    Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
    bubble->changeState(GBubble::State::MOVE);
    GModeGame::move(dir,1,bubble->bubble);
}

void GGameScene::update(float dt)
{
    //滚动背景
    rollBg();
    
    updateCollBubbleAndBlock(dt);
    updateRobot(dt);
    updateAllPos(dt);
    
//    miniMap->update(dt);
    
    
}

void GGameScene::rollBg()
{
    Size s = Director::getInstance()->getWinSize();
    float x = bubble->getPositionX() + gameLayer->getPositionX();
    float y = bubble->getPositionY() + gameLayer->getPositionY();
    
//    tmxMap->setPosition(-x+s.width/2, -y+s.height/2);
    
    this->getDefaultCamera()->setPosition(x, y);
    uiLayer->setPosition(x-s.width/2, y-s.height/2);
}

void GGameScene::cameraReset()
{
    Size s = Director::getInstance()->getWinSize();
    float x = bubble->getPositionX() + gameLayer->getPositionX();
    float y = bubble->getPositionY() + gameLayer->getPositionY();
    
    tmxMap->setPosition(-x+s.width/2, -y+s.height/2);
    this->getDefaultCamera()->setPosition(s.width/2, s.height/2);
    uiLayer->setPosition(0, 0);
}

void GGameScene::cameraResetClose()
{
    Size s = Director::getInstance()->getWinSize();
    float x = bubble->getPositionX() + gameLayer->getPositionX();
    float y = bubble->getPositionY() + gameLayer->getPositionY();
    
    tmxMap->setPosition(0,0);
    this->getDefaultCamera()->setPosition(x, y);
    uiLayer->setPosition(x-s.width/2, y-s.height/2);
}

void GGameScene::updateRank(GJsonArray* arr)
{
    rankView->updateData(arr);
}

void GGameScene::end()
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

void GGameScene::recConn()
{
    end();
    GModeUser::recConn("game");
}

void GGameScene::joinRoom(GBubbleSprite* bubble)
{
    bubble->setPosition(bubble->bubble->x,bubble->bubble->y);
//    plane->updateRotation(plane->plane->rotate);
    bubbleLayer->addChild(bubble);
    
    if(bubble->bubble->state == GBubble::State::MOVE)
    {
        Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
        bubble->move(dir,1,false);
    }
    else if(bubble->bubble->state == GBubble::State::SPEEDUP)
    {
        Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
        bubble->move(dir,1,true);
    }
//    miniMap->add(bubble);
}

void GGameScene::leaveRoom()
{
    for (Sprite* sp : colls) {
        sp->release();
    }
    colls.clear();
    GGameController::getInstance()->init(nullptr);
    end();
    GUserController::getInstance()->enterHomeScene();
}

void GGameScene::addBlock(GBlockSprite* block,int x,int y)
{
    if(x != 0)
    {
        block->setPosition(x, y);
        blockLayer->addChild(block);
        
        block->runAction(JumpTo::create(1, Vec2(block->block->x,block->block->y),
                                        random(30, 100), random(1, 2)));
//        float disX = fabsf(x - block->block->x);
//        float disY = fabsf(y - block->block->y);
//        ccBezierConfig bezier;
//        bezier.controlPoint_1 = Vec2(x, y);
//        bezier.controlPoint_2 = Vec2(x+disX/2, y+disY);
//        bezier.endPosition = Vec2(block->block->x,block->block->y);
//        block->runAction(BezierTo::create(1, bezier));
    }
    else
    {
        block->setPosition(block->block->x, block->block->y);
        blockLayer->addChild(block);
        
//        miniMap->add(block);
    }
}

void GGameScene::move(bool isSelf, cocos2d::Vec2 &dir,float power,Vec2 &pos,float rotate, std::string &uid,bool robot,bool speedUp)
{
    if(robot)
    {
        GBubbleSprite* sp = GGameController::getInstance()->findRobotByUid(uid);
        if(sp)
        {
//            if(sp->bubble->resetPosDt > 10)
//            {
//                sp->bubble->resetPosDt = 0;
//                sp->setPosition(pos);
//                sp->updateRotation(rotate);
//            }
            sp->move(dir,power,speedUp);
        }
    }
    else
    {
        if(isSelf)
        {
            bubble->move(dir,power,speedUp);
        }
        else
        {
            GBubbleSprite* sp = GGameController::getInstance()->findByUid(uid);
            if(sp)
            {
//                if(sp->bubble->resetPosDt > 10)
//                {
//                    sp->bubble->resetPosDt = 0;
//                    sp->setPosition(pos);
//                    sp->updateRotation(rotate);
//                }
                sp->move(dir,power,speedUp);
            }
        }
    }
}

void GGameScene::stopMove(bool isSelf, cocos2d::Vec2 &pos,float rotate,std::string &uid,bool speedUp)
{
    if(isSelf)
    {
        bubble->stopMove(speedUp);
    }
    else
    {
        GBubbleSprite* sp = GGameController::getInstance()->findByUid(uid);
        if(sp)
        {
//            sp->setPosition(pos);
//            sp->updateRotation(rotate);
            updatePosDt = UPDATE_DT;
            sp->stopMove(speedUp);
        }
    }
}

void GGameScene::coll(bool isSelf, cocos2d::Vec2 &pos,Vec2 &collPos,float rotate,std::string &uid)
{
//    if(isSelf)
//    {
//        bubble->coll(collPos);
//    }
//    else
//    {
//        GBubbleSprite* sp = GGameController::getInstance()->findByUid(uid);
//        if(sp)
//        {
//            sp->setPosition(pos);
//            sp->updateRotation(rotate);
//            sp->coll(collPos);
//        }
//    }
}

void GGameScene::rockHandler(cocos2d::Vec2 &dir,float power,const char *data)
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
            GModeGame::move(dir,power,bubble->bubble);
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

void GGameScene::updateCountDown(int time)
{
    if(_start)
    {
        roomTime = time/1000;
        _countDown->setNum(roomTime);
    }
}

void GGameScene::openRelived(GJsonObject* obj)
{
    if(roomTime < 5)
        return;
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
                                relived->initUI(this->obj);
                                relived->setName("relived");
                                uiLayer->addChild(relived);
                                this->obj->release();

                            }  ), NULL));
        
    }
}

void GGameScene::reloved()
{
    bubble->removeFromParent();
    bubble = nullptr;
    
    GGameController::getInstance()->bubble->release();
    bubble = GGameController::getInstance()->bubble;
    bubble->setPosition(bubble->bubble->x,bubble->bubble->y);
    bubbleLayer->addChild(bubble);
    
    GTools::playSound(SOUND_START);
}

void GGameScene::openGameOver(GJsonObject* obj)
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

void GGameScene::updateCollBubbleAndBlock(float dt)
{
    //自己
    if(bubble->bubble->state == GBubble::State::MOVE || bubble->bubble->state == GBubble::State::SPEEDUP)
    {
        for(int j=0;j<GGameController::getInstance()->blocks.size();j++)
        {
            GBlockSprite* sp = GGameController::getInstance()->blocks.at(j);
            if(sp->block->state != GBlock::State::DIE)
            {
                float dis = bubble->getUpdatePosition().getDistance(sp->getPosition());
                if(dis < bubble->getCollAndBlock())
                {
                    sp->block->state = GBlock::State::DIE;
                    GModeGame::eatBlock(sp->block,bubble->bubble);
                    break;
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
            if(bubble && (bubble->bubble->state == GBubble::State::MOVE || bubble->bubble->state == GBubble::State::SPEEDUP))
            {
                for(int j=0;j<GGameController::getInstance()->blocks.size();j++)
                {
                    GBlockSprite* sp = GGameController::getInstance()->blocks.at(j);
                    if(sp->block->state != GBlock::State::DIE)
                    {
                        float dis = bubble->getUpdatePosition().getDistance(sp->getPosition());
                        if(dis < bubble->getCollAndBlock())
                        {
                            sp->block->state = GBlock::State::DIE;
                            GModeGame::eatBlock(sp->block,bubble->bubble);
                            break;
                        }
                    }
                }
            }

        }
    }
}

void GGameScene::updateRobot(float dt)
{
    int size = (int)bubble->bubble->robotUid.size();
    if(size > 0)
    {
        robotDt+=dt;
        robotCollDt+=dt;
        if(robotDt > 0.5f)
        {
            robotDt = 0;
            
            if(robotNum < size)
            {
                std::string uid = bubble->bubble->robotUid.at(robotNum);
                GBubbleSprite * bubble = GGameController::getInstance()->findRobotByUid(uid);
                if(bubble && bubble->bubble->state != GBubble::State::DIE)
                {
//                    bubble->stopAttack();
                    updateRobotState(bubble);
                }
            }
            else
            {
                robotNum = -1;
            }
            robotNum++;
        }
        
        if(robotCollDt > 0.2)
        {
            robotCollDt = 0;
            
            for(int i=0;i< bubble->bubble->robotUid.size();i++)
            {
                std::string uid = bubble->bubble->robotUid.at(i);
                GBubbleSprite * bubble = GGameController::getInstance()->findRobotByUid(uid);
                if(bubble && bubble->bubble->state != GBubble::State::DIE && bubble->isCollWall())
                {
//                    bubble->stopAttack();
                    updateRobotState(bubble);
                }
            }
        }
    }
}

void GGameScene::updateRobotState(GBubbleSprite * bubble)
{
    int state = random(0, 10);
    //吃水滴
    if(state < 8)
    {
        //根据水滴找到方向
        int size = (int)GGameController::getInstance()->blocks.size() - 1;
        if(size <= 0)
            return;
        int r = random(0, size);
        GBlockSprite* block = GGameController::getInstance()->blocks.at(r);
        Vec2 dir = (block->getPosition() - bubble->getPosition()).getNormalized();
        
        bubble->bubble->dirX = dir.x;
        bubble->bubble->dirY = dir.y;
        bubble->changeState(GBubble::State::MOVE);
        GModeGame::move(dir,1,bubble->bubble);
    }
    //攻击
    else
    {
        //找到最近的泡泡
        GBubbleSprite* target = nullptr;
        for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
        {
            GBubbleSprite* sp = GGameController::getInstance()->bubbles.at(i);
            if(sp->bubble->state != GBubble::State::DIE)
            {
                if(target)
                {
                    float dis = bubble->getUpdatePosition().getDistance(sp->getUpdatePosition());
                    float dis2 = bubble->getUpdatePosition().getDistance(target->getUpdatePosition());
                    if(dis < dis2)
                    {
                        target = sp;
                    }
                }
                else
                {
                    target = sp;
                }
            }
        }
        
        for(int i=0;i<GGameController::getInstance()->robots.size();i++)
        {
            GBubbleSprite* sp = GGameController::getInstance()->robots.at(i);
            if(bubble != sp && sp->bubble->state != GBubble::State::DIE)
            {
                if(target)
                {
                    float dis = bubble->getUpdatePosition().getDistance(sp->getUpdatePosition());
                    float dis2 = bubble->getUpdatePosition().getDistance(target->getUpdatePosition());
                    if(dis < dis2)
                    {
                        target = sp;
                    }
                }
                else
                {
                    target = sp;
                }
            }
        }
        
        if(target)
        {
            float dis = bubble->getUpdatePosition().getDistance(this->bubble->getUpdatePosition());
            float dis2 = bubble->getUpdatePosition().getDistance(target->getUpdatePosition());
            if(dis < dis2 && this->bubble->bubble->state != GBubble::State::DIE)
            {
                target = this->bubble;
            }
        }
        else
        {
            if(this->bubble->bubble->state != GBubble::State::DIE)
            target = this->bubble;
        }
        
        if(target)
        {
            Vec2 dir = (target->getPosition() - bubble->getPosition()).getNormalized();
            
            bubble->bubble->dirX = dir.x;
            bubble->bubble->dirY = dir.y;
            bubble->changeState(GBubble::State::MOVE);
            GModeGame::move(dir,1,bubble->bubble);

//            bubble->startAttack();
        }
    }
}

void GGameScene::updateAllPos(float dt)
{
    updatePosDt+=dt;
    
    for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
    {
        GBubbleSprite * bubble = GGameController::getInstance()->bubbles.at(i);
        if(bubble->bubble->state != GBubble::State::DIE)
        {
            bubble->updatePosAndRotate();
            bubble->bubble->resetPosDt += dt;
        }
    }
    for(int i=0;i<GGameController::getInstance()->robots.size();i++)
    {
        GBubbleSprite * bubble = GGameController::getInstance()->robots.at(i);
        if(bubble->bubble->state != GBubble::State::DIE)
        {
            bubble->updatePosAndRotate();
            bubble->bubble->resetPosDt += dt;
        }
    }
    bubble->updatePosAndRotate();
    bubble->bubble->resetPosDt += dt;
    
    if(updatePosDt > UPDATE_DT + 0.5f)
    {
        updatePosDt = 0;
        
        GModeGame::updatePos(bubble->bubble);
        
        for(int j=0;j<GGameController::getInstance()->robots.size();j++)
        {
            GBubbleSprite * bubble = GGameController::getInstance()->robots.at(j);
            if(GGameController::getInstance()->isSelfBubble(bubble->bubble))
            {
                GModeGame::updatePos(bubble->bubble);
            }
        }
    }
}


void GGameScene::touchEvent(Ref *pSender, Widget::TouchEventType type)
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
                    GModeGame::move(dir,1,bubble->bubble);
                    
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
                        GModeGame::move(dir,1,bubble->bubble);
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
                        GModeGame::move(dir,1,bubble->bubble);
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
void GGameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* unused_event)
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
            GModeGame::move(dir,1,bubble->bubble);
            
            GTools::playSound(SOUND_SPEED);
        }
    }

}
void GGameScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event)
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
                GModeGame::move(dir,1,bubble->bubble);
            }
            else
            {
//                GModeGame::stopMove(bubble->bubble);
            }

        }
    }
}
