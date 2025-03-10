cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

add_library(cli src/cli.c)

target_link_options(cli PUBLIC
   -u _printf_float
)


include_directories(inc)