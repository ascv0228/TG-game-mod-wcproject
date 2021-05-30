#pragma once

#include <string>
#include <cstdint>

class AppInfo
{
public:
    static bool isAppLoaded;
    static bool isApiInited;
    static bool isHookInited;
    static std::string version;
    static bool isSendWin;
    static int64_t il2cpp_base;
};
