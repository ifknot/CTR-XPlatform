cmake_minimum_required(VERSION 3.14)
project(ctr)

set(CMAKE_CXX_STANDARD 17)

include_directories("./usr/include/sys" "crypto")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "--debug -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

add_executable(ctr
        crypto/aes_constants.h
        crypto/aes_decrypt.h
        crypto/aes_encrypt.h
        crypto/aes_reverse_constants.h
        crypto/block_cipher_constants.h
        crypto/block_cipher_factory.h
        crypto/cipher_exception.h
        crypto/error_messages.h
        crypto/nonce_factory.h
        crypto/padder_factory.h
        lorem_in.txt
        main.cpp
        README.md)
