cmake_minimum_required(VERSION 3.30)
project(psw_queue C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wextra -pedantic) #-Wextra -pedantic -Werror

add_executable(psw_queue queue.c test.c
        queue.h)
