//
//  GTools.cpp
//  OnlineGame
//
//  Created by guang on 16/10/10.
//
//

#include "GTools.h"
#include "data/GJson.h"
#include "data/GCache.h"
#include "SimpleAudioEngine.h"
#include "common/GCommon.h"

#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d::ui;

USING_NS_CC;
using namespace CocosDenshion;

static GTools* _instance = nullptr;

GTools* GTools::getInstance()
{
    if(_instance == nullptr)
        _instance = new GTools();
    return _instance;
}


std::vector<std::string> GTools::subString(std::string str,const char* reg)
{
    std::vector<std::string> vec;
    int pos = (int)str.find(reg);
    while(pos >= 0)
    {
        pos = (int)str.find(reg);
        std::string s = str.substr(0,pos);
        vec.push_back(s);
        str = str.substr(pos+1,str.length());
        pos = (int)str.find(reg);
        if(pos < 0)
            vec.push_back(str);
    }
    if(vec.size() == 0)
        vec.push_back(str);
    return vec;
}

std::string GTools::itos(int num)
{
    char c[7];
    sprintf(c, "%d",num);
    return c;
}

Animate* GTools::createAnimate(const std::string &file,int num,float speed)
{
    return GTools::createAnimate(file,1,num,speed);
}

Animate* GTools::createAnimate(const std::string &file,int start,int end,float speed)
{
    auto animation = Animation::create();
    
    for(int i=start;i<=end;i++)
    {
        char szName[100] = {0};
        sprintf(szName, "_%02d.png", i);
        std::string f  = file + szName;
        animation->addSpriteFrameWithFile(f);
    }
    
    // should last 2.8 seconds. And there are 14 frames.
    animation->setDelayPerUnit(speed);
    animation->setRestoreOriginalFrame(true);
    auto action = Animate::create(animation);
    return action;
}

void GTools::showTost(cocos2d::Node* node,const std::string text)
{
    if(!node)
    {
        node = Director::getInstance()->getRunningScene();
        node = node->getChildByName("uiLayer");
    }
    
    
    Size s = Director::getInstance()->getWinSize();
    
    Label* l_text = Label::createWithSystemFont(text, "", 20);
    
    Size l_s = l_text->getContentSize();
    
    auto bg = Scale9Sprite::create("jm-tishi1.png");
    bg->setContentSize(Size(MAX(l_s.width+20,200),MAX(l_s.height+20,60)));
    bg->setPosition(Vec2(s.width/2,s.height*0.43f));
    node->addChild(bg,1000000);
    
    l_text->setPosition(bg->getContentSize().width/2, bg->getContentSize().height/2);
    bg->addChild(l_text);
    
    bg->runAction(Sequence::create(FadeTo::create(0.1f,255),DelayTime::create(2.0f), FadeOut::create(0.2f),RemoveSelf::create(true), nullptr));
}

void GTools::showTost2(cocos2d::Node* node,const std::string text)
{
    if(!node)
    {
        node = Director::getInstance()->getRunningScene();
        node = node->getChildByName("uiLayer");
    }
    
    
    Size s = Director::getInstance()->getWinSize();
    
    Label* l_text = Label::createWithSystemFont(text, "", 20);
    
    Size l_s = l_text->getContentSize();
    
    auto bg = Scale9Sprite::create("jm-tishi1.png");
    bg->setContentSize(Size(MAX(l_s.width+20,200),MAX(l_s.height+20,60)));
    bg->setPosition(Vec2(s.width/2,s.height*0.76f));
    node->addChild(bg,1000000);
    
    l_text->setPosition(bg->getContentSize().width/2, bg->getContentSize().height/2);
    bg->addChild(l_text);
    
    bg->runAction(Sequence::create(FadeTo::create(0.1f,255),DelayTime::create(2.0f), FadeOut::create(0.2f),RemoveSelf::create(true), nullptr));
}

void GTools::showTost(cocos2d::Node* node,const std::string text,float h)
{
    if(!node)
    {
        node = Director::getInstance()->getRunningScene();
        node = node->getChildByName("uiLayer");
    }
    
    
    Size s = Director::getInstance()->getWinSize();
    
    Label* l_text = Label::createWithSystemFont(text, "", 20);
    
    Size l_s = l_text->getContentSize();
    
    auto bg = Scale9Sprite::create("jm-tishi1.png");
    bg->setContentSize(Size(MAX(l_s.width+20,200),MAX(l_s.height+20,60)));
    bg->setPosition(Vec2(s.width/2,s.height*h));
    node->addChild(bg,1000000);
    
    l_text->setPosition(bg->getContentSize().width/2, bg->getContentSize().height/2);
    bg->addChild(l_text);
    
    bg->runAction(Sequence::create(FadeTo::create(0.1f,255),DelayTime::create(2.0f), FadeOut::create(0.2f),RemoveSelf::create(true), nullptr));

}


