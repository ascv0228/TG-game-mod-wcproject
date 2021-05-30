#include <jni.h>

#include <string>
#include <vector>

#include <base64.hpp>
#include <Logger.h>
#include <BridgeFunc.h>

#include "Expand/Utils.hpp"
#include "Data/Path.h"
#include "../Const/Const_General.h"

int GET_SIGNATURES = 0x40;

std::string appPkgName = Const::General::empty;
jobject pkgManager; // sPackageManager
jobjectArray sign;  // sign (2d array)

void hookPMS(JNIEnv *env, jobject thiz, jobject context)
{

#ifdef TW
        std::string str = Const::General::tw_signature;
#elif defined(JP)
        std::string str = Const::General::jp_signature;
#endif

        auto dec = base64::decode(str);

        LOGD("native sign length: %d", dec[0]);

        jobjectArray msign = env->NewObjectArray(dec[0], RET_FUNC1(env->FindClass, Const::General::byte_array_class), NULL);

        // 計算簽名並紀錄起來
        int32_t pos = 1;
        for (int32_t i = 0; i < dec[0]; ++i)
        {
                int32_t len = dec[pos] << 24 | dec[pos + 1] << 16 | dec[pos + 2] << 8 | dec[pos + 3];
                pos += 4;

                jbyteArray bArray = env->NewByteArray(len);
                env->SetByteArrayRegion(bArray, 0, len, (jbyte *)(dec.data() + pos));
                env->SetObjectArrayElement(msign, i, bArray);
                env->DeleteLocalRef(bArray);
                pos += len;
        }
        sign = (jobjectArray)env->NewGlobalRef(msign);

        // 獲取 ActivityThread 中的 PackageManager 實例並記錄起來
        jclass ActivityThread_class = RET_FUNC1(env->FindClass, Const::General::ActivityThread_class);
        jfieldID sPackageManager_field = RET_FUNC3(env->GetStaticFieldID, ActivityThread_class,
                                                   Const::General::sPackageManager, Const::General::IPackageManager_type);
        jobject sPackageManager_object = env->GetStaticObjectField(ActivityThread_class,
                                                                   sPackageManager_field);
        pkgManager = env->NewGlobalRef(sPackageManager_object);

        // 取得 java 層包名
        jclass Context_class = env->GetObjectClass(context);
        jmethodID getPackageName = RET_FUNC3(env->GetMethodID, Context_class,
                                             Const::General::getPackageName, Const::General::getPackageName_params);
        jstring pkgName = (jstring)env->CallObjectMethod(context, getPackageName);
        appPkgName = JNIApi::Utils::jstring_ToCppString(env, pkgName);

        LOGD("appPkgName: %s", appPkgName.c_str());

        // 取得 IPackageManager 的 ClassLoader
        jclass Class_class = RET_FUNC1(env->FindClass, Const::General::Class_class);
        jmethodID forName = RET_FUNC3(env->GetStaticMethodID, Class_class,
                                      Const::General::forName, Const::General::forName_params);
        jstring IPackageManager_name = RET_FUNC1(env->NewStringUTF, Const::General::IPackageManager_class_name);
        jobject IPackageManager_reflect_class = env->CallStaticObjectMethod(Class_class, forName,
                                                                            IPackageManager_name);

        jmethodID getClassLoader = RET_FUNC3(env->GetMethodID, Class_class,
                                             Const::General::getClassLoader, Const::General::getClassLoader_params);
        jobject IPackageManager_classLoader = env->CallObjectMethod(IPackageManager_reflect_class, getClassLoader);

        // 設置 proxy 要實現的介面
        jobjectArray proxyInterfaces = env->NewObjectArray(1, Class_class, NULL);
        env->SetObjectArrayElement(proxyInterfaces, 0, IPackageManager_reflect_class);

        // 創建動態代理
        jclass Proxy_class = RET_FUNC1(env->FindClass, Const::General::Proxy_class);
        jmethodID newProxyInstance = RET_FUNC3(env->GetStaticMethodID, Proxy_class,
                                               Const::General::newProxyInstance, Const::General::newProxyInstance_params);
        jobject proxy = env->CallStaticObjectMethod(Proxy_class, newProxyInstance,
                                                    IPackageManager_classLoader, proxyInterfaces, thiz);

        // 代理 ActivityThread 中的 sPackageManager
        env->SetStaticObjectField(ActivityThread_class, sPackageManager_field, proxy);

        // 代理 ApplicationPackageManager 中的 mPM
        jmethodID getPackageManager = RET_FUNC3(env->GetMethodID, Context_class,
                                                Const::General::getPackageManager, Const::General::getPackageManager_params);
        jobject pm = env->CallObjectMethod(context, getPackageManager);
        jclass PackageManager_class = env->GetObjectClass(pm);
        jfieldID mPM_field = RET_FUNC3(env->GetFieldID, PackageManager_class,
                                       Const::General::mPM, Const::General::IPackageManager_type);
        env->SetObjectField(pm, mPM_field, proxy);

        LOGD("PMS hook success!");
}

