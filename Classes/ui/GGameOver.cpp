//
//  GGameOver.cpp
//  PaoPao
//
//  Created by guang on 2016/11/11.
//
//

#include "GGameOver.h"
#include "ui/GTouchLayer.h"
#include "ui/GNumber.h"
#include "data/GCache.h"
#include "protocol_model/GModeGame.h"
#include "protocol_model/GModeUser.h"
#include "scene/GScene.h"
#include "scene/GGameScene.h"
#include "tools/GJNITools.h"

USING_NS_CC;

bool GGameOver::init()
{
    if(!Layer::init())
    {
        return false;
    }
    kill = 0;
    bg = GTouchLayer::create();
    this->addChild(bg);
    initUI();
    return true;
}

void GGameOver::initUI()
{
    Size s = Director::getInstance()->getWinSize();
    
    auto gameover_bg = Sprite::create("jm-diban1.png");
    gameover_bg->setAnchorPoint(Vec2(0.5,1));
    gameover_bg->setPosition(s.width/2, s.height-30);
    bg->addChild(gameover_bg);
    
    auto gameover_title = Sprite::create("jm-biaoti1.png");
    gameover_title->setAnchorPoint(Vec2(0.5,1));
    gameover_title->setPosition(gameover_bg->getContentSize().width/2,gameover_bg->getContentSize().height+10);
    gameover_bg->addChild(gameover_title);
    
    auto t_title = Sprite::create("jm-biaotiWZ1.png");
    t_title->setAnchorPoint(Vec2(0.5,1));
    t_title->setPosition(Vec2(gameover_title->getContentSize().width/2,
                              gameover_title->getContentSize().height-10));
    gameover_title->addChild(t_title);
    
    Button* btn = Button::create("an-annniu2-1.png","an-annniu2-2.png");
    btn->setName("home");
    btn->setPosition(Vec2(s.width/2-230,s.height*0.12f));
    btn->addTouchEventListener(CC_CALLBACK_2(GGameOver::touchEvent, this));
    bg->addChild(btn);
    
    auto t = Sprite::create("an-WZ2.png");
    t->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    btn->addChild(t);
    
    btn = Button::create("an-annniu2-1.png","an-annniu2-2.png");
    btn->setName("again");
    btn->setPosition(Vec2(s.width/2,s.height*0.12f));
    btn->addTouchEventListener(CC_CALLBACK_2(GGameOver::touchEvent, this));
    bg->addChild(btn);
    
    t = Sprite::create("an-WZ3.png");
    t->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    btn->addChild(t);
    
    btn = Button::create("an-annniu3-1.png","an-annniu3-2.png");
    btn->setName("show");
    btn->setPosition(Vec2(s.width/2+230,s.height*0.12f));
    btn->addTouchEventListener(CC_CALLBACK_2(GGameOver::touchEvent, this));
    bg->addChild(btn);
    
    t = Sprite::create("an-WZ4.png");
    t->setPosition(btn->getContentSize().width/2, btn->getContentSize().height/2);
    btn->addChild(t);
    
    
    _listView = ListView::create();
    _listView->setDirection(ui::ScrollView::Direction::VERTICAL);
    _listView->setContentSize(Size(630, 330));
    _listView->setPosition(Vec2(30, 88));
    gameover_bg->addChild(_listView);
    
    Layout* head = createHead();
    head->setPosition(Vec2(_listView->getPositionX(),
                           _listView->getContentSize().height+_listView->getPositionY()+10));
    gameover_bg->addChild(head);
    
    end = createEnd();
    end->setPosition(Vec2(_listView->getPositionX(),
                           _listView->getPositionY()- end->getContentSize().height));
    gameover_bg->addChild(end);
    
    for(int i=0;i<10;i++)
    {
        _listView->pushBackCustomItem(createItem());
    }
    _listView->jumpToTop();

}

void GGameOver::updateData(GJsonObject* obj)
{
    GJsonArray* arr = obj->getArray("ranks");
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
    int size = (int)list.size();
    int index = arr->size();
    for(int i=index;i<size;i++)
    {
         _listView->removeItem(index);
//        Widget* w = list.at(i);
//        w->setVisible(false);
    }
}

