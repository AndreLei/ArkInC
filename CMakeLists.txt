cmake_minimum_required(VERSION 2.8)

project(ArkInC)
SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")
SET(VERSION_PATCH "0")
SET(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

include_directories(${CMAKE_SOURCE_DIR}/libcurl/include/curl)
include_directories(${CMAKE_SOURCE_DIR}/secp256k1/include)
include_directories(${CMAKE_SOURCE_DIR}/secp256k1/src)

set(SOURCE_FILES
    arkapi.h
    arkapi.c
    arkmodels.h
    cJSON.h
    cJSON.c
    jsonresponse.h
    jsonresponse.c
    main.h
    main.c)

set(SECP256K1
    ${CMAKE_SOURCE_DIR}/secp256k1/src/basic-config.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/ecdsa.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/ecdsa_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/eckey.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/eckey_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/ecmult.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/ecmult_const.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/ecmult_const_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/ecmult_gen.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/ecmult_gen_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/ecmult_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/field.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/field_10x26.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/field_10x26_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/field_5x52.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/field_5x52_asm_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/field_5x52_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/field_5x52_int128_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/field_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/group.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/group_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/hash.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/hash_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/num.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/num_gmp.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/num_gmp_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/num_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/scalar.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/scalar_4x64.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/scalar_4x64_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/scalar_8x32.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/scalar_8x32_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/scalar_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/scalar_low.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/scalar_low_impl.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/secp256k1.c
    ${CMAKE_SOURCE_DIR}/secp256k1/src/util.h

    ${CMAKE_SOURCE_DIR}/secp256k1/include/secp256k1.h
    ${CMAKE_SOURCE_DIR}/secp256k1/include/secp256k1_ecdh.h
    ${CMAKE_SOURCE_DIR}/secp256k1/include/secp256k1_recovery.h

    ${CMAKE_SOURCE_DIR}/secp256k1/src/libsecp256k1-config.h
    ${CMAKE_SOURCE_DIR}/secp256k1/src/gmp.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${SECP256K1})

if(WIN32)
    add_definitions(-DCURL_STATICLIB)
    target_link_libraries(ArkInC ${CMAKE_SOURCE_DIR}/libcurl/lib/libcurl_a.lib)
endif()
