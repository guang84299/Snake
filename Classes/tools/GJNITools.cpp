//
//  GJNITools.cpp
//  PaoPao
//
//  Created by guang on 2016/11/5.
//
//

#include "GJNITools.h"
#include "tools/GTools.h"
#include "data/GCache.h"
#include "protocol_model/GModeUser.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "jni/JniHelper.h"
#include <string>
#include <android/log.h>

#define  LOG_TAG    "JniHelper"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#else
#define  LOGD(...)
#define  LOGE(...)

#endif

using namespace std;

USING_NS_CC;


bool GJNITools::isSign()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JNIEnv* env = cocos2d::JniHelper::getEnv();
    if (!env) {
        LOGE("Failed to get JNIEnv");
        return false;
    }
    
    jclass JniToolsClass = env->FindClass("org/cocos2dx/cpp/JniTools");
    
    // 获得Activity
    jmethodID getMainActivityMID = env->GetStaticMethodID(JniToolsClass,
                                                          "getAppActivity", "()Lorg/cocos2dx/cpp/AppActivity;");
    jobject mainActivityObject = env->CallStaticObjectMethod(JniToolsClass, getMainActivityMID);
    jclass mainActivityClass = env->GetObjectClass(mainActivityObject);
    
    // 获得包名
    jmethodID getPackageNameMID = env->GetMethodID(mainActivityClass,
                                                   "getPackageName", "()Ljava/lang/String;");
    jstring packageName = (jstring)(env->CallObjectMethod(mainActivityObject, getPackageNameMID));
    
    // 获得packagemanager对象
    jmethodID getPackageManagerMID = env->GetMethodID(mainActivityClass,
                                                      "getPackageManager", "()Landroid/content/pm/PackageManager;");
    jobject packageManagerObject = env->CallObjectMethod(mainActivityObject, getPackageManagerMID);
    jclass packageManagerClass = env->GetObjectClass(packageManagerObject);
    
    // 获得packageInfo对象
    jmethodID getPackageInfoMID = env->GetMethodID(packageManagerClass,
                                                   "getPackageInfo", "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jobject packageInfoObject = env->CallObjectMethod(packageManagerObject, getPackageInfoMID, packageName, 64);
    jclass packageInfoClass = env->GetObjectClass(packageInfoObject);
    
    // 获得signature
    jfieldID fidSignature = env->GetFieldID(packageInfoClass, "signatures", "[Landroid/content/pm/Signature;");
    jobjectArray signatures = (jobjectArray)(env->GetObjectField(packageInfoObject, fidSignature));
    
    jobject signatureObject = env->GetObjectArrayElement(signatures, 0);
    jclass signatureClass = env->GetObjectClass(signatureObject);
    
    jmethodID signatureToArrayMID = env->GetMethodID(signatureClass,
                                                     "toByteArray", "()[B");
    jbyteArray signatureToArrayArray = (jbyteArray)(env->CallObjectMethod(signatureObject, signatureToArrayMID));
    
    
    // 获得MessageDigest类
    jclass messageDigestClass = env->FindClass("java/security/MessageDigest");
    jmethodID getMessageDigestInstanceMID = env->GetStaticMethodID(messageDigestClass,
                                                                   "getInstance", "(Ljava/lang/String;)Ljava/security/MessageDigest;");
    jobject messageDigestObject = env->CallStaticObjectMethod(messageDigestClass, getMessageDigestInstanceMID, env->NewStringUTF("MD5"));
    
    jmethodID messageDigestUpdateMID = env->GetMethodID(messageDigestClass, "update", "([B)V");
    
    env->CallVoidMethod(messageDigestObject, messageDigestUpdateMID, signatureToArrayArray);
    
    // 获得digest byte array
    jmethodID messageDigestDigestMID = env->GetMethodID(messageDigestClass, "digest", "()[B");
    jbyteArray digestArray = (jbyteArray)(env->CallObjectMethod(messageDigestObject, messageDigestDigestMID));
    
    // 获得Array类
    jclass arrayClass = env->FindClass("java/util/Arrays");
    jmethodID arrayToStringMID = env->GetStaticMethodID(arrayClass,
                                                        "toString", "([B)Ljava/lang/String;");
    jstring arrayToString = (jstring)(env->CallStaticObjectMethod(arrayClass, arrayToStringMID, digestArray));

    string local = "[122, -68, 39, 69, 110, -31, -124, -49, -118, -119, 88, -70, 4, -74, -45, 74]";
    string local2 = "[33, -55, 77, -28, 124, 112, -27, 91, -115, 60, -83, -101, 29, -16, -33, -19]";
    string sign = cocos2d::JniHelper::jstring2string(arrayToString);
    
    return (local == sign) || (local2 == sign);
#else
    return true;
#endif
}

std::string GJNITools::getDeviceId()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::string uuid = "";
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/JniTools", "getDeviceId","()Ljava/lang/String;"))
    {
        LOGE("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        jstring deviceId = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
        uuid = JniHelper::jstring2string(deviceId);
        LOGE("jni uid=%s",uuid.c_str());
    }
    return uuid;
#else
    int r = cocos2d::random(0, 99);
    char c[7];
    sprintf(c, "%d", r);
    std::string uid = "test_";
    uid = uid + c;

    return uid;
#endif
}

void GJNITools::shareUrl()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/JniTools", "shareUrl","()V"))
    {
        LOGE("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    }
#else
    std::string textString = "success";
    if(textString == "success")
    {
        GModeUser::share();
    }
    else if(textString == "cancel")
    {
        GTools::showTost(nullptr, _T("share cancel"));
    }
    else if(textString == "fail")
    {
        GTools::showTost(nullptr, _T("share fail"));
    }
#endif
}

void GJNITools::sharePic()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/cpp/JniTools", "sharePic","()V"))
    {
        LOGE("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    }
#else
    std::string textString = "success";
    if(textString == "success")
    {
        GTools::showTost(nullptr, _T("share success"));
    }
    else if(textString == "cancel")
    {
        GTools::showTost(nullptr, _T("share cancel"));
    }
    else if(textString == "fail")
    {
        GTools::showTost(nullptr, _T("share fail"));
    }
#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C"{
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JniTools_shareResult(JNIEnv *env, jclass, jstring res) {
        std::string textString = StringUtils::getStringUTFCharsJNI(env,res);
        
        if(textString == "success")
        {
            GModeUser::share();
        }
        else if(textString == "cancel")
        {
            GTools::showTost(nullptr, _T("share cancel"));
        }
        else if(textString == "fail")
        {
            GTools::showTost(nullptr, _T("share fail"));
        }
    }
    
    JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_JniTools_sharePicResult(JNIEnv *env, jclass, jstring res) {
        std::string textString = StringUtils::getStringUTFCharsJNI(env,res);
        
        if(textString == "success")
        {
            GTools::showTost(nullptr, _T("share success"));
        }
        else if(textString == "cancel")
        {
            GTools::showTost(nullptr, _T("share cancel"));
        }
        else if(textString == "fail")
        {
            GTools::showTost(nullptr, _T("share fail"));
        }
    }
}
#endif