void GGameOver::updateItem(int index,GJsonObject* obj)
{
    Layout* item = (Layout*)_listView->getItem(index);
    item->setVisible(true);
    
    Scale9Sprite* bg = (Scale9Sprite*)item->getChildByName("bg");
    
    int rank = obj->getInt("rank");
    std::string name = obj->getString("name");
    int kill = obj->getInt("exp");
    int die = obj->getInt("die");
    int rewardNum = obj->getInt("rewardNum");
    char c[7];
//    sprintf(c, "%d",rank);
    
    std::string showName = name;
    int len = GTools::gbkStrLen(showName);
    if(len > 8)
    {
        int to = 4;
        if(name.length() >= 21)
            to = 2;
        std::string name_head = GTools::gbkSubStr(name, 0, to);
        std::string name_end = GTools::gbkSubStr(name, len-2, len);
        showName = name_head + ".." + name_end;
    }
    
    bool isSelf = false;
    if(name == GCache::getInstance()->getUser()->name)
    {
        conSelf = isSelf = true;
        bg->initWithFile("jm-tiao3.png");
        bg->setContentSize(Size(item->getContentSize().width*0.93f,item->getContentSize().height));
        
        GNumber* t_rank = (GNumber*)end->getChildByName("rank");
        t_rank->setNum(rank);
        t_rank->useRankStyle();
        
        Text* t_name = (Text*)end->getChildByName("name");
        t_name->setString(showName);
        
        sprintf(c, "%d",kill);
        Text* t_kill = (Text*)end->getChildByName("kill");
        t_kill->setString(c);
        
        sprintf(c, "%d",die);
        Text* t_die = (Text*)end->getChildByName("die");
        t_die->setString(c);
        
        GNumber* t_reward = (GNumber*)end->getChildByName("reward");
        t_reward->setNum(rewardNum);
        
        this->kill = kill;
    }
    
    GNumber* t_rank = (GNumber*)item->getChildByName("rank");
    t_rank->setNum(rank);
    t_rank->useRankStyle();
    
    Text* t_name = (Text*)item->getChildByName("name");
    t_name->setString(showName);
    
    sprintf(c, "%d",kill);
    Text* t_kill = (Text*)item->getChildByName("kill");
    t_kill->setString(c);
    
    sprintf(c, "%d",die);
    Text* t_die = (Text*)item->getChildByName("die");
    t_die->setString(c);
    
    GNumber* t_reward = (GNumber*)item->getChildByName("reward");
    t_reward->setNum(rewardNum);
}

Layout* GGameOver::createHead()
{
    float w = _listView->getContentSize().width;
    float h = _listView->getContentSize().height/8;
    Layout* item = Layout::create();
    //    item->setTouchEnabled(true);
    item->setContentSize(Size(w,h));
    item->setPosition(Vec2(0,0));
    
    auto txt_bg = Scale9Sprite::create("jm-tiao1.png");
    txt_bg->setAnchorPoint(Vec2(0,0));
    txt_bg->setContentSize(item->getContentSize());
    txt_bg->setPosition(0, 0);
    item->addChild(txt_bg);
    
    Text* t_rank = Text::create(_T("ranking"), "", 20);
    t_rank->setColor(Color3B(164,89,6));
    t_rank->setAnchorPoint(Vec2(0.5,0.5));
    t_rank->setPosition(Vec2(w*0.1f, h/2));
    txt_bg->addChild(t_rank);
    
    Text* t_name = Text::create(_T("rank_name"), "", 20);
    t_name->setColor(Color3B(164,89,6));
    t_name->setAnchorPoint(Vec2(0.5,0.5));
    t_name->setPosition(Vec2(w*0.3f, h/2));
    txt_bg->addChild(t_name);
    
    Text* t_kill = Text::create(_T("rank_kill"), "", 20);
    t_kill->setColor(Color3B(164,89,6));
    t_kill->setAnchorPoint(Vec2(0.5,0.5));
    t_kill->setPosition(Vec2(w*0.6f, h/2));
    txt_bg->addChild(t_kill);
    
    Text* t_die = Text::create(_T("rank_die"), "", 20);
    t_die->setColor(Color3B(164,89,6));
    t_die->setAnchorPoint(Vec2(0.5,0.5));
    t_die->setPosition(Vec2(w*0.73f, h/2));
    txt_bg->addChild(t_die);
    t_die->setVisible(false);
    
    Text* t_reward = Text::create(_T("rank_reward"), "", 20);
    t_reward->setColor(Color3B(164,89,6));
    t_reward->setAnchorPoint(Vec2(0.5,0.5));
    t_reward->setPosition(Vec2(w*0.85f, h/2));
    txt_bg->addChild(t_reward);

    
    return item;
}