std::string GTools::toTMXMap(std::string tmx)
{
    auto tmxMap = TMXTiledMap::create(tmx);
    auto lay = tmxMap->getLayer("zhangai");
    
    Size mapSize = tmxMap->getMapSize();
    Size tileSize = tmxMap->getTileSize();
    
    TMXObjectGroup * spriteGroup = tmxMap->getObjectGroup("sprite");
    auto posObj = spriteGroup->getObject("pos") ;
    float posX = posObj.at("x").asFloat();
    float posY = posObj.at("y").asFloat();
    float w = posObj.at("w").asInt()*tileSize.width;
    float h = posObj.at("h").asInt()*tileSize.height;
    
    GJsonObject* obj = GJsonObject::create();
    obj->putDouble("x", posX);
    obj->putDouble("y", posY);
    obj->putDouble("w", w);
    obj->putDouble("h", h);
    
    obj->putDouble("tileW", tileSize.width);
    obj->putDouble("tileH", tileSize.height);
    
    GJsonArray* arr = GJsonArray::create();
    std::string result;
    for(int i=0;i<mapSize.width;i++)
    {
        for(int j=0;j<mapSize.height;j++)
        {
            Sprite* sp = lay->getTileAt(Vec2(i,j));
            if(sp)
            {
                int x = sp->getPositionX() - posX + tileSize.width/2;
                int y = sp->getPositionY() - posY + tileSize.height/2;
                GJsonObject* obj = GJsonObject::create();
                obj->putInt("x", x);
                obj->putInt("y", y);
                arr->add(obj);
            }
        }
    }
    obj->putArray("points", arr);
    
//    log(obj->toString().c_str());
    return result;
}


void GTools::playMusic(const std::string &file)
{
    stopMusic();
    if(GCache::isMusic())
        SimpleAudioEngine::getInstance()->playBackgroundMusic(file.c_str(),true);
}
void GTools::pauseMusic()
{
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
void GTools::stopMusic()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
void GTools::resumeMusic(const std::string &file)
{
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    if(!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
        playMusic(file);
}
void GTools::playSound(const std::string &sound)
{
    if(GCache::isSound())
        SimpleAudioEngine::getInstance()->playEffect(sound.c_str());
}

void GTools::preload()
{
    SimpleAudioEngine::getInstance()->preloadEffect(SOUND_BUTTON);
    SimpleAudioEngine::getInstance()->preloadEffect(SOUND_SHOOT);
    SimpleAudioEngine::getInstance()->preloadEffect(SOUND_DIE);
    SimpleAudioEngine::getInstance()->preloadEffect(SOUND_START);
    SimpleAudioEngine::getInstance()->preloadEffect(SOUND_SCORE);
    SimpleAudioEngine::getInstance()->preloadEffect(SOUND_DANGERIOUS);
    SimpleAudioEngine::getInstance()->preloadEffect(SOUND_SPEED);
    SimpleAudioEngine::getInstance()->preloadEffect(SOUND_GETBULLETS);
    SimpleAudioEngine::getInstance()->preloadEffect(SOUND_GETHP);
    
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MUSIC_FIGHT);
}


void GTools::screenShot(Node* node,const std::function<void(const std::string&)>& callback)
{
    Size winSize = Director::getInstance()->getWinSize();
    std::string file = FileUtils::getInstance()->getWritablePath() + "ScreenShot.png";
    if(FileUtils::getInstance()->isFileExist(file))
    {
        FileUtils::getInstance()->removeFile(file);
    }

    Image* image = utils::captureNode(node, 1);
    
    //保存截图
    image->saveToFile(file, true);
    
    auto scheduleCallback = [&,file,callback](float dt){
        callback(file);
    };
    auto _schedule = Director::getInstance()->getRunningScene()->getScheduler();
    _schedule->schedule(scheduleCallback, node, 0.0f,0,0.0f, false, "screenshot");
}

std::vector<std::string> GTools::parseUTF8(const std::string &str) {
    int l = (int)str.length();
    std::vector<std::string> ret;
    ret.clear();
    for(int p = 0; p < l; ) {
        int size;
        unsigned char c = str[p];
        if(c < 0x80) {
            size = 1;
        } else if(c < 0xc2) {
        } else if(c < 0xe0) {
            size = 2;
        } else if(c < 0xf0) {
            size = 3;
        } else if(c < 0xf8) {
            size = 4;
        } else if (c < 0xfc) {
            size = 5;
        } else if (c < 0xfe) {
            size = 6;
        }
        std::string temp = "";
        temp = str.substr(p, size);
        ret.push_back(temp);
        p += size;
    }
    return ret;
}
int GTools::gbkStrLen(const std::string &str)
{
    return (int)parseUTF8(str).size();
}
std::string GTools::gbkSubStr(const std::string &str,int from, int to)
{
    if(from > to) return "";
    std::vector<std::string> test = parseUTF8(str);
    if (test.size() < to) return str;
    std::vector<std::string>::iterator iter = test.begin();
    std::string res;
    std::string result;
    for(iter=(test.begin() + from); iter != (test.begin() + to); iter++)
    {
        res += *iter;
        
    }
    return res;
}
