//
//  GRockView.cpp
//  OnlineGame
//
//  Created by guang on 16/10/2.
//
//

#include "GRockView.h"

USING_NS_CC;

GRockView* GRockView::create(const std::string &bg,const std::string &gan, const std::string &rock,float r,GRockCallback callback)
{
    GRockView* view = new GRockView();
    if(view && view->init(bg,gan,rock,r,callback))
    {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GRockView::init(const std::string &bg,const std::string &gan, const std::string &rock,float r,GRockCallback callback)
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GRockView::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GRockView::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GRockView::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    _bg = Sprite::create(bg);
    _rock = Sprite::create(rock);
//    _gan = Sprite::create(gan);
//    _gan->setAnchorPoint(Vec2(0.5,0));
    this->_r = r;
    this->_callback = callback;
    
	isMove = false;
    _bg->setPosition(200, 100);
    this->addChild(_bg,0);
    
//    _gan->setPosition(_bg->getContentSize().width/2,_bg->getContentSize().height/2);
//    _bg->addChild(_gan,0);
    
    _rock->setPosition(200, 100);
    this->addChild(_rock,1);

    _dir = (_rock->getPosition() - _bg->getPosition()).getNormalized();
    _lastDir = rdir = Vec2(-1,-1);
    _lastApply = 0;
//    float bg_w = _bg->getContentSize().width;
//    _bg->setScale(r / bg_w, r / bg_w);
    
    isUpdate = false;
    rdt = 0;
//    this->schedule(SEL_SCHEDULE(&GRockView::update),1.0f/30);
    hide();
    return true;
}

void GRockView::show()
{
    _bg->setVisible(true);
    _rock->setVisible(true);
}

void GRockView::hide()
{
    _bg->setVisible(false);
    _rock->setVisible(false);
}


bool GRockView::onTouchBegan(Touch* touch, Event  *event)
{
    if(isMove)
        return false;
    show();
    
    auto location = touch->getLocation();
    _bg->setPosition(location);
    _rock->setPosition(location);
    isUpdate = true;
    isMove = false;
    return true;
}

void GRockView::onTouchMoved(Touch* touch, Event  *event)
{
    isMove = true;
    
    auto p1 = touch->getLocation();
    Vec2 p2 = _bg->getPosition();
    
    if(p1.getDistance(p2) < _r)
    {
        _rock->setPosition(p1);
        
    }
    if(p1.getDistance(p2) > _r)
    {
        Vec2 p3 = (p1 - p2).getNormalized() * _r + p2;
        _rock->setPosition(p3);
    }
    if(p1.getDistance(p2) > _r/10)
    {
        _dir = (_rock->getPosition()-p2).getNormalized();
        float apply = _rock->getPosition().getDistance(p2) / _r;
        if(fabsf(_dir.getAngle(_lastDir)) >= 0.02f || fabsf(apply-_lastApply) > 0.1f)
        {
            _callback(_dir,1,"move");
            _lastDir = _dir;
            _lastApply = apply;
        }
        else
        {
//            _callback(_dir,apply,"rotate");

        }
//        float angle = -180 / M_PI * _dir.getAngle() + 90;
//        angle = angle < 0 ? (360 + angle) : angle;
//        angle = angle > 180 ? angle-360 : angle;
//        _gan->setRotation(angle);
    }
}

void GRockView::onTouchEnded(Touch* touch, Event  *event)
{
    _rock->setPosition(_bg->getPosition());
    _dir = (_rock->getPosition() - _bg->getPosition()).getNormalized();
    isUpdate = false;
    isMove = false;
    _callback(_dir,0,"end");
    _lastDir = -_lastDir;
    hide();
}

void GRockView::update(float dt)
{
//     _callback(_dir,1,"move");
//    rdt += dt;
//    if(rdt > 0.1f)
//    {
//        rdt = 0;
//        
//        if(fabsf(_dir.getAngle(_lastDir)) <= 0.05f)
//        {
//            
//        }
//    }
}
