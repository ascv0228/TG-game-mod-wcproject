#pragma once

#include <string>

namespace Bridge
{
    struct JNINativeMethodBridge
    {
        std::string name;
        std::string signature;
        void *fnPtr;
    };

    template <typename T>
    struct Transformer
    {
        T value;

        Transformer(T val)
        {
            value = val;
        }
    };
    /*
    template <>
    struct Transformer<Safestring> {
        std::string tmp;
        const char *value;

        Transformer(Safestring val) {
            tmp = val.GetValue();
            value = tmp.c_str();
        }
    };*/

    template <>
    struct Transformer<int>
    {
        int32_t value;

        Transformer(int val)
        {
            value = val;
        }
    };

    template <>
    struct Transformer<bool>
    {
        bool value;

        Transformer(bool val)
        {
            value = val;
        }
    };

    template <>
    struct Transformer<std::string>
    {
        std::string tmp;
        const char *value;

        Transformer(std::string val)
        {
            tmp = val;
            value = tmp.c_str();
        }
    };

    template <size_t size>
    struct Transformer<char[size]>
    {
        char *value;

        Transformer(char (&val)[size])
        {
            value = val;
        }
    };

    template <size_t size>
    struct Transformer<const char (&)[size]>
    {
        const char *value;

        Transformer(const char (&val)[size])
        {
            value = val;
        }
    };
    /*
    template <size_t size>
    struct Transformer<int64_t[size]> {
        std::string tmp;
        const char *value;

        Transformer(int64_t (&val)[size]) {
            tmp = SAFE_CSTR(val);
            value = tmp.c_str();
        }
    };*/
}