cmake_minimum_required(VERSION 3.20..27)
project(RythmForge)

if(POLICY CMP0148)
    cmake_policy(SET CMP0148 OLD)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(PYBIND11_CPP_STANDARD -std=c++20)
set(PYBIND11_PYTHON_VERSION 3.10)

if(MSVC)
    message("Compiling with MSVC")
else()
    message("Compiling with GCC/Clang")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 REQUIRED)

set(PYBIND11_FINDPYTHON ON)
include_directories(${Python3_INCLUDE_DIRS})
include_directories(${pybind11_INCLUDE_DIRS})


find_package(Boost REQUIRED)

if (Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
    add_definitions(-DBOOST_ALL_NO_LIB)
else()
    message(FATAL_ERROR "Boost not found!")
endif()

add_subdirectory(src)

include(CTest)
enable_testing()

# Verbose Makefile for better debugging
set(CMAKE_VERBOSE_MAKEFILE ON)

message(STATUS "Python3 include dirs: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python3 libraries: ${Python3_LIBRARIES}")
message(STATUS "pybind11 include dirs: ${pybind11_INCLUDE_DIRS}")
