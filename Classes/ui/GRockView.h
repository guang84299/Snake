//
//  GRockView.h
//  OnlineGame
//
//  Created by guang on 16/10/2.
//
//

#ifndef GRockView_h
#define GRockView_h

#include "cocos2d.h"

typedef std::function<void(cocos2d::Vec2 &dir,float power,const char *data)> GRockCallback;
#define GROCK_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3, ##__VA_ARGS__)

class GRockView : public cocos2d::Layer
{
public:
    static GRockView* create(const std::string &bg,const std::string &gan,const std::string &rock,float r,GRockCallback callback);
    
    bool init(const std::string &bg,const std::string &gan,const std::string &rock,float r,GRockCallback callback);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void update(float dt);
    
    void show();
    void hide();
private:
    cocos2d::Sprite* _bg;
    cocos2d::Sprite* _gan;
    cocos2d::Sprite* _rock;
    float _r;
    cocos2d::Vec2 _dir;
    cocos2d::Vec2 _lastDir;
    cocos2d::Vec2 rdir;
    float _lastApply;
    GRockCallback _callback;
    bool isUpdate;
    bool isMove;
    float rdt;
};

#endif /* GRockView_h */
