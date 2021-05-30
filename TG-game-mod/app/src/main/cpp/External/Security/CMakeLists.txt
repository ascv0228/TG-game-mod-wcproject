cmake_minimum_required(VERSION 3.4.1)

ENABLE_LANGUAGE(ASM)

set(LIBRARY_NAME "Security")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

add_library( # Sets the name of the library.
        ${LIBRARY_NAME}

        # Sets the library as a shared library.
        STATIC

        Seed.cpp)

include_directories(
)

find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries( # Specifies the target library.
        ${LIBRARY_NAME}

        # Links the target library to the log library
        # included in the NDK.
        ${log-lib})

target_include_directories(${LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
