cmake_minimum_required(VERSION 3.10)
project(crypto_labs C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(CRYPTO_LIBRARY_HEADERS
        include/crypto.h
        include/arcfour.h)

set(CRYPTO_LIBRARY_SOURCES
        src/arcfour.c)

add_library(crypto ${CRYPTO_LIBRARY_HEADERS} ${CRYPTO_LIBRARY_SOURCES})

add_executable(rc4 demo/rc4.c)
target_link_libraries(rc4 crypto)