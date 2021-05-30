#pragma once

#include <jni.h>

#include <string>
#include "../../Il2cpp/Il2cppApi/Field.hpp"

namespace JNIApi
{
    class Utils
    {
    private:
    public:
        static std::string jstring_ToCppString(JNIEnv *env, jstring &jstr)
        {
            const char *str = env->GetStringUTFChars(jstr, NULL);
            std::string cpp_str = str;
            env->ReleaseStringUTFChars(jstr, str);
            return cpp_str;
        }

        static jstring jstring_Create(JNIEnv *env, std::string str)
        {
            jstring jstr = env->NewStringUTF(str.c_str());
            return jstr;
        }
    };
}
