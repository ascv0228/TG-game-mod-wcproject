#include <jni.h>

#include <string>

#include <BridgeFunc.h>

#include "Expand/Utils.hpp"
#include "Expand/AccountHandler.h"
#include "../Hook/Data/AppInfo.h"
#include "../Const/Const_General.h"

AccountHandler accHandler;

jint JNI_saveCurrent(JNIEnv *env, jobject thiz, jstring tag) {

    if(!AppInfo::isApiInited || !AppInfo::isHookInited) {
        return 3;
    }

    std::string cpp_tag = JNIApi::Utils::jstring_ToCppString(env, tag);

    if(!accHandler.addAccount(cpp_tag)) {
        return 1;
    }

    if(!accHandler.saveAll()) {
        return 2;
    }

    return 0;
}

jboolean JNI_loadSelected(JNIEnv *env, jobject thiz, jstring tag) {

    if(!AppInfo::isApiInited || !AppInfo::isHookInited) {
        return JNI_FALSE;
    }

    std::string cpp_tag = JNIApi::Utils::jstring_ToCppString(env, tag);

    if(!accHandler.switchAccount(cpp_tag)) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

jobjectArray JNI_loadAccountData(JNIEnv *env, jobject thiz) {
    auto acc_datas = accHandler.loadAll();

    int32_t data_len = acc_datas.size();

    jclass String_class = RET_FUNC1(env->FindClass, Const::General::String_class);
    jobjectArray ret_datas = env->NewObjectArray(data_len, String_class, NULL);

    for (int32_t i = 0; i < data_len; ++i) {
        jstring str = JNIApi::Utils::jstring_Create(env, acc_datas[i]);
        env->SetObjectArrayElement(ret_datas, i, str);
        env->DeleteLocalRef(str);
    }

    return ret_datas;
}

jint JNI_deleteSelected(JNIEnv *env, jobject thiz, jobjectArray tags) {
    int32_t deleted = 0;

    int32_t tags_len = env->GetArrayLength(tags);
    for(int i = 0; i < tags_len; ++i) {
        jstring jtag = (jstring) env->GetObjectArrayElement(tags, i);
        auto tag = JNIApi::Utils::jstring_ToCppString(env, jtag);
        env->DeleteLocalRef(jtag);

        if(!accHandler.deleteAccount(tag)) break;

        ++deleted;
    }

    if(!accHandler.saveAll()) {
        return -1;
    }

    return deleted;
}

jboolean JNI_newAccount(JNIEnv *env, jobject thiz) {

    if(!AppInfo::isApiInited || !AppInfo::isHookInited) {
        return JNI_FALSE;
    }

    if(!accHandler.toAccountScene()) {
        return JNI_FALSE;
    }

    return JNI_TRUE;
}