Layout* GGameOver::createItem()
{
    float w = _listView->getContentSize().width;
    float h = _listView->getContentSize().height/6;
    float sc = 0.93f;
    Layout* item = Layout::create();
    //    item->setTouchEnabled(true);
    item->setContentSize(Size(w,h));
    item->setPosition(Vec2(0,0));
    
    auto txt_bg = Scale9Sprite::create("jm-tiao2.png");
    txt_bg->setName("bg");
    txt_bg->setAnchorPoint(Vec2(0.5,0.5));
    txt_bg->setContentSize(Size(w*sc,h));
    txt_bg->setPosition(w/2, h/2);
    item->addChild(txt_bg);
    
    GNumber* t_rank = GNumber::create(1);
    t_rank->setName("rank");
    t_rank->setAnchorPoint(Vec2(0.5,0.5));
    t_rank->setPosition(Vec2(w*0.1f, h/2));
    item->addChild(t_rank);
    
    Text* t_name = Text::create("aaaa", "", 23);
    t_name->setColor(Color3B::WHITE);
    t_name->setName("name");
    t_name->setAnchorPoint(Vec2(0.5,0.5));
    t_name->setPosition(Vec2(w*0.3f, h/2));
    t_name->enableOutline(Color4B::BLACK,1);
    t_name->enableGlow(Color4B::BLACK);
    item->addChild(t_name);
    
    Text* t_kill = Text::create("23", "", 24);
    t_kill->setColor(Color3B::WHITE);
    t_kill->setName("kill");
    t_kill->setAnchorPoint(Vec2(0.5,0.5));
    t_kill->setPosition(Vec2(w*0.6f, h/2));
    t_kill->enableOutline(Color4B::BLACK,1);
    t_kill->enableGlow(Color4B::BLACK);
    item->addChild(t_kill);
    
    Text* t_die = Text::create("32", "", 24);
    t_die->setColor(Color3B(248,65,58));
    t_die->setName("die");
    t_die->setAnchorPoint(Vec2(0.5,0.5));
    t_die->setPosition(Vec2(w*0.73f, h/2));
    t_die->enableOutline(Color4B::BLACK,1);
    t_die->enableGlow(Color4B::BLACK);
    item->addChild(t_die);
    t_die->setVisible(false);

    auto sp = Sprite::create("jm-zuanshi1.png");
    sp->setAnchorPoint(Vec2(1,0.5));
    sp->setPosition(Vec2(w*0.85f, h/2));
    item->addChild(sp);
    
    auto t_reward = GNumber::create(0);
    t_reward->setName("reward");
    t_reward->setAnchorPoint(Vec2(0,0.5));
    t_reward->setPosition(Vec2(w*0.85f+3, h/2));
    item->addChild(t_reward);
    
    return item;
}

Layout* GGameOver::createEnd()
{
    float w = _listView->getContentSize().width;
    float h = _listView->getContentSize().height/6;
    Layout* item = Layout::create();
    //    item->setTouchEnabled(true);
    item->setContentSize(Size(w,h));
    item->setPosition(Vec2(0,0));
    
    auto txt_bg = Sprite::create("jm-tiao5.png");
    txt_bg->setName("bg");
    txt_bg->setAnchorPoint(Vec2(0.5,0.5));
    txt_bg->setPosition(w/2, h/2);
    item->addChild(txt_bg);
    
    GNumber* t_rank = GNumber::create(1);
    t_rank->setName("rank");
    t_rank->setAnchorPoint(Vec2(0.5,0.5));
    t_rank->setPosition(Vec2(w*0.1f, h/2));
    item->addChild(t_rank);
    
    Text* t_name = Text::create("aaaa", "", 23);
    t_name->setColor(Color3B::WHITE);
    t_name->setName("name");
    t_name->setAnchorPoint(Vec2(0.5,0.5));
    t_name->setPosition(Vec2(w*0.3f, h/2));
    t_name->enableOutline(Color4B::BLACK,1);
    t_name->enableGlow(Color4B::BLACK);
    item->addChild(t_name);
    
    Text* t_kill = Text::create("23", "", 24);
    t_kill->setColor(Color3B::WHITE);
    t_kill->setName("kill");
    t_kill->setAnchorPoint(Vec2(0.5,0.5));
    t_kill->setPosition(Vec2(w*0.6f, h/2));
    t_kill->enableOutline(Color4B::BLACK,1);
    t_kill->enableGlow(Color4B::BLACK);
    item->addChild(t_kill);
    
    Text* t_die = Text::create("32", "", 24);
    t_die->setColor(Color3B(248,65,58));
    t_die->setName("die");
    t_die->setAnchorPoint(Vec2(0.5,0.5));
    t_die->setPosition(Vec2(w*0.73f, h/2));
    t_die->enableOutline(Color4B::BLACK,1);
    t_die->enableGlow(Color4B::BLACK);
    item->addChild(t_die);
    t_die->setVisible(false);
    
    auto sp = Sprite::create("jm-zuanshi1.png");
    sp->setAnchorPoint(Vec2(1,0.5));
    sp->setPosition(Vec2(w*0.85f, h/2));
    item->addChild(sp);
    
    auto t_reward = GNumber::create(0);
    t_reward->setName("reward");
    t_reward->setAnchorPoint(Vec2(0,0.5));
    t_reward->setPosition(Vec2(w*0.85f+3, h/2));
    item->addChild(t_reward);
    
    return item;
}

