cmake_minimum_required(VERSION 3.26.4)

project(lab_04)

add_executable(lab_04
        main.c
        md5.h
        md5.c
        rsa.h
        rsa.c
)
target_link_libraries(lab_04 m)
