cmake_minimum_required(VERSION 3.4.1)

set(LIBRARY_NAME "TechGG")

option(EnableOLLVM "Enable OLLVM Obfuscator" OFF)
option(EnableSecurity "Enable Security Type" ON)
option(EnableDobby "Enable Dobby Hook" ON)
option(EnableWhale "Enable Whale Hook" ON)
option(EnableSandHook "Enable SandHook Hook" OFF)
option(EnableSubstrate "Enable Substrate Hook" ON)
option(EnableKittyMemory "Enable KittyMemory Hook" ON)
option(EnableAnd64InlineHook "Enable And64InlineHook Hook" OFF)

set(CMAKE_COMMON_FLAGS "${CMAKE_COMMON_FLAGS} -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -ffunction-sections -fdata-sections")
set(CMAKE_COMMON_FLAGS "${CMAKE_COMMON_FLAGS} -w -s -Werror -fms-extensions -Wno-error=format-security -Wno-error=c++11-narrowing")
set(CMAKE_C_FLAGS "${CMAKE_COMMON_FLAGS} ${CMAKE_C_FLAGS} -Wold-style-cast -fno-exceptions -stdlib=libc++")
set(CMAKE_CXX_FLAGS "${CMAKE_COMMON_FLAGS} ${CMAKE_CXX_FLAGS} -std=c++17")

if (EnableOLLVM)
    set(CMAKE_C_COMPILER "C:/ollvm-10.x/bin/clang.exe")
    set(CMAKE_CXX_COMPILER "C:/ollvm-10.x/bin/clang++.exe")

    set(OLLVM_FLAGS "${OLLVM_FLAGS} -mllvm -irobf")
    set(OLLVM_FLAGS "${OLLVM_FLAGS} -mllvm --irobf-indbr")
    set(OLLVM_FLAGS "${OLLVM_FLAGS} -mllvm --irobf-icall")
    set(OLLVM_FLAGS "${OLLVM_FLAGS} -mllvm --irobf-indgv")
    set(OLLVM_FLAGS "${OLLVM_FLAGS} -mllvm --irobf-cse")
    set(OLLVM_FLAGS "${OLLVM_FLAGS} -mllvm --irobf-cff")
    set(OLLVM_FLAGS "${OLLVM_FLAGS} -mllvm -enable-subobf -mllvm -sub_loop=3 -mllvm -sub_prob=100")
    set(SPEC_OLLVM_FLAGS "${OLLVM_FLAGS}")
    set(OLLVM_FLAGS "${OLLVM_FLAGS} -mllvm -enable-bcfobf -mllvm -bcf_loop=1 -enable-bcfobf -mllvm -bcf_prob=100 -mllvm -bcf_cond_compl=10")
endif()

add_subdirectory(External)

if (EnableOLLVM)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OLLVM_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OLLVM_FLAGS}")
endif()

set(LINKER_FLAGS "${LINKER_FLAGS} -ffixed-x18 -Wl,--hash-style=both")
set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,--exclude-libs,ALL")
set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,--gc-sections,--strip-all")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

include_directories(Includes)

set(tg_SOURCE
        ${tg_SOURCE}
        Main.cpp
        Hook/HookFuncs.cpp
        Hook/HookEntry.cpp
        Hook/Data/Defs.cpp
        Hook/Data/Config.cpp
        Hook/Data/AppInfo.cpp
        JNI/JNIEntry.cpp
        JNI/MApplication.cpp
        JNI/Preferences.cpp
        JNI/NativeToast.cpp
        JNI/AccountManager.cpp
        JNI/Menu.cpp
        JNI/Expand/AccountHandler.cpp
        il2cpp/il2cpp-init.cpp
        Includes/Utils.cpp
        )

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
    set(tg_SOURCE
            ${tg_SOURCE}
            Const/v7/Const_Assembly.cpp
            Const/v7/Const_NameSpaze.cpp
            Const/v7/Const_Klass.cpp
            Const/v7/Const_Method.cpp
            Const/v7/Const_Field.cpp
            Const/v7/Const_General.cpp
            Const/v7/Const_Il2cpp.cpp
            )
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
    set(tg_SOURCE
            ${tg_SOURCE}
            Const/v8/Const_Assembly.cpp
            Const/v8/Const_NameSpaze.cpp
            Const/v8/Const_Klass.cpp
            Const/v8/Const_Method.cpp
            Const/v8/Const_Field.cpp
            Const/v8/Const_General.cpp
            Const/v8/Const_Il2cpp.cpp
            )
endif ()

add_library(${LIBRARY_NAME} SHARED ${tg_SOURCE})

set_target_properties(${LIBRARY_NAME} PROPERTIES LINK_FLAGS_DEBUG -s)
set_target_properties(${LIBRARY_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)

find_library(log-lib log)

target_link_libraries(${LIBRARY_NAME} ${log-lib} ${ExternalLibrary})