//
//  GMiniMap.cpp
//  PaoPao
//
//  Created by guang on 2016/11/5.
//
//

#include "GMiniMap.h"
#include "controller/GGameController.h"

USING_NS_CC;

GMiniMapSprite* GMiniMapSprite::create(Type type,std::string uid)
{
    GMiniMapSprite *sprite = new (std::nothrow) GMiniMapSprite();
    if (sprite)
    {
        sprite->init(type, uid);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
    
}

GMiniMapSprite* GMiniMapSprite::create(Type type,int _id)
{
    GMiniMapSprite *sprite = new (std::nothrow) GMiniMapSprite();
    if (sprite)
    {
        sprite->init(type, _id);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
    
}

void GMiniMapSprite::init(Type type,std::string uid)
{
    this->type = type;
    this->uid = uid;
    initSp();
}
void GMiniMapSprite::init(Type type,int _id)
{
    this->type = type;
    this->_id = _id;
    initSp();
}

void GMiniMapSprite::initSp()
{
    if(type == Type::SELF)
    {
        this->initWithFile("ui-ditu3.png");
    }
    else if(type == Type::ENEMY)
    {
        this->initWithFile("ui-ditu6.png");
    }
    else if(type == Type::OBSTACLE)
    {
        this->initWithFile("ui-ditu4.png");
    }
    else if(type == Type::BULLET)
    {
        this->initWithFile("ui-ditu5.png");
    }

}



bool GMiniMap::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    bg = Sprite::create("ui-ditu1.png");
//    bg->setPosition(sp->getContentSize().width/2, sp->getContentSize().height/2);
    this->addChild(bg);
    this->setContentSize(bg->getContentSize());
    
    return true;
}

void GMiniMap::initDta()
{
//    Node* node = this->getParent()->getParent()->getChildByName("map")->getChildByName("gameLayer");
//    float sc = node->getContentSize().width / 250.f;
//    Size s = node->getContentSize()/sc;
//    
//    bg->setContentSize(s);
//    this->setContentSize(bg->getContentSize());
    Size s = Director::getInstance()->getWinSize();
    this->setPosition(Vec2(this->getContentSize().width/2,
                              s.height-this->getContentSize().height/2));
    
    enemySp.clear();
    bulletSp.clear();
    bg->removeAllChildren();
    
    GBubble* bubble = GGameController::getInstance()->bubble->bubble;
    std::string uid = bubble->uid;
    selfSp = GMiniMapSprite::create(GMiniMapSprite::Type::SELF,uid);
    Vec2 v(bubble->x,bubble->y);
    selfSp->setPosition(toVec(v));
    bg->addChild(selfSp);
    
    //敌人
    for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
    {
        GBubble* bubble = GGameController::getInstance()->bubbles.at(i)->bubble;
        std::string uid = bubble->uid;
        auto sp = GMiniMapSprite::create(GMiniMapSprite::Type::ENEMY, uid);
        Vec2 v(bubble->x,bubble->y);
        sp->setPosition(toVec(v));
        bg->addChild(sp);
        enemySp.push_back(sp);
    }
    for(int i=0;i<GGameController::getInstance()->robots.size();i++)
    {
        GBubble* bubble = GGameController::getInstance()->robots.at(i)->bubble;
        std::string uid = bubble->uid;
        auto sp = GMiniMapSprite::create(GMiniMapSprite::Type::ENEMY, uid);
        Vec2 v(bubble->x,bubble->y);
        sp->setPosition(toVec(v));
        bg->addChild(sp);
        enemySp.push_back(sp);
    }
    //障碍
//    for(int i=0;i<GGameController::getInstance()->clounds.size();i++)
//    {
//        GClound* clound = GGameController::getInstance()->clounds.at(i)->clound;
//        auto sp = GMiniMapSprite::create(GMiniMapSprite::Type::OBSTACLE, "clound");
//        Vec2 v(clound->x,clound->y);
//        sp->setPosition(toVec(v));
//        bg->addChild(sp);
//    }
    //特殊子弹
    for(int i=0;i<GGameController::getInstance()->blocks.size();i++)
    {
        GBlock* block = GGameController::getInstance()->blocks.at(i)->block;
        if(block->bulletType > 1)
        {
            int _id = block->_id;
            auto sp = GMiniMapSprite::create(GMiniMapSprite::Type::BULLET, _id);
            Vec2 v(block->x,block->y);
            sp->setPosition(toVec(v));
            bg->addChild(sp);
            bulletSp.push_back(sp);
        }
    }
    //障碍
    TMXTiledMap* tmxMap = (TMXTiledMap*)this->getParent()->getParent()->getChildByName("map");
    auto lay = tmxMap->getLayer("zhangai");
    Size mapSize = tmxMap->getMapSize();
    Size tileSize = tmxMap->getTileSize();
    
    TMXObjectGroup * spriteGroup = tmxMap->getObjectGroup("sprite");
    auto posObj = spriteGroup->getObject("pos") ;
    float posX = posObj.at("x").asFloat();
    float posY = posObj.at("y").asFloat();
    for(int i=0;i<mapSize.width;i++)
    {
        for(int j=0;j<mapSize.height;j++)
        {
            Sprite* sp = lay->getTileAt(Vec2(i,j));
            if(sp)
            {
                int x = sp->getPositionX() - posX + tileSize.width/2;
                int y = sp->getPositionY() - posY + tileSize.height/2;
                auto sp = GMiniMapSprite::create(GMiniMapSprite::Type::OBSTACLE, "clound");
                Vec2 v(x,y);
                sp->setPosition(toVec(v));
                bg->addChild(sp);
            }
        }
    }

}

void GMiniMap::add(GBubbleSprite* bubble)
{
    std::string uid = bubble->bubble->uid;
    auto sp = GMiniMapSprite::create(GMiniMapSprite::Type::ENEMY, uid);
    Vec2 v(bubble->bubble->x,bubble->bubble->y);
    sp->setPosition(toVec(v));
    bg->addChild(sp);
    enemySp.push_back(sp);
}
void GMiniMap::add(GBlockSprite* block)
{
    if(block->block->bulletType > 1 || block->block->type == 2)
    {
        int _id = block->block->_id;
        auto sp = GMiniMapSprite::create(GMiniMapSprite::Type::BULLET, _id);
        Vec2 v(block->block->x,block->block->y);
        sp->setPosition(toVec(v));
        bg->addChild(sp);
        bulletSp.push_back(sp);
    }
}

void GMiniMap::update(float dt)
{
    Vec2 v = GGameController::getInstance()->bubble->getPosition();
    selfSp->setPosition(toVec(v));
    
    //敌人
    for(int i=0;i<GGameController::getInstance()->bubbles.size();i++)
    {
        GBubbleSprite* bubble = GGameController::getInstance()->bubbles.at(i);
        std::string uid = bubble->bubble->uid;
        Vec2 v = bubble->getPosition();
        
        GMiniMapSprite* sp = find(uid);
        if(sp)
        {
            sp->setPosition(toVec(v));
        }
    }
    for(int i=0;i<GGameController::getInstance()->robots.size();i++)
    {
        GBubbleSprite* bubble = GGameController::getInstance()->robots.at(i);
        std::string uid = bubble->bubble->uid;
        Vec2 v = bubble->getPosition();
        
        GMiniMapSprite* sp = find(uid);
        if(sp)
        {
            sp->setPosition(toVec(v));
        }
    }
    
}

Vec2 GMiniMap::toVec(cocos2d::Vec2 &v)
{
    Node* node = this->getParent()->getParent()->getChildByName("map")->getChildByName("gameLayer");
    Size s = node->getContentSize();
    
    float scw = (this->getContentSize().width-20) / s.width;
    float sch = (this->getContentSize().height-20) / s.height;
    
    return Vec2(v.x*scw,v.y*sch);
}

GMiniMapSprite* GMiniMap::find(std::string uid)
{
    for(int i=0;i<enemySp.size();i++)
    {
        GMiniMapSprite* sp = enemySp.at(i);
        if(sp->uid == uid)
            return sp;
    }
    return nullptr;
}
GMiniMapSprite* GMiniMap::find(int _id)
{
    for(int i=0;i<bulletSp.size();i++)
    {
        GMiniMapSprite* sp = bulletSp.at(i);
        if(sp->_id == _id)
            return sp;
    }
    return nullptr;
}

void GMiniMap::remove(std::string uid)
{
    for(int i=0;i<enemySp.size();i++)
    {
        GMiniMapSprite* sp = enemySp.at(i);
        if(sp->uid == uid)
        {
            sp->removeFromParent();
            enemySp.erase(enemySp.begin()+i);
        }
    }

}
void GMiniMap::remove(int _id)
{
    for(int i=0;i<bulletSp.size();i++)
    {
        GMiniMapSprite* sp = bulletSp.at(i);
        if(sp->_id == _id)
        {
            sp->removeFromParent();
            bulletSp.erase(bulletSp.begin()+i);
        }
    }
}



