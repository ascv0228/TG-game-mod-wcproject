#pragma once

#define KS(...) {__VA_ARGS__}

#define FIELD_OFFSET(var, klass, field) \
    var.field = [&]() -> auto { \
        auto ks = klass; \
        return RET_FUNC2(Il2CppApi::Helper::find_field, ks, Const::Field::field); \
    }()->offset

#define STRUCT_FIELD_OFFSET(var, klass, field) \
    var.field = [&]() -> auto { \
        auto ks = klass; \
        return RET_FUNC2(Il2CppApi::Helper::find_field, ks, Const::Field::field); \
    }()->offset - sizeof(Il2CppObject)

#define STRING_FIELD_OFFSET(var, klass, field) \
    var.field = [&]() -> auto { \
        auto ks = klass; \
        return RET_FUNC2(Il2CppApi::Helper::find_field, ks, #field); \
    }()->offset

#define FIELD_SN_ADDR(var, klass, field) \
    var = [&]() -> auto { \
        auto ks = klass; \
        return RET_FUNC2(Il2CppApi::Helper::find_field, ks, Const::Field::field); \
    }()

#define CLASS(var, enc_assembly, enc_namespaze, klass) \
    var = [&]() -> auto { \
        auto ks = klass; \
        return RET_FUNC3(Il2CppApi::Helper::find_class, enc_assembly, enc_namespaze, ks); \
    }()


#define METHOD(klass, func, argsCount) \
    func = (decltype(func)) [&]() -> auto { \
        auto ks = klass; \
        return RET_FUNC3(Il2CppApi::Helper::find_method, ks, Const::Method::func, argsCount); \
    }()->methodPointer;

#define METHOD_P(assembly, namespaze, klasses, preName, func, argsCount) \
    preName##_##func = (decltype(preName##_##func)) RET_FUNC3(Il2CppApi::Helper::find_method, assembly, namespaze, klasses, #func, argsCount)->methodPointer

#define METHOD_SN(klass, preName, func, argsCount) \
    preName = (decltype(preName)) [&]() -> auto { \
        auto ks = klass; \
        return RET_FUNC3(Il2CppApi::Helper::find_method, ks, Const::Method::func, argsCount); \
    }()->methodPointer

#define METHOD_INFO_SN(klass, var, func, argsCount) \
    var = [&]() -> auto { \
        auto ks = klass; \
        return RET_FUNC3(Il2CppApi::Helper::find_method, ks, Const::Method::func, argsCount); \
    }()

#define HOOK(methodInfo, func) \
  RET_FUNC3(il2cpp_hook, methodInfo, (void*)new_##func, (void**)&old_##func)

#define HOOK_P(methodInfo, preName, func) \
  RET_FUNC3(il2cpp_hook, methodInfo, (void*)new_##preName##_##func, (void**)&old_##preName##_##func)

#define LoadMethod(klass, func, argsCount) \
    klass##_##func = [&]() -> auto { \
        auto ks = klass; \
        return RET_FUNC3(Il2CppApi::Helper::find_method, ks, Const::Method::func, argsCount); \
    }()

bool sub_hack_thread();
