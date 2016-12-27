//
//  GJNITools.h
//  PaoPao
//
//  Created by guang on 2016/11/5.
//
//

#ifndef GJNITools_h
#define GJNITools_h

#include "cocos2d.h"

class GJNITools
{
public:
    static bool isSign();
    static std::string getDeviceId();
    static void shareUrl();
    static void sharePic();
    static void showInterstitialAd();
    static void mode();
    static void mode2();
    static void ping(int ping);
};

#endif /* GJNITools_h */
