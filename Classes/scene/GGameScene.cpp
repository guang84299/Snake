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

#define UPDATE_DT 0.1

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
    tmxMap->addChild(gameLayer,8);
    
 
    
    blockLayer1 = SpriteBatchNode::create("wujian-A1.png");
    gameLayer->addChild(blockLayer1);
    
    blockLayer2 = SpriteBatchNode::create("wujian-A2.png");
    gameLayer->addChild(blockLayer2);
    
    blockLayer3 = SpriteBatchNode::create("wujian-A3.png");
    gameLayer->addChild(blockLayer3);
    
    blockLayer4 = SpriteBatchNode::create("wujian-A4.png");
    gameLayer->addChild(blockLayer4);
    
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
    
    myExp = RichText::create();
    myExp->setAnchorPoint(Vec2(0.5,1));
    myExp->setPosition(Vec2(s.width/2,s.height-70));
    uiLayer->addChild(myExp);
    myExp->pushBackElement(RichElementText::create(0,Color3B::WHITE,255,_T("len"),"",24));
    myExp->pushBackElement(RichElementText::create(1,Color3B(245,230,126),255,"0","",24));
    myExp->pushBackElement(RichElementText::create(2,Color3B::WHITE,255,_T("mi"),"",24));
    
    Button* btn = Button::create("ui-anniu1.png","ui-anniu2.png");
    btn->setName("speed");
    btn->setAnchorPoint(Vec2(0.5,0));
    btn->setPosition(Vec2(s.width - btn->getContentSize().width/2 - 20,20));
    btn->addTouchEventListener(CC_CALLBACK_2(GGameScene::touchEvent, this));
    uiLayer->addChild(btn);
//    auto speed = Sprite::create("ui-anniu-jiasu1.png");
//    speed->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
//    btn->addChild(speed);
//    
//    speedCd = ProgressTimer::create(Sprite::create("ui-anniu1-2.png"));
//    speedCd->setType( ProgressTimer::Type::RADIAL );
//    speedCd->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
//    speedCd->setPercentage(100);
//    btn->addChild(speedCd);
    
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
    
    miniMap = GMiniMap::create();
    uiLayer->addChild(miniMap);
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

void GGameScene::resetInit()
{
    blockLayer1->removeAllChildren();
    blockLayer2->removeAllChildren();
    blockLayer3->removeAllChildren();
    blockLayer4->removeAllChildren();
    bubbleLayer->removeAllChildren();
    
    initSprite();
    
    miniMap->initDta();
}

void GGameScene::resetInitBlock()
{
    blockLayer1->removeAllChildren();
    blockLayer2->removeAllChildren();
    blockLayer3->removeAllChildren();
    blockLayer4->removeAllChildren();
    
    //水滴
    for(int i=0;i<GGameController::getInstance()->blocks.size();i++)
    {
        for(int j=0;j<GGameController::getInstance()->blocks[i].size();j++)
        {
            GBlockSprite* block = GGameController::getInstance()->blocks[i].at(j);
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
            
            block->release();
        }
    }
    
    GModeGame::startGame();
    startHeartBeat();
    _start = false;
    lastPos = Vec2(-1000,-1000);
    robotDt = 1000;
    robotNum = 0;
    robotCollDt = 0;
    crystalsDt = 0;
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
    //机器人
    for(int i=0;i<GGameController::getInstance()->robots.size();i++)
    {
        GBubbleSprite* robot = GGameController::getInstance()->robots.at(i);
        robot->setPosition(robot->bubble->x,robot->bubble->y);
        bubbleLayer->addChild(robot,9999999);
        robot->initBody();
        robot->release();
    }
    //泡泡
    for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
    {
        GBubbleSprite* bubble = GGameController::getInstance()->bubbles.at(i);
        bubble->setPosition(bubble->bubble->x,bubble->bubble->y);
        bubbleLayer->addChild(bubble,9999999);
        bubble->initBody();
        bubble->release();
    }
    //自己泡泡
    bubble = GGameController::getInstance()->bubble;
    bubble->setPosition(bubble->bubble->x,bubble->bubble->y);
    bubbleLayer->addChild(bubble,9999999);
    bubble->initBody();
    GGameController::getInstance()->bubble->release();
    
    rollBg(0);
}

void GGameScene::start()
{
    _start = true;
    this->schedule(SEL_SCHEDULE(&GGameScene::update),1/60.f);
    
    GTools::playSound(SOUND_START);
    
    Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
    bubble->changeState(GBubble::State::MOVE);
    GModeGame::move(dir,1,bubble->bubble);
    bubble->playRelive();
}

void GGameScene::update(float dt)
{
    //滚动背景
//    mapRollDt += dt;
//    if(mapRollDt > 0.5f)
//    {
//        rollBg(mapRollDt);
//        mapRollDt = 0;
//    }
    
    updateCollBubble(dt);
    updateCollBubbleAndBlock(dt);
    updateRobot(dt);
    updateAllPos(dt);
    
    miniMap->update(dt);
    
    rollBg(dt);
}

