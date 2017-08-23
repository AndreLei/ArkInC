cmake_minimum_required(VERSION 2.8)

project(ArkInC)
SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "0")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

set(SOURCE_FILES main.h
    main.c
    cJSON.h
    cJSON.c)

include_directories(${CMAKE_SOURCE_DIR}/libcurl/include/curl)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

if(WIN32)
    add_definitions(-DCURL_STATICLIB)
    target_link_libraries(ArkInC ${CMAKE_SOURCE_DIR}/libcurl/lib/libcurl_a.lib)
endif()
