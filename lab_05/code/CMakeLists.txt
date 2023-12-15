cmake_minimum_required(VERSION 3.26.4)

project(lab_05)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab_05
        main.cpp
)
target_link_libraries(lab_05 m)
