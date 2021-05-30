#pragma once

#include <map>
#include <string>
#include <vector>
#include <regex>

#include <Logger.h>
#include <obfuscate.hpp>

#include "../../Hook/Data/AppInfo.h"
#include "../il2cpp-appdata.h"

namespace Il2CppApi {
    class Helper final {
    private:
        Helper() = delete;

        inline static bool isInited = false;
        inline static const char *KMATCH_PATTERN = OBFUSCATE("^kmatch\\$(.*?)$"); // 在class名稱前面加上kmatch$將會使用不完全匹配模式
        inline static std::map<std::string, const Il2CppImage*> il2cpp_assemblies;
    public:
        inline static Il2CppDomain *il2cpp_domain = nullptr;

        static bool init()
        {
            if(isInited) return false;

            il2cpp_domain = il2cpp_domain_get();
            while(!il2cpp_domain) {
                sleep(1);
                il2cpp_domain = il2cpp_domain_get();
            }
            LOGD("hack game domin get %p", il2cpp_domain);

            size_t size;
            auto assemblies = il2cpp_domain_get_assemblies(il2cpp_domain, &size);
            while(!assemblies) {
                sleep(1);
                assemblies = il2cpp_domain_get_assemblies(il2cpp_domain, &size);
            }
            LOGD("hack game get assemblies %p", assemblies);

            for (int32_t i = 0; i < size; ++i) {
                auto image = il2cpp_assembly_get_image(assemblies[i]);
                il2cpp_assemblies[image->name] = image;
            }
            LOGD("hack game set finish");

            isInited = true;

            return true;
        }

        static Il2CppClass* find_class(std::string assembly_name, std::string namespaze, std::vector<std::string> klasses)
        {
            LOGD("hack game start find class");

            auto image = il2cpp_assemblies[assembly_name];
            if(!image) return nullptr;
            LOGD("hack game find image %s: %p", image->name, image);

            auto klass = il2cpp_class_from_name(image, namespaze.c_str(), klasses[0].c_str());
            if(!klass) return nullptr;
            LOGD("hack game find class %s: %p ", klass->name, klass);

            for(auto subklass = klasses.begin() + 1; subklass != klasses.end(); ++subklass) {

                void *last_klass = klass; // 用來檢查是否發現下一層class
                void *iter = nullptr;
                while(auto itf = il2cpp_class_get_nested_types(klass, &iter)) {
                    LOGD("hack game find class %s: %p", itf->name, klass);
                    std::regex reg(KMATCH_PATTERN);
                    std::smatch groups;

                    if(std::regex_match(*subklass, groups, reg) && groups.size() > 1) { // 不完全匹配模式
                        if(std::string(itf->name).find(groups[1].str()) != std::string::npos) {
                            klass = itf;
                            LOGD("hack game find class %s: %p by kmatch", itf->name, itf);
                            break;
                        }
                    } else { // 完全匹配模式
                        if(itf->name == *subklass) {
                            klass = itf;
                            LOGD("hack game find class %s: %p ", itf->name, itf);
                            break;
                        }
                    }

                }

                if(last_klass == klass) { // 未發現下一層class
                    klass = nullptr;
                    break;
                }
            }

            return klass;
        }

        static const MethodInfo* find_method(Il2CppClass* klass, std::string funcName, int32_t argsCount)
        {
            LOGD("%p %s %d", klass, funcName.c_str(), argsCount);
            if(!klass) return nullptr;

            const MethodInfo* method = il2cpp_class_get_method_from_name(klass, funcName.c_str(), argsCount);
            if(!method) return nullptr;
            LOGD("hack game find method %s: %p ", method->name, method);

            LOGD("hack game get method %s: RVA->%p VA->%p",
                 funcName.c_str(), (void*)((uint64_t)method->methodPointer - AppInfo::il2cpp_base), method->methodPointer);

            return method;
        }

        static const MethodInfo* find_method(Il2CppClass* klass, std::string funcName, std::string returnType, std::vector<std::string> paramTypes)
        {
            void *iter = nullptr;
            while (auto method = il2cpp_class_get_methods(klass, &iter)) {

                if(method->name != funcName) continue;
                if(method->parameters_count != paramTypes.size()) continue;
                if(returnType != il2cpp_type_get_name(method->return_type)) continue;

                LOGD("returnType name: %s", il2cpp_type_get_name(method->return_type));

                bool isFind = true;
                for(int32_t i = 0; i < method->parameters_count; ++i) {

                    LOGD("parameters name: %s", il2cpp_type_get_name(method->parameters[i].parameter_type));

                    if(paramTypes[i] != il2cpp_type_get_name(method->parameters[i].parameter_type)) {
                        isFind = false;
                        break;
                    }
                }

                if(isFind) {
                    LOGD("hack game get method %s: RVA->%p VA->%p",
                         funcName.c_str(), (void*)((uint64_t)method->methodPointer - AppInfo::il2cpp_base), method->methodPointer);
                    return method;
                }
            }

            return nullptr;
        }

        static std::vector<const MethodInfo*> find_all_methods(Il2CppClass* klass, std::vector<std::string> excludes)
        {
            std::vector<const MethodInfo*> ret_methods;

            void *iter = nullptr;
            while(auto method = il2cpp_class_get_methods(klass, &iter)) {
                int32_t exclude_idx = -1;
                for(int32_t i = 0; i < excludes.size(); ++i) {
                    if(excludes[i] == method->name) {
                        exclude_idx = i;
                        break;
                    }
                }

                if(exclude_idx != -1) {
                    excludes.erase(excludes.cbegin() + exclude_idx);
                } else {
                    ret_methods.emplace_back(method);
                }
            }

            return ret_methods;
        }

        static FieldInfo* find_field(Il2CppClass* klass, std::string fieldName) {
            return il2cpp_class_get_field_from_name(klass, fieldName.c_str());
        }
    };
}