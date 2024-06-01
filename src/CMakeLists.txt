cmake_minimum_required(VERSION 3.20..3.27)

# Unit tests
add_subdirectory(tests)

FILE(GLOB_RECURSE SRC_LOAD "src/load/*.cpp")
FILE(GLOB_RECURSE SRC_CORE "src/core/*.cpp")

add_executable(rythm_forge_cpp_exe main.cpp ${SRC_CORE} ${SRC_LOAD})

# Load module
ADD_LIBRARY(RythmForgeLoadCpp ${SRC_LOAD})
set_target_properties(RythmForgeLoadCpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

pybind11_add_module(rythm_forge_load_cpp modules/load_module.cpp)
target_link_libraries(rythm_forge_load_cpp PRIVATE RythmForgeLoadCpp)
target_compile_definitions(rythm_forge_load_cpp PRIVATE)

# Core Module
ADD_LIBRARY(RythmForgeCoreCpp ${SRC_CORE})
set_target_properties(RythmForgeCoreCpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

pybind11_add_module(rythm_forge_core_cpp modules/core_module.cpp)
target_link_libraries(rythm_forge_core_cpp PRIVATE RythmForgeCoreCpp RythmForgeLoadCpp)
target_compile_definitions(rythm_forge_core_cpp PRIVATE)
