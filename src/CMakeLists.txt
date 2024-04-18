cmake_minimum_required(VERSION 3.20..3.27)


FILE(GLOB_RECURSE SRC "src/*.cpp")

add_executable(rythm_forge_cpp_exe main.cpp ${SRC})

ADD_LIBRARY(
        RythmForgeLoadCpp ${SRC}
)
set_target_properties(RythmForgeLoadCpp PROPERTIES POSITION_INDEPENDENT_CODE ON)

pybind11_add_module(rythm_forge_load_cpp modules/load_module.cpp)
target_link_libraries(rythm_forge_load_cpp PRIVATE RythmForgeLoadCpp)
target_compile_definitions(rythm_forge_load_cpp
        PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

pybind11_add_module(rythm_forge_core_cpp modules/core_module.cpp)
target_compile_definitions(rythm_forge_core_cpp
        PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
