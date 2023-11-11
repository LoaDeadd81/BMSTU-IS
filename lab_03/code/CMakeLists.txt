cmake_minimum_required(VERSION 3.26.4)

project(lab_03)

add_executable(lab_03
        main.c
        AES.c
        AES.h
)
target_link_libraries(lab_03 m)
add_executable(tmp ../tmp.c)
