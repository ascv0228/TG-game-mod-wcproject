#include <obfuscate.hpp>
#include <fstream>
#include <Utils.h>
#include <string>
#include <BridgeFunc.h>

#include "../Const/Const_General.h"

//const char* libName = OBFUSCATE("libil2cpp.so");//SAFE_CSTR(Const::General::libil2cpp_so); //Default target lib is Il2Cpp

std::string s(const std::string &str)
{
    const size_t p = 16;
    static const char c[p] = ".";

    std::string r = str;
    for (size_t i = 0; i < str.length(); i++)
        r[i] ^= ~c[i % p];
    return r;
}

DWORD findLibrary(const char *library)
{
    char filename[0xFF] = {0},
         buffer[1024] = {0};
    FILE *fp = NULL;
    DWORD address = 0;
    VOID_FUNC2(strcpy, filename, Const::General::map_file);

    fp = RET_FUNC2(fopen, filename, Const::General::rt);
    if (fp == NULL)
    {
        LOGD(OBFUSCATE("fopen"));
        goto done;
    }

    while (fgets(buffer, sizeof(buffer), fp))
    {
        if (strstr(buffer, library))
        {
            address = (DWORD)strtoul(buffer, NULL, 16);
            goto done;
        }
    }

done:

    if (fp)
    {
        fclose(fp);
    }

    return address;
}

DWORD getAbsoluteAddress(const char *libraryName, DWORD relativeAddr)
{
    libBase = findLibrary(libraryName);
    if (libBase == 0)
        return 0;
    return (reinterpret_cast<DWORD>(libBase + relativeAddr));
}

bool isLibraryLoaded(const char *libraryName)
{
    char line[512] = {0};
    FILE *fp = RET_FUNC2(fopen, Const::General::map_file, Const::General::rt);
    if (fp != NULL)
    {
        while (fgets(line, sizeof(line), fp))
        {
            if (strstr(line, s("\xbd\x96\x9d\x9d\x92\x8b\xd1\x8c\x90").c_str()))
            {
                int *i = (int *)0x0;
                *i = 1;
            }
            if (strstr(line, libraryName))
                return true;
        }
        fclose(fp);
    }
    return false;
}

void MakeToast(JNIEnv *env, jobject thiz, const char *text, int length)
{
    //Add our toast in here so it wont be easy to change by simply editing the smali and cant
    //be cut out because this method is needed to start the hack (Octowolve is smart)
    jstring jstr = env->NewStringUTF(text); //Edit this text to your desired toast message!
    jclass toast = RET_FUNC1(env->FindClass, Const::General::Toast_Class);
    jmethodID methodMakeText =
        RET_FUNC3(env->GetStaticMethodID,
                  toast,
                  Const::General::makeText,
                  Const::General::Toast_Method);
    if (methodMakeText == NULL)
    {
        LOGE(OBFUSCATE("toast.makeText not Found"));
        return;
    }
    //The last int is the length on how long the toast should be displayed
    //0 = Short, 1 = Long
    jobject toastobj = env->CallStaticObjectMethod(toast, methodMakeText,
                                                   thiz, jstr, length);

    jmethodID methodShow =
        RET_FUNC3(env->GetMethodID,
                  toast,
                  Const::General::show,
                  Const::General::Show_Method);
    if (methodShow == NULL)
    {
        LOGE(OBFUSCATE("toast.show not Found"));
        return;
    }
    env->CallVoidMethod(toastobj, methodShow);
}

uintptr_t string2Offset(const char *c)
{
    int base = 16;
    // See if this function catches all possibilities.
    // If it doesn't, the function would have to be amended
    // whenever you add a combination of architecture and
    // compiler that is not yet addressed.
    static_assert(sizeof(uintptr_t) == sizeof(unsigned long) || sizeof(uintptr_t) == sizeof(unsigned long long),
                  "Please add string to handle conversion for this architecture.");

    // Now choose the correct function ...
    if (sizeof(uintptr_t) == sizeof(unsigned long))
    {
        return strtoul(c, nullptr, base);
    }

    // All other options exhausted, sizeof(uintptr_t) == sizeof(unsigned long long))
    return strtoull(c, nullptr, base);
}

void writeToFile(std::string dir, std::string buf)
{
    //LOGD("load sounds %s", dir.c_str());
    std::ofstream os(dir.c_str(), std::ios::trunc);
    os.write(buf.c_str(), buf.length());
    os.close();
}