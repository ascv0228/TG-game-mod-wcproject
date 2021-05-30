#include <jni.h>
#include <unistd.h>

#include <base64.hpp>
#include <Logger.h>
#include <Utils.h>
#include <obfuscate.hpp>
#include <BridgeFunc.h>
#include <string>
#include <cstdint>
#include <pthread.h>

#include "Hook/HookEntry.h"
#include "Hook/HookFuncs.h"
#include "Hook/Data/Config.h"
#include "Hook/Data/AppInfo.h"
#include "Hook/Data/Defs.h"
#include "Const/Const_General.h"
#include "il2cpp/il2cpp-appdata.h"

/*
void test(int32_t result)
{

}

void Show()
{

    Il2CppObject *UI_Common = il2cpp_object_new(UI_Common_Class);
    auto UI_Common_ctor = (void (*)(void*)) getAbsoluteAddress("libil2cpp.so", 0x14F29A8);
    UI_Common_ctor(UI_Common);

    auto get_CurrentDialog = (void* (*)(void*)) getAbsoluteAddress("libil2cpp.so", 0x14F2AA0);
    void *dialog = get_CurrentDialog(UI_Common);
    Il2CppString *str = il2cpp_string_new("Test");
    if(dialog) {
        //Awake(dialog);
        new_OpenDialog(UI_Common, 0, str, 0, false);
    } else {
        LOGD("Dialog is null!");
    }
    //il2cpp_gc_disable();

    Il2CppObject *UM_inst = NULL;
    il2cpp_field_static_get_value(UIManager_inst, &UM_inst);
    if(!UM_inst) return;
    LOGD("get UM_inst %p", UM_inst);
    LOGD("UM_inst cctor %d %d", UM_inst->klass->has_cctor, UM_inst->klass->cctor_finished);
    auto UIManager_ctor = (void (*)(void*)) getAbsoluteAddress("libil2cpp.so", 0x24F8584);
    UIManager_ctor(UM_inst);

    Il2CppObject *commonUI = (Il2CppObject*) get_CommonUI(UM_inst);
    if(!commonUI) return;
    LOGD("get commonUI %p", commonUI);
    LOGD("commonUI cctor %d %d", commonUI->klass->has_cctor, commonUI->klass->cctor_finished);
    auto Common_ctor = (void (*)(void*)) getAbsoluteAddress("libil2cpp.so", 0x14F29A8);
    Common_ctor(commonUI);

    if(!Action_Class) return;
    LOGD("get Action_Class %p: %s", Action_Class, Action_Class->name);

    void *onComplete = il2cpp_object_new(Action_Class);
    if(!onComplete) return;
    LOGD("get onComplete %p", onComplete);

    //const MethodInfo *method = il2cpp_class_get_method_from_name(Action_Class, ".ctor", 2);
    auto Action_ctor = (void (*)(void *instance, void *obj, void *ptr, void *info)) getAbsoluteAddress("libil2cpp.so", 0x279B248);
    LOGD("get method .ctor");

    //void *param[2] = {NULL, func};
    //method->invoker_method(method->methodPointer, method, onComplete, param);
    Action_ctor(onComplete, NULL, (void*)test, (void*)56110840);
    LOGD("invoker method .ctor");
    static Il2CppString *str = il2cpp_string_new("Test");
    //OpenDialog(commonUI, 0, str, 0, false);
    auto UI_Dialog_ctor = (void (*)(void*)) getAbsoluteAddress("libil2cpp.so", 0x1A82BEC);
    auto get_CurrentDialog = (void* (*)(void*)) getAbsoluteAddress("libil2cpp.so", 0x14F2AA0);

    Il2CppObject *dialog = (Il2CppObject*) get_CurrentDialog(commonUI);
    LOGD("dialog cctor %d %d", dialog->klass->has_cctor, dialog->klass->cctor_finished);
    if(dialog) {
        UI_Dialog_ctor(dialog);
        new_OpenDialog(commonUI, 0, str, 0, 0);
    } else {
        LOGD("Dialog is null!");
    }
    //il2cpp_gc_enable();
}*/

void *hack_thread(void *)
{
    LOGD(OBFUSCATE("pthread called"));

    do
    {
        sleep(1);
    } while (!AppInfo::isAppLoaded);

#ifdef NDEBUG

#ifdef JP
    if (AppInfo::version != Const::General::jp_version)
    {
        int32_t *p = 0;
        *p = 1;
    }
#elif defined(TW)
    if (AppInfo::version != Const::General::tw_version)
    {
        int32_t *p = 0;
        *p = 1;
    }
#endif

#endif

    LOGD(OBFUSCATE("Checked"));

    LOGD("Version = %s", AppInfo::version.c_str());

    do
    {
        sleep(1);
    } while (!RET_FUNC1(isLibraryLoaded, Const::General::libil2cpp_so));

    LOGD(OBFUSCATE("Lib %s has been loaded"), Const::General::libil2cpp_so.c_str());

    while (!RET_FUNC1(isLibraryLoaded, Const::General::libTechGG_so))
    {
        LOGD(OBFUSCATE("Check self-1!"));
    }

    while (RET_FUNC1(findLibrary, Const::General::libTechGG_so) < AppInfo::isAppLoaded)
    {
        LOGD(OBFUSCATE("Check self-2!"));
    }

    AppInfo::il2cpp_base = RET_FUNC1(findLibrary, Const::General::libil2cpp_so);

    LOGD(OBFUSCATE("hack game start"));

    if (!sub_hack_thread())
    {
        LOGD(OBFUSCATE("error find!"));
        int32_t *p = 0;
        *p = 1;
    }

    LOGD(OBFUSCATE("Hooked"));

    return NULL;
}

//No need to use JNI_OnLoad, since we don't use JNIEnv
//We do this to hide OnLoad from disassembler
__attribute__((constructor)) void lib_main()
{
    LOGD(OBFUSCATE("Own lib has been loaded"));
    // Create a new thread so it does not block the main thread, means the game would not freeze
    pthread_t ptid;
    VOID_FUNC4(pthread_create, &ptid, (pthread_attr_t *)NULL, &hack_thread, (void *)NULL);
}

/*
//Does not work yet
//\ndk\21.3.6528147\sources\android\native_app_glue
//#include <android_native_app_glue.h>

void android_main(struct android_app* state) {
    LOGD(OBFUSCATE("android_main"));
}*/
