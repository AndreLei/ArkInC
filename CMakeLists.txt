cmake_minimum_required(VERSION 2.8)

project(ArkInC)
SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "0")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

add_executable(${PROJECT_NAME}
    "main.c"
    "libcurl/include/curl/curl.h"
    "libcurl/include/curl/curlver.h"
    "libcurl/include/curl/easy.h"
    "libcurl/include/curl/mprintf.h"
    "libcurl/include/curl/multi.h"
    "libcurl/include/curl/stdcheaders.h"
    "libcurl/include/curl/system.h"
    "libcurl/include/curl/typecheck-gcc.h")

INCLUDE_DIRECTORIES(${PROJECT_NAME}
    "libcurl/lib"
    "libcurl/include/curl"
    "libcurl/bin")

LINK_DIRECTORIES(${PROJECT_NAME} "libcurl/lib")
TARGET_LINK_LIBRARIES(${PROJECT_NAME} "libcurl/bin")

