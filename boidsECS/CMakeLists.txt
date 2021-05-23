#Standalone library definition and installation
#Does not change compiler warnings!

cmake_minimum_required(VERSION 3.10)

# set the project name
project(boidsECS VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

# add the executable
add_executable(boidsECS main.cpp)
