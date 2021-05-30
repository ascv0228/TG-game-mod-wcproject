#pragma once

#include <string>

#include "../il2cpp-appdata.h"

namespace Il2CppApi {

    class String {
    private:
        Il2CppString **_str;
    public:
        String(Il2CppString *&str) {
            _str = &str;
        }

        String(Il2CppString *&&str) {
            _str = &str;
        }

        void set(Il2CppString *str) {
            *_str = str;
        }

        void set(std::string str) {
            *_str = il2cpp_string_new(str.c_str());
        }

        Il2CppString* get() {
            return *_str;
        }

        std::string to_string() {
            std::string str = "";

            auto str_addr = *_str;
            if(str_addr) {
                for(int32_t i = 0; i < str_addr->length; ++i) {
                    str += (char)str_addr->chars[i];
                }
            }

            return str;
        }
    };
}