cmake_minimum_required(VERSION 3.26.4)

project(lab_02)

add_executable(lab_02
        main.c
        DES.c
        DES.h
)
target_link_libraries(lab_02 m)