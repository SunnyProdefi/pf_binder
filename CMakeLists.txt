cmake_minimum_required(VERSION 3.16)

project(pf_binder LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BINDER_USE_PKGCONFIG "Use pkg-config to locate libbinder" ON)

if(BINDER_USE_PKGCONFIG)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(LIBBINDER QUIET libbinder)
    endif()
endif()

set(BINDER_INCLUDE_DIR "" CACHE PATH "Path to Android binder headers (e.g. /usr/include)")
set(UTILS_INCLUDE_DIR "" CACHE PATH "Path to Android utils headers (e.g. /usr/include)")

if(NOT LIBBINDER_FOUND)
    find_path(BINDER_INCLUDE_DIR
        NAMES binder/IServiceManager.h
        PATHS
            ${BINDER_INCLUDE_DIR}
            ${ANDROID_ROOT}/system/libbinder/include
            ${ANDROID_ROOT}/frameworks/native/include
            /usr/include
    )

    find_path(UTILS_INCLUDE_DIR
        NAMES utils/String16.h
        PATHS
            ${UTILS_INCLUDE_DIR}
            ${ANDROID_ROOT}/system/core/libutils/include
            ${ANDROID_ROOT}/frameworks/native/include
            /usr/include
    )

    find_library(BINDER_LIB NAMES binder)
    find_library(UTILS_LIB NAMES utils)
    find_library(LOG_LIB NAMES log)
endif()

add_library(hello_binder STATIC
    src/IHello.cpp
)

target_include_directories(hello_binder
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

if(LIBBINDER_FOUND)
    target_include_directories(hello_binder PUBLIC ${LIBBINDER_INCLUDE_DIRS})
    target_link_libraries(hello_binder PUBLIC ${LIBBINDER_LIBRARIES})
else()
    if(NOT BINDER_INCLUDE_DIR OR NOT UTILS_INCLUDE_DIR)
        message(FATAL_ERROR "libbinder headers not found. Set ANDROID_ROOT, BINDER_INCLUDE_DIR, and UTILS_INCLUDE_DIR.")
    endif()
    if(NOT BINDER_LIB OR NOT UTILS_LIB OR NOT LOG_LIB)
        message(FATAL_ERROR "libbinder libraries not found. Set library search paths or install libbinder.")
    endif()
    target_include_directories(hello_binder PUBLIC ${BINDER_INCLUDE_DIR} ${UTILS_INCLUDE_DIR})
    target_link_libraries(hello_binder PUBLIC ${BINDER_LIB} ${UTILS_LIB} ${LOG_LIB})
endif()

add_executable(hello_service src/HelloService.cpp)
target_link_libraries(hello_service PRIVATE hello_binder)

add_executable(hello_client src/HelloClient.cpp)
target_link_libraries(hello_client PRIVATE hello_binder)
