cmake_minimum_required(VERSION 3.4.1)

include_directories(src/main/cpp/include)
include_directories(src/main/cpp/src/common)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})

add_library(tengine SHARED IMPORTED)
set_target_properties(tengine PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libtengine-lite.so)
add_library(opencv SHARED IMPORTED)
set_target_properties(opencv PROPERTIES IMPORTED_LOCATION ${LIB_DIR}/libopencv_java4.so)

aux_source_directory(src/main/cpp/src/common tengine_example)

add_library(
        tengine_demo
        SHARED
        src/main/cpp/include/impl.c
        ${tengine_example}
        src/main/cpp/src/tm_openpose.cpp
        src/main/cpp/src/tm_landmark.cpp
        src/main/cpp/src/jni_main.cpp
        src/main/cpp/src/GraphParam.cpp
        src/main/cpp/src/tgwrapper.cpp
        src/main/cpp/src/openpose.cpp
        src/main/cpp/src/JTgwrapper.cpp
)
find_library( # Sets the name of the path variable.
        log-lib

        # Specifies the name of the NDK library that
        # you want CMake to locate.
        log)

target_link_libraries(
        tengine_demo
        jnigraphics
        tengine
        opencv
)