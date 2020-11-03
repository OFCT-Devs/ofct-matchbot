cmake_minimum_required(VERSION 3.10.2)
project(ofct-matchbot)

find_package(OpenSSL REQUIRED)

set(CMAKE_C_STANDARD 11)

add_executable(${PROJECT_NAME}
    main.c
    match.h match.c
    typedef.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE crypto ssl pthread /lib/libircclient.so)