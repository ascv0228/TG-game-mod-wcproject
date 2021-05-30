#include <jni.h>

#include <string>
#include <vector>

#include <Logger.h>
#include <BridgeFunc.h>

#include "MApplication.h"
#include "Menu.h"
#include "NativeToast.h"
#include "Preferences.h"
#include "AccountManager.h"
#include "../Const/Const_General.h"

JavaVM *jvm = NULL;

template <size_t size>
void RegisterNative(JNIEnv *env, jclass clz, Bridge::JNINativeMethodBridge (&bridge)[size])
{
    JNINativeMethod registerParams[size];
    for (int32_t i = 0; i < size; ++i)
    {
        registerParams[i].name = bridge[i].name.c_str();
        registerParams[i].signature = bridge[i].signature.c_str();
        registerParams[i].fnPtr = bridge[i].fnPtr;
    }
    env->RegisterNatives(clz, registerParams, size);
}

void loadJNI()
{
    JNIEnv *env = NULL;
    jvm->GetEnv((void **)&env, JNI_VERSION_1_4);

    Bridge::JNINativeMethodBridge mapp_bridge[] = {
        {Const::General::attachBaseContext, Const::General::JNI_attachBaseContext_params, (void *)JNI_attachBaseContext},
        {Const::General::invoke, Const::General::JNI_invoke_params, (void *)JNI_invoke}};
    jclass mapp_clazz = RET_FUNC1(env->FindClass, Const::General::MApplication_class);
    RegisterNative(env, mapp_clazz, mapp_bridge);

    Bridge::JNINativeMethodBridge menu_bridge[] = {
        {Const::General::LoadSounds, Const::General::JNI_LoadSounds_params, (void *)JNI_LoadSounds},
        {Const::General::Title, Const::General::JNI_Title_params, (void *)JNI_Title},
        {Const::General::Heading, Const::General::JNI_Heading_params, (void *)JNI_Heading},
        {Const::General::Icon, Const::General::JNI_Icon_params, (void *)JNI_Icon},
        {Const::General::IconWebViewData, Const::General::JNI_IconWebViewData_params, (void *)JNI_IconWebViewData},
        {Const::General::getFeatureList, Const::General::JNI_getFeatureList_params, (void *)JNI_getFeatureList}};
    jclass menu_clazz = RET_FUNC1(env->FindClass, Const::General::FloatingModMenuService_class);
    RegisterNative(env, menu_clazz, menu_bridge);

    Bridge::JNINativeMethodBridge pref_bridge[] = {
        {Const::General::Changes, Const::General::JNI_Changes_params, (void *)JNI_Changes},
        {Const::General::Export, Const::General::JNI_Export_params, (void *)JNI_Export}};
    jclass pref_clazz = RET_FUNC1(env->FindClass, Const::General::Preferences_class);
    RegisterNative(env, pref_clazz, pref_bridge);

    Bridge::JNINativeMethodBridge toast_bridge[] = {
        {Const::General::Toast, Const::General::JNI_Toast_params, (void *)JNI_Toast},
    };
    jclass toast_clazz = RET_FUNC1(env->FindClass, Const::General::NativeToast_class);
    RegisterNative(env, toast_clazz, toast_bridge);

    Bridge::JNINativeMethodBridge acc_bridge[] = {
        {Const::General::saveCurrent, Const::General::JNI_saveCurrent_params, (void *)JNI_saveCurrent},
        {Const::General::loadSelected, Const::General::JNI_loadSelected_params, (void *)JNI_loadSelected},
        {Const::General::loadAccountData, Const::General::JNI_loadAccountData_params, (void *)JNI_loadAccountData},
        {Const::General::deleteSelected, Const::General::JNI_deleteSelected_params, (void *)JNI_deleteSelected},
        {Const::General::newAccount, Const::General::JNI_newAccount_params, (void *)JNI_newAccount}};
    jclass acc_clazz = RET_FUNC1(env->FindClass, Const::General::AccountManager_class);
    RegisterNative(env, acc_clazz, acc_bridge);
}

JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *vm, void *reserved)
{

    LOGD("JNI_OnLoad");

    jvm = vm;

    JNIEnv *env = NULL;
    if (vm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK)
    {
        return JNI_ERR;
    }

    loadJNI();

    LOGD("JNI_OnLoad finish");

    return JNI_VERSION_1_4;
}