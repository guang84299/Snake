LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos/audio/include)

LOCAL_MODULE := MyGame_shared

LOCAL_MODULE_FILENAME := libMyGame

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/common/GCommon.cpp \
                   ../../Classes/controller/GGameController.cpp \
                   ../../Classes/controller/GUserController.cpp \
                   ../../Classes/data/GCache.cpp \
                   ../../Classes/data/GJson.cpp \
                   ../../Classes/model/GBlock.cpp \
                   ../../Classes/model/GBubble.cpp \
                   ../../Classes/model/GBullet.cpp \
                   ../../Classes/model/GClound.cpp \
                   ../../Classes/model/GUser.cpp \
                   ../../Classes/network/GHttp.cpp \
                   ../../Classes/network/GServer.cpp \
                   ../../Classes/protocol/GData.cpp \
                   ../../Classes/protocol/GHandler.cpp \
                   ../../Classes/protocol/GProtocol.cpp \
                   ../../Classes/protocol/GReflect.cpp \
                   ../../Classes/protocol_model/GModeGame.cpp \
                   ../../Classes/protocol_model/GModeUser.cpp \
                   ../../Classes/scene/GGameScene.cpp \
                   ../../Classes/scene/GGameScene2.cpp \
                   ../../Classes/scene/GHomeScene.cpp \
                   ../../Classes/scene/GMainScene.cpp \
                   ../../Classes/scene/GScene.cpp \
                   ../../Classes/sprite/GBlockSprite.cpp \
                   ../../Classes/sprite/GBubbleSprite.cpp \
                   ../../Classes/sprite/GBulletSprite.cpp \
                   ../../Classes/sprite/GCloundSprite.cpp \
                   ../../Classes/sprite/GBodySprite.cpp \
                   ../../Classes/tools/GJNITools.cpp \
                   ../../Classes/tools/GTools.cpp \
                   ../../Classes/ui/GGameOver.cpp \
                   ../../Classes/ui/GMiniMap.cpp \
                   ../../Classes/ui/GNumber.cpp \
                   ../../Classes/ui/GRankView.cpp \
                   ../../Classes/ui/GRelived.cpp \
                   ../../Classes/ui/GRockView.cpp \
                   ../../Classes/ui/GRule.cpp \
                   ../../Classes/ui/GSetting.cpp \
                   ../../Classes/ui/GShare.cpp \
                   ../../Classes/ui/GSkin.cpp \
                   ../../Classes/ui/GTouchLayer.cpp \
                   ../../Classes/ui/GUserInfo.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END
