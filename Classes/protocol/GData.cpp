//
//  GData.cpp
//  OnlineGame
//
//  Created by guang on 16/9/28.
//
//

#include "GData.h"
#include "data/GJson.h"


GData* GData::create(std::string mode,GJsonObject *jsbody)
{
    GData* data = new GData();
    data->mode = mode;
    data->jsbody = jsbody;
    data->pack();
    data->autorelease();
    return data;
}

std::string GData::pack()
{
    jsbody->putString("mode", mode);
    this->str = jsbody->toString();
    this->mode = mode;
    this->body = jsbody->getObject("body")->toString();
    this->bodyLength = body.length();
    this->length = this->str.length();
    
    return this->str;
}

std::string GData::toString()
{
    return str + '\r';
}

