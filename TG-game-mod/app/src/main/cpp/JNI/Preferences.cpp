#include <jni.h>

#include <string>
#include <cstdlib>
#include <fstream>

#include <Logger.h>
#include <BridgeFunc.h>

#include "Expand/Utils.hpp"
#include "Data/Path.h"
#include "../Const/Const_General.h"
#include "../Hook/Data/Config.h"
#include "../Hook/Data/AppInfo.h"

std::string getVersionName(JNIEnv *env, jobject context)
{
    LOGD("getVersionName");
    jclass contextClass = env->GetObjectClass(context);

    LOGD("%s %s", Const::General::getPackageName.c_str(),
         Const::General::getPackageName_Method.c_str());
    LOGD("%s %s", Const::General::getPackageManager.c_str(),
         Const::General::getPackageManager_Method.c_str());

    jmethodID getPackageNameMid = RET_FUNC3(env->GetMethodID,
                                            contextClass, Const::General::getPackageName,
                                            Const::General::getPackageName_Method);
    jmethodID getPackageManager = RET_FUNC3(env->GetMethodID,
                                            contextClass, Const::General::getPackageManager,
                                            Const::General::getPackageManager_Method);

    LOGD("%s", Const::General::PackageManager_Class.c_str());
    LOGD("%s %s", Const::General::getPackageInfo.c_str(),
         Const::General::getPackageInfo_Method.c_str());

    jclass packageManagerClass = RET_FUNC1(env->FindClass,
                                           Const::General::PackageManager_Class);
    jmethodID getPackageInfo = RET_FUNC3(env->GetMethodID,
                                         packageManagerClass, Const::General::getPackageInfo,
                                         Const::General::getPackageInfo_Method);

    LOGD("%s", Const::General::PackageInfo_Class.c_str());
    LOGD("%s %s", Const::General::versionName.c_str(), Const::General::String_Type.c_str());

    jclass packageInfoClass = RET_FUNC1(env->FindClass,
                                        Const::General::PackageInfo_Class);
    jfieldID versionCodeFid = RET_FUNC3(env->GetFieldID,
                                        packageInfoClass, Const::General::versionName,
                                        Const::General::String_Type);

    jstring packageName = (jstring)env->CallObjectMethod(context, getPackageNameMid);
    jobject packageManagerObj = env->CallObjectMethod(context, getPackageManager);
    jobject packageInfoObj = env->CallObjectMethod(packageManagerObj, getPackageInfo, packageName,
                                                   0x0);
    jstring versionName = (jstring)env->GetObjectField(packageInfoObj, versionCodeFid);
    return JNIApi::Utils::jstring_ToCppString(env, versionName);
}

void JNI_Changes(JNIEnv *env, jclass clazz, jobject obj, jint feature,
                 jint value, jboolean boolean, jstring str)
{

#ifndef NDEBUG
    std::string featureName = JNIApi::Utils::jstring_ToCppString(env, str);
    LOGD("Feature name: %d - %s | Value: = %d | Bool: = %d", feature, featureName.c_str(), value,
         boolean);
#endif

    if (!AppInfo::isAppLoaded)
    {
        AppInfo::version = getVersionName(env, obj);
        AppInfo::isAppLoaded = true;
    }

    // Changed to if-statement because modders can easly mess up with cases.
    if (feature == 0)
    {
        Config::isHack = boolean;
    }
    else if (feature == 1)
    {
        Config::isEnterWin = boolean;
    }
    else if (feature == 2)
    {
        Config::isSkipTalk = boolean;
    }
    else if (feature == 3)
    {
        Config::dropRate = value;
    }
    else if (feature == 4)
    {
        Config::isKillEnemy = boolean;
    }
    else if (feature == 5)
    {
        Config::isInvincible = boolean;
    }
    else if (feature == 6)
    {
        Config::abilityRate = value;
    }
    else if (feature == 7)
    {
        Config::speedRate = value;
    }
    else if (feature == 8)
    {
        Config::isGodMode = boolean;
    }
    else if (feature == 9)
    {
        Config::isInfiniteSp = boolean;
    }
    else if (feature == 10)
    {
        Config::isHideDamage = boolean;
    }
    else if (feature == 11)
    {
        Config::isSkipTutorial = boolean;
    }
    else if (feature == 20)
    {
        Config::Injection_Type = value;
    }
    else if (feature == 21)
    {
        Config::Injection_Id = value;
    }
    else if (feature == 22)
    {
        Config::Injection_Num = value;
    }
    else if (feature == 7879)
    {
        Config::isEnableAdv = boolean;
    }
    else if (feature == 7880)
    {
        Config::isCloseEnemyKill = boolean;
    }
    else if (feature == 7881)
    {
        Config::isCloseTreasureKill = boolean;
    }
    else if (feature == 7882)
    {
        Config::isCloseObjectKill = boolean;
    }
    else if (feature == 7883)
    {
        Config::advDropRate = value;
    }
    else if (feature == 7884)
    {
        Config::advAbilityRate = value;
    }
    else if (feature == 7885)
    {
        Config::isCloseHPRate = boolean;
    }
    else if (feature == 7886)
    {
        Config::isCloseSPRate = boolean;
    }
    else if (feature == 7887)
    {
        Config::isCloseATKRate = boolean;
    }
    else if (feature == 7888)
    {
        Config::isCloseDEFRate = boolean;
    }
    else if (feature == 7889)
    {
        Config::isCloseHITRate = boolean;
    }
}

jstring JNI_Export(JNIEnv *env, jclass clazz)
{
#ifdef TW
    std::string exportFile = Const::General::tw_exportFile;
#elif defined(JP)
    std::string exportFile = Const::General::jp_exportFile;
#endif

    // 寫出檔案
    std::string inPath = safe_prefPath + exportFile;
    std::string outPath = safe_extPath + exportFile;

    LOGD("%s", inPath.c_str());
    LOGD("%s", outPath.c_str());
    std::ifstream fin(inPath);
    if (fin && fin.good())
    {
        std::ofstream fout(outPath);
        fout << fin.rdbuf();
        fout.close();
        return JNIApi::Utils::jstring_Create(env, Const::General::export_success_msg + outPath);
    }
    else
    {
        return JNIApi::Utils::jstring_Create(env, Const::General::export_error_msg);
    }
}