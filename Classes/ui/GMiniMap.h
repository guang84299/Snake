//
//  GMiniMap.h
//  PaoPao
//
//  Created by guang on 2016/11/5.
//
//

#ifndef GMiniMap_h
#define GMiniMap_h

#include "cocos2d.h"

class GBubbleSprite;
class GBlockSprite;

class GMiniMapSprite : public cocos2d::Sprite
{
public:
    enum Type
    {
        SELF,//自己
        ENEMY,//敌人
        OBSTACLE,//障碍
        BULLET//特殊子弹
    };
    
    static GMiniMapSprite* create(Type type,std::string uid);
    static GMiniMapSprite* create(Type type,int _id);
    void init(Type type,std::string uid);
    void init(Type type,int _id);
    void initSp();
    void resetType(Type type);
    
public:
    Type type;
    std::string uid;
    int _id;
};


class GMiniMap : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GMiniMap);
    
    void initDta();
    
    void update(float dt);
    
    void updateTarget();
    
    void add(GBubbleSprite* bubble);
    void add(GBlockSprite* block);
    
    cocos2d::Vec2 toVec(cocos2d::Vec2 &v);
    GMiniMapSprite* find(std::string uid);
    GMiniMapSprite* find(int _id);
    
    void remove(std::string uid);
    void remove(int _id);
private:
    cocos2d::Sprite* bg;
    GMiniMapSprite* selfSp;
    std::vector<GMiniMapSprite*> enemySp;
    std::vector<GMiniMapSprite*> bulletSp;
    
    float scw;
    float sch;
};

#endif /* GMiniMap_h */