void GGameScene::rollBg(float dt)
{
    Size s = Director::getInstance()->getWinSize();
    
    float x = bubble->getPositionX() + gameLayer->getPositionX();
    float y = bubble->getPositionY() + gameLayer->getPositionY();
//    tmxMap->runAction(MoveTo::create(dt, Vec2(-x+s.width/2, -y+s.height/2)));
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

bool GGameScene::isStart()
{
    return this->_start;
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
    bubbleLayer->addChild(bubble,9999999);
    bubble->initBody();
    bubble->playRelive();
    
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
    miniMap->add(bubble);
}

void GGameScene::leaveRoom()
{
    GGameController::getInstance()->init(nullptr);
    end();
    GUserController::getInstance()->enterHomeScene();
}

void GGameScene::addBlock(GBlockSprite* block,int x,int y)
{
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

    if(x != 0)
    {
        block->setPosition(x, y);
//        blockLayer->addChild(block);
        
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
//        blockLayer->addChild(block);
        
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
            bubble->move(dir, 1,speedUpBtn);
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
void GGameScene::updateExp()
{
    char c[7];
    sprintf(c, "%d",bubble->bubble->exp);
    myExp->removeElement(1);
    myExp->insertElement(RichElementText::create(1, Color3B(245,230,126), 255, c, "", 24),1);
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
    bubbleLayer->addChild(bubble,9999999);
    bubble->initBody();
    bubble->playRelive();
    Vec2 dir(bubble->bubble->dirX,bubble->bubble->dirY);
    bubble->changeState(GBubble::State::MOVE);
    GModeGame::move(dir,1,bubble->bubble);
    
    
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

void GGameScene::updateCollBubble(float dt)
{
    if(bubble->bubble->state != GBubble::State::DIE && !bubble->isColl)
    {
        //和其他人
        for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
        {
            GBubbleSprite* sp = GGameController::getInstance()->bubbles.at(i);
            if(sp->bubble->state != GBubble::State::DIE && !sp->isColl && GGameController::getInstance()->isContain(bubble, sp))
            {
                for(int j=0;j<sp->bodys.size();j++)
                {
                    GBodySprite* body = sp->bodys.at(j);
                    float dis = bubble->getUpdatePosition().getDistance(body->getPosition());
                    if(dis < bubble->getContentSize().width/2 + body->getContentSize().width/2*body->getScale())
                    {
//                        bubble->changeState(GBubble::State::DIE);
                        bubble->coll();
                        GModeGame::bulletCollision(sp->bubble, bubble->bubble);
                        break;
                    }
                }
            }
            if(bubble->bubble->state == GBubble::State::DIE || bubble->isColl)
                break;
        }
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
                    if(dis < bubble->getContentSize().width/2 + body->getContentSize().width/2*body->getScale())
                    {
//                        bubble->changeState(GBubble::State::DIE);
                        bubble->coll();
                        GModeGame::bulletCollision(sp->bubble,bubble->bubble);
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
            GModeGame::bulletCollision(nullptr, bubble->bubble);
        }
    }
    
    //自己的机器人
    if(bubble->bubble->robotUid.size() > 0)
    {
        for(int i=0;i<bubble->bubble->robotUid.size();i++)
        {
            std::string uid = bubble->bubble->robotUid.at(i);
            GBubbleSprite * sp = GGameController::getInstance()->findRobotByUid(uid);
            if(sp && sp->bubble->state != GBubble::State::DIE && !sp->isColl)
            {
                //和自己
                if(bubble->bubble->state != GBubble::State::DIE && !bubble->isColl && GGameController::getInstance()->isContain(sp, this->bubble))
                {
                    for(int j=0;j<bubble->bodys.size();j++)
                    {
                        GBodySprite* body = bubble->bodys.at(j);
                        float dis = sp->getUpdatePosition().getDistance(body->getPosition());
                        if(dis < sp->getContentSize().width/2 + body->getContentSize().width/2*body->getScale())
                        {
//                            sp->changeState(GBubble::State::DIE);
                            sp->coll();
                            GModeGame::bulletCollision(bubble->bubble,sp->bubble);
                            break;
                        }
                    }
                }
               
                //和其他人
                for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
                {
                    GBubbleSprite* sp2 = GGameController::getInstance()->bubbles.at(i);
                    if(sp2->bubble->state != GBubble::State::DIE && !sp2->isColl && GGameController::getInstance()->isContain(sp, sp2))
                    {
                        for(int j=0;j<sp2->bodys.size();j++)
                        {
                            GBodySprite* body = sp2->bodys.at(j);
                            float dis = sp->getUpdatePosition().getDistance(body->getPosition());
                            if(dis < sp->getContentSize().width/2 + body->getContentSize().width/2*body->getScale())
                            {
//                                sp->changeState(GBubble::State::DIE);
                                sp->coll();
                                GModeGame::bulletCollision(sp2->bubble, sp->bubble);
                                break;
                            }
                        }
                    }
                    if(sp->bubble->state == GBubble::State::DIE || sp->isColl)
                        break;
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
                            if(dis < sp->getContentSize().width/2 + body->getContentSize().width/2*body->getScale())
                            {
//                                sp->changeState(GBubble::State::DIE);
                                sp->coll();
                                GModeGame::bulletCollision(sp2->bubble, sp->bubble);
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

void GGameScene::updateCollBubbleAndBlock(float dt)
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
                                GModeGame::eatBlock(sp->block,bubble->bubble);
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
                                GModeGame::eatBlock(sp->block,bubble->bubble);
                                break;
                            }
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
                if(bubble && bubble->bubble->state != GBubble::State::DIE)
                {
                    if(bubble->isCollWall())
                    updateRobotState(bubble);
                }
            }
        }
        
        if(robotAvoidDt > 0.5)
        {
            robotAvoidDt = 0;
            
            updateRobotAvoid();
        }

    }
}

void GGameScene::updateRobotState(GBubbleSprite * bubble)
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
        GModeGame::move(dir,1,bubble->bubble);
    }
}

void GGameScene::updateRobotAvoid()
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
                //和自己
                for(int j=0;j<bubble->bodys.size();j++)
                {
                    GBodySprite* body = bubble->bodys.at(j);
                    float dis = sp->getUpdatePosition().getDistance(body->getPosition());
                    if(dis < sp->getContentSize().width/2 + body->getContentSize().width/2*body->getScale() * 5)
                    {
                        Vec2 dir = (sp->getPosition() - body->getPosition()).getNormalized();
                        
                        bubble->bubble->dirX = dir.x;
                        bubble->bubble->dirY = dir.y;
                        bubble->changeState(GBubble::State::MOVE);
                        GModeGame::move(dir,1,sp->bubble);
                        break;
                    }
                }
                //和其他人
                for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
                {
                    GBubbleSprite* sp2 = GGameController::getInstance()->bubbles.at(i);
                    if(sp2->bubble->state != GBubble::State::DIE)
                    {
                        for(int j=0;j<sp2->bodys.size();j++)
                        {
                            GBodySprite* body = sp2->bodys.at(j);
                            float dis = sp->getUpdatePosition().getDistance(body->getPosition());
                            if(dis < sp->getContentSize().width/2 + body->getContentSize().width/2*body->getScale() * 5)
                            {
                                Vec2 dir = (sp->getPosition() - body->getPosition()).getNormalized();
                                
                                bubble->bubble->dirX = dir.x;
                                bubble->bubble->dirY = dir.y;
                                bubble->changeState(GBubble::State::MOVE);
                                GModeGame::move(dir,1,sp->bubble);
                                break;
                            }
                        }
                    }
                    if(sp->bubble->state == GBubble::State::DIE)
                        break;
                }
                //和机器人
                for(int i=0;i<GGameController::getInstance()->robots.size();i++)
                {
                    GBubbleSprite* sp2 = GGameController::getInstance()->robots.at(i);
                    if(sp2->bubble->state != GBubble::State::DIE && sp != sp2)
                    {
                        for(int j=0;j<sp2->bodys.size();j++)
                        {
                            GBodySprite* body = sp2->bodys.at(j);
                            float dis = sp->getUpdatePosition().getDistance(body->getPosition());
                            if(dis < sp->getContentSize().width/2 + body->getContentSize().width/2*body->getScale() * 5)
                            {
                                Vec2 dir = (sp->getPosition() - body->getPosition()).getNormalized();
                                
                                bubble->bubble->dirX = dir.x;
                                bubble->bubble->dirY = dir.y;
                                bubble->changeState(GBubble::State::MOVE);
                                GModeGame::move(dir,1,sp->bubble);
                                break;
                            }
                        }
                    }
                    if(sp->bubble->state == GBubble::State::DIE)
                        break;
                }
                
            }
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
            bubble->update(dt);
            bubble->updatePosAndRotate();
            bubble->bubble->resetPosDt += dt;
        }
    }
    for(int i=0;i<GGameController::getInstance()->robots.size();i++)
    {
        GBubbleSprite * bubble = GGameController::getInstance()->robots.at(i);
        if(bubble->bubble->state != GBubble::State::DIE)
        {
            bubble->update(dt);
            bubble->updatePosAndRotate();
            bubble->bubble->resetPosDt += dt;
        }
    }
    bubble->update(dt);
    bubble->updatePosAndRotate();
    bubble->bubble->resetPosDt += dt;
    
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
