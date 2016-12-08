//
//  GShaderView.cpp
//  PaoPao
//
//  Created by guang on 2016/11/1.
//
//

#include "GShaderView.h"

USING_NS_CC;


GShaderView::GShaderView() : _glprogramstate(nullptr)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _backgroundListener = EventListenerCustom::create(EVENT_RENDERER_RECREATED,
                                                      [this](EventCustom*)
                                                      {
                                                          auto glProgram = _glprogramstate->getGLProgram();
                                                          glProgram->reset();
                                                          glProgram->initWithByteArrays(ccPositionTextureColor_noMVP_vert, _fragSource.c_str());
                                                          glProgram->link();
                                                          glProgram->updateUniforms();
                                                      }
                                                      );
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_backgroundListener, -1);
#endif
}
GShaderView::~GShaderView()
{
    CC_SAFE_RELEASE_NULL(_glprogramstate);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    Director::getInstance()->getEventDispatcher()->removeEventListener(_backgroundListener);
#endif
}

GShaderView* GShaderView::create(const std::string& filename)
{
    GShaderView *sprite = new (std::nothrow) GShaderView();
    if (sprite && sprite->initWithFile(filename) && sprite->init())
    {
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
}

bool GShaderView::init()
{
    if(!Sprite::init())
    {
        return false;
    }
    
    initGLProgramState("res/shaders/example_Outline.fsh");
    
    cocos2d::Vec3 color(1.0f, 0.2f, 0.3f);
    GLfloat radius = 0.01f;
    GLfloat threshold = 1.75;
    
    _glprogramstate->setUniformVec3("u_outlineColor", color);
    _glprogramstate->setUniformFloat("u_radius", radius);
    _glprogramstate->setUniformFloat("u_threshold", threshold);
    
    setGLProgramState(_glprogramstate);
    
    return true;
}

void GShaderView::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
#if CC_USE_CULLING
    // Don't do calculate the culling if the transform was not updated
    _insideBounds = (flags & FLAGS_TRANSFORM_DIRTY) ? renderer->checkVisibility(transform, _contentSize) : _insideBounds;
    
    if(_insideBounds)
#endif
    {
        // negative effects: order < 0
        
        _cusumCommand.init(_globalZOrder, _texture->getName(), _glprogramstate, _blendFunc, &_quad, 1, transform, flags);
        renderer->addCommand(&_cusumCommand);
        
        // normal effect: order == 0
        _trianglesCommand.init(_globalZOrder, _texture->getName(), getGLProgramState(), _blendFunc, _polyInfo.triangles, transform, flags);
        renderer->addCommand(&_trianglesCommand);
        
        // postive effects: oder >= 0
        _cusumCommand.init(_globalZOrder, _texture->getName(), _glprogramstate, _blendFunc, &_quad, 1, transform, flags);
        renderer->addCommand(&_cusumCommand);
    }
}

bool GShaderView::initGLProgramState(const std::string &fragmentFilename)
{
    auto fileUtiles = FileUtils::getInstance();
    auto fragmentFullPath = fileUtiles->fullPathForFilename(fragmentFilename);
    auto fragSource = fileUtiles->getStringFromFile(fragmentFullPath);
    auto glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource.c_str());
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    _fragSource = fragSource;
#endif
    
    _glprogramstate = (glprogram == nullptr ? nullptr : GLProgramState::getOrCreateWithGLProgram(glprogram));
    CC_SAFE_RETAIN(_glprogramstate);
    
    _cusumCommand = QuadCommand();
    
    return _glprogramstate != nullptr;
}

