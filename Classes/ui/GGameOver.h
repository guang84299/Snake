//
//  GGameOver.h
//  PaoPao
//
//  Created by guang on 2016/11/11.
//
//

#ifndef GGameOver_h
#define GGameOver_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
#include "data/GJson.h"

using namespace cocos2d::ui;


class GGameOver : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GGameOver);
    
    void initUI();
    void updateData(GJsonObject* arr);
    void updateItem(int index,GJsonObject* obj);
    Layout* createHead();
    Layout* createItem();
    Layout* createEnd();
    void openShare();
    void removeShare();
    void screenShotCallback(const std::string& file);
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
private:
    cocos2d::Layer* bg;
    ListView* _listView;
    Layout* end;
    bool conSelf;
    int kill;
};


#endif /* GGameOver_h */
