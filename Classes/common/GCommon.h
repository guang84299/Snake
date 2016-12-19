//
//  GCommon.h
//  OnlineGame
//
//  Created by guang on 16/10/13.
//
//

#ifndef GCommon_h
#define GCommon_h

#include "cocos2d.h"

#define MUSIC_MIAN "mainBGM.mp3"
#define MUSIC_FIGHT "fightBGM.mp3"
#define SOUND_BUTTON "button.mp3"
#define SOUND_SHOOT "shoot.mp3"
#define SOUND_DIE "dead.mp3"
#define SOUND_START "born.mp3"
#define SOUND_SCORE "kill.mp3"
#define SOUND_DANGERIOUS "dangerious.mp3"
#define SOUND_SPEED "speed.mp3"
#define SOUND_GETBULLETS "getBullets.mp3"
#define SOUND_GETHP "getHP.mp3"

class GCommon
{
public:
    //碰撞常量
    static const int NO_COLL = 100000;
    //飞机
    static const int PLANE_1 = 0x00000001;
    static const int PLANE_2 = 0x00001100;
    static const int PLANE_3 = 0x00000000;
    
    //飞机圈
    static const int PLANE_CIRLE_1 = 0x00000010;
    static const int PLANE_CIRLE_2 = 0x00000010;//0x00000000
    static const int PLANE_CIRLE_3 = 0x00010010;

    //水晶
    static const int CRYSTAL_1 = 0x00000100;
    static const int CRYSTAL_2 = 0x00000001;
    static const int CRYSTAL_3 = 0x00000000;
    
    //子弹
    static const int BULLET_1 = 0x00001000;
    static const int BULLET_2 = 0x00010001;
    static const int BULLET_3 = 0x00000000;
    
    //球
    static const int BALL_1 = 0x00010010;//0x00010000
    static const int BALL_2 = 0x00001010;//0x00001000
    static const int BALL_3 = 0x00000010;
    
};
#endif /* GCommon_h */
