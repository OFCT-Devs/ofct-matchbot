cmake_minimum_required(VERSION 3.10.2)
project(ircbot)

find_package(OpenSSL REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(ircbot
    main.c
    match.h match.c
    typedef.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE crypto ssl pthread /lib/libircclient.so)