std::string safe_prefPath = Const::General::empty;
std::string safe_extPath = Const::General::empty;
std::string safe_filePath = Const::General::empty;

void initPath(JNIEnv *env, jobject context)
{
        LOGD("initPath");

        jclass ContextImpl_class = RET_FUNC1(env->FindClass, Const::General::ContextImpl_class);
        jmethodID getImpl = RET_FUNC3(env->GetStaticMethodID, ContextImpl_class,
                                      Const::General::getImpl, Const::General::getImpl_params);
        jobject context_impl = env->CallStaticObjectMethod(ContextImpl_class, getImpl, context);

        // 取得 shared_prefs 儲存路徑
        jmethodID getPreferencesDir = RET_FUNC3(env->GetMethodID, ContextImpl_class,
                                                Const::General::getPreferencesDir, Const::General::getPreferencesDir_params);
        jobject prefDir = env->CallObjectMethod(context_impl, getPreferencesDir);

        jclass File_class = env->GetObjectClass(prefDir);
        jmethodID getAbsolutePath = RET_FUNC3(env->GetMethodID, File_class,
                                              Const::General::getAbsolutePath, Const::General::getAbsolutePath_params);
        jstring prefPath = (jstring)env->CallObjectMethod(prefDir, getAbsolutePath);
        safe_prefPath = JNIApi::Utils::jstring_ToCppString(env, prefPath);

        LOGD("Preference Path -> %s", safe_prefPath.c_str());

        // 取得外部儲存路徑
        jmethodID getExternalFilesDir = RET_FUNC3(env->GetMethodID, ContextImpl_class,
                                                  Const::General::getExternalFilesDir, Const::General::getExternalFilesDir_params);
        jobject extDir = env->CallObjectMethod(context_impl, getExternalFilesDir, 0);
        jstring extPath = (jstring)env->CallObjectMethod(extDir, getAbsolutePath);
        safe_extPath = JNIApi::Utils::jstring_ToCppString(env, extPath);

        // 取得 files 儲存路徑
        jmethodID getFilesDir = RET_FUNC3(env->GetMethodID, ContextImpl_class,
                                          Const::General::getFilesDir, Const::General::getFilesDir_params);
        jobject fileDir = env->CallObjectMethod(context_impl, getFilesDir, 0);
        jstring filePath = (jstring)env->CallObjectMethod(fileDir, getAbsolutePath);
        safe_filePath = JNIApi::Utils::jstring_ToCppString(env, filePath);

        LOGD("File Path -> %s", safe_filePath.c_str());
}

bool jPassApiCheck(JNIEnv *env, jobject context)
{
        jclass ByPass_class = RET_FUNC1(env->FindClass, Const::General::ByPass_class);
        jmethodID passApiCheck = RET_FUNC3(env->GetStaticMethodID, ByPass_class,
                                           Const::General::passApiCheck, Const::General::passApiCheck_params);
        return env->CallStaticBooleanMethod(ByPass_class, passApiCheck, context);
}

