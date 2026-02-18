# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html
# Set the minimum required version of CMake
cmake_minimum_required(VERSION 3.22.1)

# Set standards for C and C++
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

# Set the project name based on the name given on the gradle.properties
project("${APP_LIB_NAME}")

# Include raylib and raymob as subdirectories
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/freetype)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/harfbuzz)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/raylib)
add_subdirectory(${CMAKE_SOURCE_DIR}/deps/raymob)

# --- libzmq + czmq: use pre-built static libraries from czmq's builds/android ---
set(CZMQ_PREFIX ${CMAKE_SOURCE_DIR}/deps/zeromq/czmq/builds/android/prefix)

# Map Android ABI to the folder name the build script uses
if(ANDROID_ABI STREQUAL "arm64-v8a")
    set(ZMQ_ARCH_DIR ${CZMQ_PREFIX}/arm64)
elseif(ANDROID_ABI STREQUAL "armeabi-v7a")
    set(ZMQ_ARCH_DIR ${CZMQ_PREFIX}/arm)
elseif(ANDROID_ABI STREQUAL "x86_64")
    set(ZMQ_ARCH_DIR ${CZMQ_PREFIX}/x86_64)
elseif(ANDROID_ABI STREQUAL "x86")
    set(ZMQ_ARCH_DIR ${CZMQ_PREFIX}/x86)
endif()

# Import pre-built libzmq
add_library(zmq STATIC IMPORTED)
set_target_properties(zmq PROPERTIES
    IMPORTED_LOCATION ${ZMQ_ARCH_DIR}/lib/libzmq.a
)

# Import pre-built libczmq
add_library(czmq STATIC IMPORTED)
set_target_properties(czmq PROPERTIES
    IMPORTED_LOCATION ${ZMQ_ARCH_DIR}/lib/libczmq.a
)
# --------------------------------------------------------------

# Explicitly list source files (all .c files in the cpp directory)
set(SOURCES
    ${CMAKE_SOURCE_DIR}/Array.c
    ${CMAKE_SOURCE_DIR}/Board.c
    ${CMAKE_SOURCE_DIR}/Button.c
    ${CMAKE_SOURCE_DIR}/Client.c
    ${CMAKE_SOURCE_DIR}/EndScreen.c
    ${CMAKE_SOURCE_DIR}/Game.c
    ${CMAKE_SOURCE_DIR}/LanguageSelection.c
    ${CMAKE_SOURCE_DIR}/MainMenu.c
    ${CMAKE_SOURCE_DIR}/Queue.c
    ${CMAKE_SOURCE_DIR}/SaveData.c
    ${CMAKE_SOURCE_DIR}/SlotSelector.c
    ${CMAKE_SOURCE_DIR}/TamilText.c
    ${CMAKE_SOURCE_DIR}/TextRender.c
    ${CMAKE_SOURCE_DIR}/Translation.c
)

# Add headers directory for android_native_app_glue.c
include_directories(${ANDROID_NDK}/sources/android/native_app_glue/)

# Add android_native_app_glue.c to the source files
list(APPEND SOURCES ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Create a shared library with game source files
add_library(${APP_LIB_NAME} SHARED ${SOURCES})

# Define compiler macros for the library
target_compile_definitions(${APP_LIB_NAME} PRIVATE PLATFORM_ANDROID)
target_compile_definitions(${APP_LIB_NAME} PRIVATE PLATFORM_ANDROID)
target_compile_definitions(${APP_LIB_NAME} PRIVATE SERVER_URL="${SERVER_URL}")

# Apply flags depending on the build type
if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(${APP_LIB_NAME} PRIVATE _DEBUG DEBUG)
endif()

# Include raylib and raymob header files
target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/deps/raylib/src")
target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/deps/raymob")
target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/deps/freetype/include")
target_include_directories(${APP_LIB_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/deps/harfbuzz/src")
target_include_directories(${APP_LIB_NAME} PRIVATE "${ZMQ_ARCH_DIR}/include")

# Link required libraries to the native application
# Order matters: dependencies should come after the libraries that depend on them
target_link_libraries(${APP_LIB_NAME} 
    raylib
    raymoblib
    freetype
    harfbuzz
    czmq
    zmq
    log
    android
    EGL
    GLESv2
    OpenSLES
    m
)
