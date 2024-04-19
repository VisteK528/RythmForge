cmake_minimum_required(VERSION 3.20..27)
project(RythmForge)

if(POLICY CMP0148)
    cmake_policy(SET CMP0148 OLD)
endif()

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD -std=c++20)
set(PYBIND11_PYTHON_VERSION 3.10)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../../rythm_forge/lib/)

# Find PyBind11
find_package(pybind11 REQUIRED)
set(PYBIND11_FINDPYTHON ON)

add_subdirectory(src)

include(CTest)
enable_testing()
