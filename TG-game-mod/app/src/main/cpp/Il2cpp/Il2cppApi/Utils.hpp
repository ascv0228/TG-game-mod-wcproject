#pragma once

#include <Logger.h>

#include "Types.h"
#include "../il2cpp-appdata.h"
#include "../../Hook/Data/AppInfo.h"

namespace Il2CppApi {
    class Utils final {
    private:
        Utils() = delete;

        static void showException(Il2CppException* exception) {
            if(exception) {
                LOGD("exception in address: %p", exception);

                LOGD("name -----> %s", exception->klass->name);

                auto class_name = String(exception->class_name).to_string();
                LOGD("class_name -----> %s", class_name.c_str());

                auto message = String(exception->message).to_string();
                LOGD("message -----> %s", message.c_str());

                auto source = String(exception->source).to_string();
                LOGD("source -----> %s", source.c_str());

                auto help_link = String(exception->help_link).to_string();
                LOGD("help_link -----> %s", help_link.c_str());

                auto stack_trace = String(exception->stack_trace).to_string();
                LOGD("stack_trace -----> %s", stack_trace.c_str());

                LOGD("------------------------------ end");
            }
        }

    public:
        template <typename T>
        static Il2CppObject* box(Il2CppClass *typeInfo, T val) {
            T _val = val;
            return il2cpp_value_box(typeInfo, &_val);
        }

        template <typename T>
        static T unbox(Il2CppObject* obj) {
            return *(T*)il2cpp_object_unbox(obj);
        }

        static Il2CppObject* invoke(const MethodInfo *methodInfo, void *instance, void **params) {
            Il2CppException* exception = nullptr;
            Il2CppObject *ret_obj = il2cpp_runtime_invoke(methodInfo, instance, params, &exception);
            showException(exception);
            return ret_obj;
        }

        template <size_t size>
        static Il2CppObject* invoke(const MethodInfo *methodInfo, void *instance, Il2CppObject *(&params)[size]) {
            Il2CppException* exception = nullptr;
            Il2CppObject *ret_obj = il2cpp_runtime_invoke_convert_args(methodInfo, instance, params, size, &exception);
            showException(exception);
            return ret_obj;
        }
    };
}