//
//  GGShaderView.h
//  PaoPao
//
//  Created by guang on 2016/11/1.
//
//

#ifndef GShaderView_h
#define GShaderView_h

#include "cocos2d.h"
class GEffect;

class GShaderView : public cocos2d::Sprite
{
public:
    static GShaderView* create(const std::string& filename);
    virtual bool init() override;
    bool initGLProgramState(const std::string &fragmentFilename);
    cocos2d::GLProgramState* getGLProgramState() const { return _glprogramstate; }
    void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
protected:
    GShaderView();
    ~GShaderView();

    cocos2d::GLProgramState* _glprogramstate;
    cocos2d::QuadCommand _cusumCommand;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    std::string _fragSource;
    cocos2d::EventListenerCustom* _backgroundListener;
#endif
};

#endif /* GShaderView_h */
