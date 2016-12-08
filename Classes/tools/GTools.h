//
//  GTools.h
//  OnlineGame
//
//  Created by guang on 16/10/10.
//
//

#ifndef GTools_h
#define GTools_h

#include "cocos2d.h"

class GTools
{
public:
    static GTools* getInstance();
    static std::vector<std::string> subString(std::string str,const char* reg);
    static std::string itos(int num);
    static cocos2d::Animate* createAnimate(const std::string &file,int num,float speed);
    static cocos2d::Animate* createAnimate(const std::string &file,int start,int end,float speed);
    
    //ui
    static void showTost(cocos2d::Node* node,const std::string text);
    static std::string toTMXMap(std::string tmx);
    
    //sound
    static void playMusic(const std::string &file);
    static void pauseMusic();
    static void stopMusic();
    static void resumeMusic(const std::string &file);
    static void playSound(const std::string &sound);
    
    static void preload();
    
    //截屏
    static void screenShot(cocos2d::Node* node,const std::function<void(const std::string&)>& callback);
    
    static std::vector<std::string> parseUTF8(const std::string &str);
    static int gbkStrLen(const std::string &str);
    static std::string gbkSubStr(const std::string &str,int from, int to);
};

#endif /* GTools_h */
