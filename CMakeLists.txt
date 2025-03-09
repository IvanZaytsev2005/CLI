cmake_minimum_required(VERSION 3.28)

add_library(cli src/cli.c)

target_include_directories(cli PUBLIC inc)