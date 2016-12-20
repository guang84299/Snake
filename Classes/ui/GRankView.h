//
//  GRankView.h
//  OnlineGame
//
//  Created by guang on 2016/10/29.
//
//

#ifndef GRankView_h
#define GRankView_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "ui/UIScrollView.h"

#include "data/GJson.h"

using namespace cocos2d::ui;

class GRankView : public cocos2d::Layer
{
public:
    virtual bool init();
    
    void updateData(GJsonArray* arr);
    
    Layout* createHead();
    Layout* createItem();
    void updateItem(int index,GJsonObject* obj);
    
    CREATE_FUNC(GRankView);
    
private:
    ListView* _listView;
    bool conSelf;
    int myRank;
};

#endif /* GRankView_h */
