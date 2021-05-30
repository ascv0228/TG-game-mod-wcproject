#pragma once

#include "BridgeType.h"

#define CREATE_BRIDGE(name, var, ...) \
    auto name = Bridge::Transformer<decltype(var)>(var)

#define VARS1(var, ...) \
    CREATE_BRIDGE(_, var)
#define VARS2(var, ...) \
    CREATE_BRIDGE(__, var); \
    VARS1(__VA_ARGS__)
#define VARS3(var, ...) \
    CREATE_BRIDGE(___, var); \
    VARS2(__VA_ARGS__)
#define VARS4(var, ...) \
    CREATE_BRIDGE(____, var); \
    VARS3(__VA_ARGS__)
#define VARS5(var, ...) \
    CREATE_BRIDGE(_____, var); \
    VARS4(__VA_ARGS__)

#define VOID_FUNC0(callback) \
    [&]() -> void { \
        callback(); \
    }()
#define VOID_FUNC1(callback, a) \
    [&]() -> void { \
        VARS1(a); \
        callback(_.value); \
    }()
#define VOID_FUNC2(callback, a, b) \
    [&]() -> void { \
        VARS2(a, b); \
        callback(__.value, _.value); \
    }()
#define VOID_FUNC3(callback, a, b, c) \
    [&]() -> void { \
        VARS3(a, b, c); \
        callback(___.value, __.value, _.value); \
    }()
#define VOID_FUNC4(callback, a, b, c, d) \
    [&]() -> void { \
        VARS4(a, b, c, d); \
        callback(____.value, ___.value, __.value, _.value); \
    }()
#define VOID_FUNC5(callback, a, b, c, d, e) \
    [&]() -> void { \
        VARS5(a, b, c, d, e); \
        callback(_____.value, ____.value, ___.value, __.value, _.value); \
    }()

#define RET_FUNC0(callback) \
    [&]() -> auto { \
        return callback(); \
    }()
#define RET_FUNC1(callback, a) \
    [&]() -> auto { \
        VARS1(a); \
        return callback(_.value); \
    }()
#define RET_FUNC2(callback, a, b) \
    [&]() -> auto { \
        VARS2(a, b); \
        return callback(__.value, _.value); \
    }()
#define RET_FUNC3(callback, a, b, c) \
    [&]() -> auto { \
        VARS3(a, b, c); \
        return callback(___.value, __.value, _.value); \
    }()
#define RET_FUNC4(callback, a, b, c, d) \
    [&]() -> auto { \
        VARS4(a, b, c, d); \
        return callback(____.value, ___.value, __.value, _.value); \
    }()
#define RET_FUNC5(callback, a, b, c, d, e) \
    [&]() -> auto { \
        VARS5(a, b, c, d, e); \
        return callback(_____.value, ____.value, ___.value, __.value, _.value); \
    }()
    