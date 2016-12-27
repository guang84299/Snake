#include "AppDelegate.h"
#include "scene/GMainScene.h"
#include "data/GCache.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("PaoPao", cocos2d::Rect(0, 0, designResolutionSize.width*0.8f, designResolutionSize.height*0.8f));
#else
        glview = GLViewImpl::create("PaoPao");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
//    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_HEIGHT);
//    glview->setFrameZoomFactor(0.6);
//    auto frameSize = glview->getFrameSize();
//    // if the frame's height is larger than the height of medium size.
//    if (frameSize.height > mediumResolutionSize.height)
//    {        
//        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is larger than the height of small size.
//    else if (frameSize.height > smallResolutionSize.height)
//    {        
//        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
//    }
//    // if the frame's height is smaller than the height of medium size.
//    else
//    {        
//        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
//    }

    register_all_packages();
    
    GCache::getInstance()->init();
    
    FileUtils::getInstance()->addSearchPath("fonts/");
    FileUtils::getInstance()->addSearchPath("res/animate/");
    FileUtils::getInstance()->addSearchPath("res/game/");
    FileUtils::getInstance()->addSearchPath("res/game/bullet/");
    FileUtils::getInstance()->addSearchPath("res/game/map/");
    FileUtils::getInstance()->addSearchPath("res/game/roles/");
    FileUtils::getInstance()->addSearchPath("res/music/");
    FileUtils::getInstance()->addSearchPath("res/particle/");
    FileUtils::getInstance()->addSearchPath("res/ui/common/");
    FileUtils::getInstance()->addSearchPath("res/ui/game/");
    FileUtils::getInstance()->addSearchPath("res/ui/head/");
    FileUtils::getInstance()->addSearchPath("res/ui/main/");
    FileUtils::getInstance()->addSearchPath("res/ui/home/");
    FileUtils::getInstance()->addSearchPath("res/ui/home/info/");
    FileUtils::getInstance()->addSearchPath("res/ui/home/setting/");
    FileUtils::getInstance()->addSearchPath("res/ui/home/skin/");
    FileUtils::getInstance()->addSearchPath("res/ui/home/rule/");
    FileUtils::getInstance()->addSearchPath("res/ui/home/share/");
    
    std::string wz = "res/ui/common/wz-" + GCache::getInstance()->getLang();

    if(GCache::getInstance()->getLang() == "zh" || !FileUtils::getInstance()->isDirectoryExist(wz))
    {
        wz = "res/ui/common/wz/";
    }
    FileUtils::getInstance()->addSearchPath(wz);
    
    FileUtils::getInstance()->addSearchPath(FileUtils::getInstance()->getWritablePath());
    
    if(director->isDisplayStats())
    {
//        FileUtils::getInstance()->setDefaultResourceRootPath("/Users/Resources/");
//        std::vector<std::string> vec;
//        FileUtils::getInstance()->setSearchPaths(vec);
    }

    // create a scene. it's an autorelease object
    auto scene = GMainScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
