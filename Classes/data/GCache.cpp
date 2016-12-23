//
//  GUserCache.cpp
//  OnlineGame
//
//  Created by guang on 16/10/1.
//
//

#include "GCache.h"
#include "tools/GTools.h"
USING_NS_CC;

static GCache* _instance = nullptr;

GCache* GCache::getInstance()
{
    if(_instance == nullptr)
    {
        _instance = new GCache();
        _instance->setUser(nullptr);
        _instance->retain();
    }
    return _instance;
}

void GCache::init()
{
    lang = Application::getInstance()->getCurrentLanguageCode();
}

std::string GCache::getLang()
{
    return this->lang;
}

void GCache::initFont()
{
    std::string path = "font-";
    path = path + this->lang + std::string(".plist");
    if(this->lang == "zh" || !FileUtils::getInstance()->isFileExist(path))
    {
        path = "font.plist";
    }
    v_font =  FileUtils::getInstance()->getValueMapFromFile(path);
}

std::string GCache::getFont(const std::string &key)
{
    std::string str = v_font[key.c_str()].asString();
    return str;
}

void GCache::setUser(GUser *user)
{
    if(user)
    {
        user->retain();
        this->_user = user;
    }
}

GUser* GCache::getUser()
{
    return this->_user;
}

void GCache::setBullets(std::vector<GBullet *> bullets)
{
    this->bullets = bullets;
    for(GBullet* bullet : this->bullets)
    {
        bullet->retain();
    }
}

GBullet* GCache::getBullet(int _id)
{
    for(GBullet* bullet : this->bullets)
    {
        if(bullet->_id == _id)
        {
            return bullet;
        }
    }
    return nullptr;
}

void GCache::setBulletId(int bulletId)
{
    this->bulletId = bulletId;
}

int GCache::getBulletId()
{
    return this->bulletId;
}

void GCache::setRoomWidth(int roomWidth)
{
    this->roomWidth = roomWidth;
}

int GCache::getRoomWidth()
{
    return this->roomWidth;
}

void GCache::setRoomHeight(int roomHeight)
{
    this->roomHeight = roomHeight;
}

int GCache::getRoomHeight()
{
    return this->roomHeight;
}

void GCache::setMapId(int mapId)
{
    this->mapId = mapId;
}
int GCache::getMapId()
{
    return this->mapId;
}

void GCache::setMapPosX(float mapPosX)
{
    this->mapPosX = mapPosX;
}
float GCache::getMapPosX()
{
    return this->mapPosX;
}

void GCache::setMapPosY(float mapPosY)
{
    this->mapPosY = mapPosY;
}
float GCache::getMapPosY()
{
    return this->mapPosY;
}

void GCache::setExps(std::string exps)
{
    this->exps = GTools::subString(exps, "/");
}
int GCache::getExp(int level)
{
    if(level <= 0)
        return 0;
    if(level >= exps.size())
    {
        std::string exp = exps.at(exps.size()-1);
        return std::atoi(exp.c_str());
    }
    
    std::string exp = exps.at(level-1);
    return std::atoi(exp.c_str());
}

void GCache::setGrows(std::string grows)
{
    this->grows = GTools::subString(grows, "/");
}
float GCache::getGrow(int level)
{
    if(level <= 0)
        return 1;
    if(level >= grows.size())
    {
        std::string grow = grows.at(grows.size()-1);
        return std::atof(grow.c_str());
    }
    
    std::string grow = grows.at(level-1);
    return std::atof(grow.c_str());

}

int GCache::getLevel(int exp)
{
    if(exp <= 0)
        return 0;
    for(int i=1;i<exps.size();i++)
    {
        if(exp >= getExp(i) && exp < getExp(i+1))
            return i+1;
    }
    return (int)exps.size();
}

void GCache::setNames(std::string names)
{
    this->names = GTools::subString(names, ",");
}
std::string GCache::getName()
{
    std::string name = this->names.at(random(0, (int)this->names.size()-1));
    name = name.replace(0, 1, "");
    return name.replace(name.size()-1,1,"");
}

void GCache::setSkins(std::string skins)
{
    this->skins = skins;
}
std::string GCache::getSkins()
{
    return this->skins;
}
void GCache::setSkinNum(int skinNum)
{
    this->skinNum = skinNum;
}
int GCache::getSkinNum()
{
    return this->skinNum;
}

void GCache::setBubble(std::string bubble)
{
    this->bubble = bubble;
}
std::string GCache::getBubble()
{
    return this->bubble;
}

void GCache::setShareAward(int shareAward)
{
    this->shareAward = shareAward;
}
int GCache::getShareAward()
{
    return this->shareAward;
}

void GCache::setKillMeUid(std::string killMeUid)
{
    this->killMeUid = killMeUid;
}
std::string GCache::getKillMeUid()
{
    return this->killMeUid;
}

int GCache::getMapPlitNum()
{
    return 200;
}

void GCache::destory()
{
    _user->release();
    for(GBullet* bullet : this->bullets)
    {
        bullet->release();
    }
    this->bullets.clear();
    
}


void GCache::setUid(std::string uid)
{
    UserDefault::getInstance()->setStringForKey("uid", uid);
    UserDefault::getInstance()->flush();
}
std::string GCache::getUid()
{
    return UserDefault::getInstance()->getStringForKey("uid");
}

void GCache::setMusic(bool music)
{
    UserDefault::getInstance()->setBoolForKey("music", music);
    UserDefault::getInstance()->flush();
}
bool GCache::isMusic()
{
    return UserDefault::getInstance()->getBoolForKey("music");
}
void GCache::setSound(bool sound)
{
    UserDefault::getInstance()->setBoolForKey("sound", sound);
    UserDefault::getInstance()->flush();
}
bool GCache::isSound()
{
    return UserDefault::getInstance()->getBoolForKey("sound");
}

void GCache::setHandStyle(bool isRight)
{
    UserDefault::getInstance()->setBoolForKey("handStyle", isRight);
    UserDefault::getInstance()->flush();
}
bool GCache::isRightHand()
{
    return UserDefault::getInstance()->getBoolForKey("handStyle");
}

