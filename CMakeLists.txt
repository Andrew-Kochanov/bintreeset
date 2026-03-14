cmake_minimum_required(VERSION 3.14)
project(bst)

add_library(bst src/lib.c)

if(MSVC)
  target_compile_options(bst PRIVATE /W4 /WX /O2 /std:clatest)
else()
  target_compile_options(bst PRIVATE -Wall -Wextra -Wpedantic -O2 -std=gnu2x)
endif()
