//
//  GSkin.h
//  PaoPao
//
//  Created by guang on 2016/11/21.
//
//

#ifndef GSkin_h
#define GSkin_h

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

#include "data/GJson.h"

using namespace cocos2d::ui;

class GNumber;

class GSkin : public cocos2d::Layer
{
public:
    virtual bool init();
    
    CREATE_FUNC(GSkin);
    
    void initUI();
    void updateUI();
    Layout* createItem(int index);
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
private:
    cocos2d::Layer* bg;
    ListView* _listView;
    GNumber* t_crystal;
    GNumber* t_skin;
    GJsonArray* _data;
};


#endif /* GSkin_h */
