cmake_minimum_required(VERSION 3.20..3.27)

get_filename_component(PARENT_DIR ../ ABSOLUTE)
include_directories(${PARENT_DIR})

# RythmForge loading tests
FILE(GLOB_RECURSE LOAD_SRC "../src/load/*.cpp")

add_executable(test_pcm test_pcm.cpp ${LOAD_SRC})
add_test(PCM_unit_tests test_pcm WORKING_DIRECTORY<${PARENT_DIR})
