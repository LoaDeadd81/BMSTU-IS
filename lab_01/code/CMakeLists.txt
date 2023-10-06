cmake_minimum_required(VERSION 3.26.4)

project(lab_01)

add_executable(lab_01
        enigma.c
        enigma.h
        main.c
)

add_executable(gen
        gen/gen.c
)