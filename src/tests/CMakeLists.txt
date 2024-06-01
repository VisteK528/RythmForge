cmake_minimum_required(VERSION 3.20..3.27)

enable_testing()
get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(${PARENT_DIR})

# RythmForge loading tests
FILE(GLOB_RECURSE LOAD_SRC "../src/load/*.cpp")
FILE(GLOB_RECURSE TEST_SRC "*.cpp")


add_executable(rythm_forge_cpp_unit_tests ${TEST_SRC} ${LOAD_SRC})
add_test(RythmForgeTests rythm_forge_cpp_unit_tests WORKING_DIRECTORY:${PARENT_DIR})