void JNI_attachBaseContext(JNIEnv *env, jobject thiz, jobject context)
{
        jPassApiCheck(env, context);

        hookPMS(env, thiz, context);

        // base.attachBaseContext
        jclass thiz_clss = env->GetObjectClass(thiz);
        jclass super_class = env->GetSuperclass(thiz_clss);
        jmethodID baseMethodId = RET_FUNC3(env->GetMethodID, super_class,
                                           Const::General::attachBaseContext, Const::General::attachBaseContext_params);
        env->CallNonvirtualVoidMethod(thiz, super_class, baseMethodId, context);

        initPath(env, context);
}

jobject JNI_invoke(JNIEnv *env, jobject thiz, jobject proxy, jobject method,
                   jobjectArray args)
{

        LOGD("native invoke");

        // 運行原函數並取得原返回值
        jclass Method_class = env->GetObjectClass(method);
        jmethodID method_invoke = RET_FUNC3(env->GetMethodID,
                                            Method_class, Const::General::invoke, Const::General::invoke_params);
        jobject invoke_object = env->CallObjectMethod(method, method_invoke, pkgManager, args);

        // 獲取方法名
        jmethodID method_getName = RET_FUNC3(env->GetMethodID,
                                             Method_class, Const::General::getName, Const::General::getName_params);
        jstring methodName = (jstring)env->CallObjectMethod(method, method_getName);
        std::string cpp_methodName = JNIApi::Utils::jstring_ToCppString(env, methodName);

        LOGD("method name %s", cpp_methodName.c_str());

        if (cpp_methodName == Const::General::getPackageInfo)
        {

                // 獲取包名
                jstring pkgName = (jstring)env->GetObjectArrayElement(args, 0);
                std::string cpp_pkgName = JNIApi::Utils::jstring_ToCppString(env, pkgName);

                // 獲取 Action flag
                jobject flag = env->GetObjectArrayElement(args, 1);
                jclass Integer_class = env->GetObjectClass(flag);
                jmethodID integer_intValue = RET_FUNC3(env->GetMethodID, Integer_class,
                                                       Const::General::intValue, Const::General::intValue_params);
                int32_t c_flag = env->CallIntMethod(flag, integer_intValue);

                if ((c_flag & GET_SIGNATURES) && cpp_pkgName == appPkgName)
                {

                        // 分配儲存簽名的空間
                        int32_t len = env->GetArrayLength(sign);
                        jclass Signature_class = RET_FUNC1(env->FindClass, Const::General::Signature_class);
                        jobjectArray signature_array = env->NewObjectArray(len, Signature_class, 0);

                        // 填充簽名
                        for (int32_t i = 0; i < len; ++i)
                        {
                                jmethodID Signature_method_init = RET_FUNC3(env->GetMethodID, Signature_class,
                                                                            Const::General::init_method, Const::General::init_params);
                                jbyteArray block_sign = (jbyteArray)env->GetObjectArrayElement(sign, i);
                                jobject m_signature = env->NewObject(Signature_class, Signature_method_init,
                                                                     block_sign);
                                env->SetObjectArrayElement(signature_array, i, m_signature);
                                env->DeleteLocalRef(m_signature);
                                env->DeleteLocalRef(block_sign);
                        }

                        // 替換簽名
                        jclass PackageInfo_class = env->GetObjectClass(invoke_object);
                        jfieldID PackageInfo_field_signatures = RET_FUNC3(env->GetFieldID, PackageInfo_class,
                                                                          Const::General::signatures, Const::General::Signature_array_type);
                        env->SetObjectField(invoke_object, PackageInfo_field_signatures, signature_array);
                        env->DeleteLocalRef(signature_array);
                }
        }

        LOGD("native invoke finish");

        return invoke_object;
}
