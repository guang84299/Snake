//
//  GJson.h
//  OnlineGame
//
//  Created by guang on 16/9/29.
//
//

#ifndef GJson_h
#define GJson_h

#include "cocos2d.h"
#include "json/document.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

class GJsonArray;
class GJsonObject : public cocos2d::Ref
{
private:
    GJsonObject(){};
public:
    static GJsonObject* create();
    static GJsonObject* create(const char *json);
    static GJsonObject* create(std::string &json);
    std::string toString();
    
    void putInt(const char* key,int val);
    void putBool(const char* key,bool val);
    void putDouble(const char* key,double val);
    void putString(const char* key,const char* val);
    void putString(const char* key,std::string &val);
    void putObject(const char* key,GJsonObject *val);
    void putArray(const char* key,GJsonArray *array);
    
    int getInt(const char* key);
    bool getBool(const char* key);
    double getDouble(const char* key);
    std::string getString(const char* key);
    GJsonObject* getObject(const char* key);
    GJsonArray* getArray(const char* key);

    void remove(const char* key);
public:
    rapidjson::Document document;
};

class GJsonArray : public cocos2d::Ref
{
private:
    GJsonArray(){};
public:
    static GJsonArray* create();
    static GJsonArray* create(const char *jsonArray);
    static GJsonArray* create(std::string &jsonArray);
    std::string toString();
    void add(GJsonObject *val);
    void remove(GJsonObject *val);
    int size();
    GJsonObject* at(int i);
public:
    rapidjson::Document document;
};

#endif /* GJson_h */
