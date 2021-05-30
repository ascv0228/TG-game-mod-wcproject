#ifndef DAWN_LOGGER_H
#define DAWN_LOGGER_H

//#define NDEBUG

#include <android/log.h>

enum daLogType
{
    daDEBUG = 3,
    daERROR = 6,
    daINFO = 4,
    daWARN = 5
};

#ifdef NDEBUG

#define LOGD(...)
#define LOGE(...)
#define LOGI(...)
#define LOGW(...)

#else

#include <obfuscate.hpp>

#define TAG OBFUSCATE("TechGG_Menu")

#define LOGD(...) ((void)__android_log_print(daDEBUG, TAG, __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(daERROR, TAG, __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(daINFO, TAG, __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(daWARN, TAG, __VA_ARGS__))

#endif //NDEBUG

#endif //DAWN_LOGGER_H