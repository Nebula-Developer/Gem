cmake_minimum_required(VERSION 3.23)
project(Gem)

set(CMAKE_CXX_STANDARD 14)

add_executable(Gem main.cpp args.cpp args.h compile/compile.cpp compile/compile.h)
