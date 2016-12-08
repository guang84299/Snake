//
//  GHttp.h
//  OnlineGame
//
//  Created by guang on 16/9/30.
//
//

#ifndef GHttp_h
#define GHttp_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"

typedef std::function<void(bool state,const char *data)> GHttpCallback;
#define GHTTP_CALLBACK(__selector__,__target__, ...) std::bind(&__selector__,__target__, std::placeholders::_1, std::placeholders::_2, ##__VA_ARGS__)

class GHttp : public cocos2d::Ref
{
public:
    static GHttp* create();
    
    void request(const char* url,const char* data,GHttpCallback callback);
    
    void onHttpRequestCompleted(cocos2d::network::HttpClient *sender, cocos2d::network::HttpResponse *response);
private:
    GHttpCallback callback;
};

#endif /* GHttp_h */
