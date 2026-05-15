cmake_minimum_required(VERSION 3.29)
project(prog C)

set(CMAKE_C_STANDARD 11)

add_executable(prog main.c
        ui.c
        panel.c
        panel.h
        fs.c
        fs.h
        input.c
        input.h
)
