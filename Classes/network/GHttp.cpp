//
//  GHttp.cpp
//  OnlineGame
//
//  Created by guang on 16/9/30.
//
//

#include "GHttp.h"
USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocos2d::network;


GHttp* GHttp::create()
{
    GHttp* http = new GHttp();
    http->autorelease();
    return http;
}

void GHttp::request(const char*url, const char*data, GHttpCallback callback)
{
    this->callback = callback;
    HttpRequest* request = new (std::nothrow) HttpRequest();
    request->setUrl(url);
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(CC_CALLBACK_2(GHttp::onHttpRequestCompleted, this));
    
    // write the post data
    request->setRequestData(data, strlen(data));
    request->setTag("GHttp");
    HttpClient::getInstance()->sendImmediate(request);
    request->release();
    
    this->retain();
}

void GHttp::onHttpRequestCompleted(HttpClient *sender, HttpResponse *response)
{
    if (!response || !response->isSucceed())
    {
        callback(false,"");
        return;
    }
    
    // dump data
    std::vector<char> *buffer = response->getResponseData();
    std::string data = "";
    for (unsigned int i = 0; i < buffer->size(); i++)
    {
        data += (*buffer)[i];
    }
    callback(true,data.c_str());
    this->release();
}
