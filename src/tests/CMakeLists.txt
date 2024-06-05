cmake_minimum_required(VERSION 3.20..3.27)

enable_testing()
get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(${PARENT_DIR})

# RythmForge loading tests
FILE(GLOB_RECURSE LOAD_SRC "../src/load/*.cpp")
FILE(GLOB_RECURSE CORE_SRC "../src/core/feature.cpp" "../src/core/fft.cpp")
FILE(GLOB_RECURSE INC "../include/core*.cpp")
FILE(GLOB_RECURSE TEST_SRC "*.cpp")


add_executable(rythm_forge_cpp_unit_tests ${TEST_SRC} ${LOAD_SRC} ${CORE_SRC} ${INC})
add_test(RythmForgeTests rythm_forge_cpp_unit_tests WORKING_DIRECTORY:${PARENT_DIR})
