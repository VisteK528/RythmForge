cmake_minimum_required(VERSION 3.20..27)
project(RythmForge)

if(POLICY CMP0148)
    cmake_policy(SET CMP0148 OLD)
endif()

set(CMAKE_CXX_STANDARD 20)
set(PYBIND11_CPP_STANDARD -std=c++20)
set(PYBIND11_PYTHON_VERSION 3.10)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

execute_process(
        COMMAND ${Python3_EXECUTABLE} -c "import os; os.system('python3 -m site --user-site')"
        OUTPUT_VARIABLE PYTHON_SITE_PACKAGES
        OUTPUT_STRIP_TRAILING_WHITESPACE
)
set(pybind11_DIR ${PYTHON_SITE_PACKAGES}/pybind11/include)
message(${PYTHON_SITE_PACKAGES}/pybind11/include)

# Find PyBind11
find_package(pybind11 REQUIRED)
set(PYBIND11_FINDPYTHON ON)

add_subdirectory(src)

include(CTest)
enable_testing()
