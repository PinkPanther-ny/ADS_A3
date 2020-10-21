cmake_minimum_required(VERSION 3.17)
project(A3_clion C)

set(CMAKE_C_STANDARD 11)

add_executable(A3_clion peg_solitaire.c src/ai.c src/ai.h src/hashtable.c src/hashtable.h src/layouts.h src/stack.c src/stack.h src/utils.c src/utils.h)
