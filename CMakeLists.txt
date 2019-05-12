cmake_minimum_required(VERSION 3.10)
project(UNO C)

set(CMAKE_C_STANDARD 99)

add_executable(UNO main.c cli.c cli.h file.c file.h game.c game.h player.c player.h stack.c stack.h list.c list.h)