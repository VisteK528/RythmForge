cmake_minimum_required(VERSION 3.20..27)
project(RythmForge)

if(POLICY CMP0148)
    cmake_policy(SET CMP0148 OLD)
endif()

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD -std=c++20)
set(PYBIND11_PYTHON_VERSION 3.10)

if(MSVC)
    message("MSVC")
else()  # GCC and Clang
    message("GCC / Clang")
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Find PyBind11
find_package(pybind11 REQUIRED)
set(PYBIND11_FINDPYTHON ON)


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
