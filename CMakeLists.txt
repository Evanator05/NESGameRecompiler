cmake_minimum_required(VERSION 3.13)
project(NesRecomp)
set(CMAKE_CXX_STANDARD 14)

add_executable(NesRomRecompiler src/main.cpp)

target_include_directories(NesRomRecompiler PUBLIC src/)