void GGameOver::removeShare()
{
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    GGameScene* game = dynamic_cast<GGameScene*>(sc);
    if(game)
    {
        game->cameraResetClose();
    }
    bg->removeChildByName("openShare");
}

void GGameOver::openShare()
{
    GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
    GGameScene* game = dynamic_cast<GGameScene*>(sc);
    if(game)
    {
        game->cameraReset();
    }
    
    Size s = Director::getInstance()->getWinSize();
    
    auto lay = GTouchLayer::create();
//    lay->setColor(Color4B(0, 0, 0, 60));
    lay->setName("openShare");
    bg->addChild(lay);
    
    auto guang1 = Sprite::create("jm-fengxiang3.png");
    guang1->setScale(2);
    guang1->setPosition(s.width/2, s.height*0.55f);
    lay->addChild(guang1);
    
    auto gameover_bg = Scale9Sprite::create("jm-diban2.png");
    gameover_bg->setContentSize(Size(570,348));
    gameover_bg->setAnchorPoint(Vec2(0.5,0.5));
    gameover_bg->setPosition(s.width/2, s.height*0.58f);
    lay->addChild(gameover_bg);
    
    auto gameover_title = Sprite::create("logo.png");
    gameover_title->setAnchorPoint(Vec2(0.5,0.5));
    gameover_title->setPosition(gameover_bg->getContentSize().width/2,gameover_bg->getContentSize().height-20);
    gameover_bg->addChild(gameover_title,2);
    
    
    auto guang2 = Sprite::create("jm-fengxiang2.png");
    guang2->setPosition(gameover_title->getContentSize().width*0.6f, gameover_title->getContentSize().height-20);
    gameover_title->addChild(guang2);
    

    auto setBg = Scale9Sprite::create("jm-tiao1.png");
    setBg->setContentSize(Size(508,36));
    setBg->setPosition(Vec2(gameover_bg->getContentSize().width/2,gameover_bg->getContentSize().height*0.77f));
    gameover_bg->addChild(setBg);
    
    auto shareBg = Sprite::create("ui-wenzhi3.png");
    shareBg->setAnchorPoint(Vec2(1,0.5));
    shareBg->setPosition(Vec2(gameover_bg->getContentSize().width/2,gameover_bg->getContentSize().height*0.6f));
    gameover_bg->addChild(shareBg);
    
    GNumber* t_kill = GNumber::create(this->kill);
    t_kill->setColor(Color3B::GREEN);
    t_kill->setAnchorPoint(Vec2(0.5,0.5));
    t_kill->setScale(2);
    t_kill->setPosition(Vec2(gameover_bg->getContentSize().width/2,gameover_bg->getContentSize().height*0.35f));
    gameover_bg->addChild(t_kill);
}

void GGameOver::screenShotCallback(const std::string& file)
{
    if(FileUtils::getInstance()->isFileExist(file))
    {
//        Size s = Director::getInstance()->getWinSize();
//        auto sp = Sprite::create(file);
//        sp->setPosition(s.width/2, s.height/2);
//        
//        GScene* sc = (GScene*)Director::getInstance()->getRunningScene();
//        Node* node = sc->getChildByName("uiLayer");
//        node->addChild(sp,1000);
//        
//        sp->runAction(Sequence::create(ScaleTo::create(0.2f, 0.85f),DelayTime::create(2),
//                                       RemoveSelf::create(), NULL));
        
        GJNITools::sharePic();
    }
}

void GGameOver::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
    Button* btn = (Button*)pSender;
    std::string name = btn->getName();
    switch (type)
    {
        case Widget::TouchEventType::BEGAN:
            GTools::playSound(SOUND_BUTTON);
            break;
            
        case Widget::TouchEventType::MOVED:
            break;
            
        case Widget::TouchEventType::ENDED:
            
            if(name == "home")
            {
                GModeGame::questLeaveRoom();
            }
            else if(name == "again")
            {
                GModeUser::enterRoom();
                this->runAction(Sequence::create(DelayTime::create(0.2f),RemoveSelf::create(), NULL));
            }
            else if(name == "show")
            {
                openShare();
                GTools::screenShot(Director::getInstance()->getRunningScene(),CC_CALLBACK_1(GGameOver::screenShotCallback, this));
                removeShare();
            }
            break;
            
        case Widget::TouchEventType::CANCELED:
            break;
            
        default:
            break;
    }
    
}
