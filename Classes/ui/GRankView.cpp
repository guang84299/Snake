//
//  GRankView.cpp
//  OnlineGame
//
//  Created by guang on 2016/10/29.
//
//

#include "GRankView.h"
#include "data/GCache.h"
#include "tools/GTools.h"

USING_NS_CC;

bool GRankView::init()
{
    if(!Layer::init())
    {
        return false;
    
    }
    Size s = Director::getInstance()->getWinSize();
    
    auto bg = Sprite::create("ui-paihang1.png");
    bg->setAnchorPoint(Vec2(1,1));
    bg->setPosition(s.width, s.height);
    this->addChild(bg);
    
    _listView = ListView::create();
    _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _listView->setContentSize(Size(160, 290));
    _listView->setPosition(Vec2(10,8));
    bg->addChild(_listView);
    
//    _listView->pushBackCustomItem(createHead());
    for(int i=0;i<10;i++)
    {
        _listView->pushBackCustomItem(createItem());
    }
     _listView->jumpToTop();
    return true;
}

void GRankView::updateData(GJsonArray* arr)
{
    conSelf = false;
    for(int i=0;i<arr->size();i++)
    {
        if(i>=10)
            break;
        if(!conSelf && i==9)
        {
            while(!conSelf)
            {
                updateItem(9,arr->at(i));
                i++;
            }
            break;
        }
        else
        {
            updateItem(i,arr->at(i));
        }
    }
    Vector<Widget*> list = _listView->getItems();
    for(int i=arr->size();i<list.size();i++)
    {
        Widget* w = list.at(i);
        w->setVisible(false);
    }
}

void GRankView::updateItem(int index,GJsonObject* obj)
{
    Layout* item = (Layout*)_listView->getItem(index);
    item->setVisible(true);
    
//    int rank = obj->getInt("rank");
    std::string name = obj->getString("name");
    int exp = obj->getInt("exp");
    char c[7];
//    sprintf(c, "%d",rank);
    
    bool isSelf = false;
    if(name == GCache::getInstance()->getUser()->name)
    {
        conSelf = isSelf = true;
    }
    
//    Text* t_rank = (Text*)item->getChildByName("rank");
//    t_rank->setString(c);
    
    Text* t_name = (Text*)item->getChildByName("name");
    int len = GTools::gbkStrLen(name);
    if(len > 8)
    {
        int to = 4;
        if(name.length() >= 21)
            to = 2;
        std::string name_head = GTools::gbkSubStr(name, 0, to);
        std::string name_end = GTools::gbkSubStr(name, len-2, len);
        name = name_head + ".." + name_end;
    }
    t_name->setString(name);
    
    sprintf(c, "%d",exp);
    Text* t_exp = (Text*)item->getChildByName("exp");
    t_exp->setString(c);
    
    if(isSelf)
    {
//        t_rank->setColor(Color3B::RED);
        t_name->setColor(Color3B::RED);
        t_exp->setColor(Color3B::RED);
    }
    else
    {
//        t_rank->setColor(Color3B::WHITE);
        t_name->setColor(Color3B::WHITE);
        t_exp->setColor(Color3B::WHITE);
    }
}

Layout* GRankView::createHead()
{
    Layout* item = Layout::create();
//    item->setTouchEnabled(true);
    item->setContentSize(Size(_listView->getContentSize().width,30));
    item->setPosition(Vec2(0,0));
    
    Text* t_rank = Text::create(_T("ranking"), "", 20);
    t_rank->setAnchorPoint(Vec2(0.5,0.5));
    t_rank->setPosition(Vec2(_listView->getContentSize().width/6, 0));
    item->addChild(t_rank);
    
    Text* t_name = Text::create(_T("rank_name"), "", 20);
    t_name->setAnchorPoint(Vec2(0.5,0.5));
    t_name->setPosition(Vec2(_listView->getContentSize().width/6*3, 0));
    item->addChild(t_name);
    
    Text* t_exp = Text::create(_T("rank_kill"), "", 20);
    t_exp->setAnchorPoint(Vec2(0.5,0.5));
    t_exp->setPosition(Vec2(_listView->getContentSize().width/6*5, 0));
    item->addChild(t_exp);
    
    return item;
}

Layout* GRankView::createItem()
{
    Layout* item = Layout::create();
//    item->setTouchEnabled(true);
    item->setContentSize(Size(_listView->getContentSize().width,_listView->getContentSize().height/10));
    item->setPosition(Vec2(0,0));
    
//    Text* t_rank = Text::create("", "", 20);
//    t_rank->setName("rank");
//    t_rank->setAnchorPoint(Vec2(0.5,0.5));
//    t_rank->setPosition(Vec2(_listView->getContentSize().width/6, 0));
//    item->addChild(t_rank);
    
    Text* t_name = Text::create("", "", 18);
    t_name->setName("name");
    t_name->setAnchorPoint(Vec2(0,0));
//    t_name->setPosition(Vec2(_listView->getContentSize().width/6*3, 0));
    t_name->setPosition(Vec2(_listView->getContentSize().width/6+10, 0));
    item->addChild(t_name);
    
    Text* t_exp = Text::create("", "", 18);
    t_exp->setName("exp");
    t_exp->setAnchorPoint(Vec2(0.5,0));
//    t_exp->setPosition(Vec2(_listView->getContentSize().width/6*5, 0));
    t_exp->setPosition(Vec2(_listView->getContentSize().width/6*5, 0));
    item->addChild(t_exp);
    
    return item;
}

