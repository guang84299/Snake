//
//  GJson.cpp
//  OnlineGame
//
//  Created by guang on 16/9/29.
//
//

#include "GJson.h"

GJsonObject* GJsonObject::create()
{
    GJsonObject* json = new GJsonObject();
    json->document.SetObject();
    json->autorelease();
    return json;
}

GJsonObject* GJsonObject::create(const char* jsonData)
{
    std::string json = jsonData;
    return GJsonObject::create(json);
}

GJsonObject* GJsonObject::create(std::string &jsonData)
{
    GJsonObject* json = new GJsonObject();
    json->document.Parse<0>(jsonData.c_str());
    if(!json->document.HasParseError())
    {
        json->autorelease();
        return json;
    }
    
    CC_SAFE_DELETE(json);
    return nullptr;
}

std::string GJsonObject::toString()
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}

void GJsonObject::putInt(const char* key, int val)
{
    rapidjson::Value gkey(rapidjson::kStringType);
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    gkey.SetString(key, allocator);
    document.AddMember(gkey, val, allocator);
}

void GJsonObject::putBool(const char* key, bool val)
{
    rapidjson::Value gkey(rapidjson::kStringType);
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    gkey.SetString(key, allocator);
    document.AddMember(gkey, val, allocator);
}

void GJsonObject::putDouble(const char* key, double val)
{
    rapidjson::Value gkey(rapidjson::kStringType);
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    gkey.SetString(key, allocator);
    document.AddMember(gkey, val, allocator);
}

void GJsonObject::putString(const char* key, const char* val)
{
    std::string s_val = val;
    putString(key,s_val);
}

void GJsonObject::putString(const char* key, std::string& val)
{
    rapidjson::Value gkey(rapidjson::kStringType);
    rapidjson::Value gval(rapidjson::kStringType);

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    gkey.SetString(key, allocator);
    gval.SetString(val.c_str(),allocator);
    document.AddMember(gkey, gval, allocator);
}

int GJsonObject::getInt(const char* key)
{
    if(document.HasMember(key))
        return document[key].GetInt();
    return 0;
}

bool GJsonObject::getBool(const char* key)
{
    if(document.HasMember(key))
        return document[key].GetBool();
    return false;
}

double GJsonObject::getDouble(const char* key)
{
    if(document.HasMember(key))
        return document[key].GetDouble();
    return 0;
}

std::string GJsonObject::getString(const char* key)
{
    if(document.HasMember(key))
        return document[key].GetString();
    return "";
}

void GJsonObject::putObject(const char* key, GJsonObject* val)
{
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value v(val->document,allocator);
    rapidjson::Value gkey(rapidjson::kStringType);
    gkey.SetString(key, allocator);
    document.AddMember(gkey, v, allocator);
}

void GJsonObject::putArray(const char* key, GJsonArray* array)
{
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    rapidjson::Value v(array->document,allocator);
    rapidjson::Value gkey(rapidjson::kStringType);
    gkey.SetString(key, allocator);
    document.AddMember(gkey, v, allocator);
}

GJsonObject* GJsonObject::getObject(const char* key)
{
    GJsonObject* val = GJsonObject::create();
    if(document.HasMember(key))
    {
        rapidjson::Value gkey(rapidjson::kStringType);
        gkey.SetString(key, document.GetAllocator());
        rapidjson::Value v(document[key],document.GetAllocator());
//        val->document.AddMember(gkey, v, val->document.GetAllocator());
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        v.Accept(writer);
        val->document.Parse(buffer.GetString());
    }
    return val;
}

GJsonArray* GJsonObject::getArray(const char* key)
{
    GJsonArray* val = GJsonArray::create();
    if(document.HasMember(key))
    {
        rapidjson::Value gkey(rapidjson::kStringType);
        gkey.SetString(key, document.GetAllocator());
        rapidjson::Value v(document[key],document.GetAllocator());
//        val->document.AddMember(gkey, v, val->document.GetAllocator());
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        v.Accept(writer);
        val->document.Parse(buffer.GetString());
    }
    return val;
}


void GJsonObject::remove(const char* key)
{
    if(document.HasMember(key))
    {
        document.RemoveMember(key);
    }
}


GJsonArray* GJsonArray::create()
{
    GJsonArray* value = new GJsonArray();
    value->document.SetArray();
    value->autorelease();
    return value;
}

GJsonArray* GJsonArray::create(const char* jsonArray)
{
    std::string json = jsonArray;
    return GJsonArray::create(json);
}

GJsonArray* GJsonArray::create(std::string &jsonArray)
{
    GJsonArray* json = new GJsonArray();
    json->document.Parse<0>(jsonArray.c_str());
    if(!json->document.HasParseError())
    {
        json->autorelease();
        return json;
    }
    
    CC_SAFE_DELETE(json);
    return nullptr;
}

std::string GJsonArray::toString()
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    return buffer.GetString();
}

void GJsonArray::add(GJsonObject *val)
{
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    document.PushBack(rapidjson::Value(val->document,allocator), allocator);
}

void GJsonArray::remove(GJsonObject *val)
{
    rapidjson::Value::ConstValueIterator it = document.Begin();
    for(int i=0;i<document.Size();i++)
    {
       
        if(document[i] == val->document)
        {
            document.Erase(it);
            break;
        }
        it++;
    }
   
}

int GJsonArray::size()
{
    return document.Size();
}

GJsonObject* GJsonArray::at(int i)
{
    GJsonObject* val = GJsonObject::create();
    rapidjson::Value v(document[i],document.GetAllocator());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    v.Accept(writer);
    val->document.Parse(buffer.GetString());
    return val;
}
