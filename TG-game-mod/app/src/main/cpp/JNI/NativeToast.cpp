#include <jni.h>

#include <Utils.h>
#include <string>
#include <BridgeFunc.h>

#include "../Const/Const_General.h"

void JNI_Toast(JNIEnv *env, jclass obj, jobject context, jint numOfMessage)
{
    int32_t length = Toast::LENGTH_LONG;
    if (numOfMessage == 0)
    {
        VOID_FUNC4(MakeToast, env, context, Const::General::creator_info, length);
    }
    if (numOfMessage == 1)
    {
        VOID_FUNC4(MakeToast, env, context, Const::General::hidden_info, length);
    }
    if (numOfMessage == 2)
    {
        VOID_FUNC4(MakeToast, env, context, Const::General::killed_info, length);
    }
    /*
    if (numOfMessage == 3)
    {
        VOID_FUNC4(MakeToast, env, context, Const::General::injection_info, length);
    }*/
}