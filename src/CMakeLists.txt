cmake_minimum_required(VERSION 3.20..3.27)


FILE(GLOB_RECURSE SRC "src/*.cpp")

add_executable(rythm_forge_cpp main.cpp ${SRC})
