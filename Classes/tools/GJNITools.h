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
};

#endif /* GJNITools_h */
