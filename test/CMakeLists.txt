#Standalone unit tests definition

cmake_minimum_required(VERSION 3.10)
project(boidsECSTests)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#dependencies
include(CTest)

#needed to have access to our own project headers
include_directories(..)

#find_package(GTest MODULE REQUIRED)

#here, we only want to find package if we run the standalone tests.
#if we are being run from top level, we already know about boidsECS package
#and we wont enter in the if branch.

#if(NOT TARGET geranim0::boidsECS)
#    find_package(boidsECS CONFIG REQUIRED)
#endif()
#
##target definition
#add_executable(boidsECSTests tests.cpp)
#target_link_libraries(boidsECSTest
#    PRIVATE
#        geranim0::boidsECS
#        GTest::Main
#)
#
#add_test(NAME boidsECS.UnitTests
#    COMMAND boidsECSTests
#)


#testing testing
#enable_testing()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(
    boidsECSTests
  ../boidsECS/helpers.cpp
  ECS.test.cpp
  gmeta.test.cpp
  bresenham.test.cpp
  flocon_generator.test.cpp
)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${CONAN_LIBS})

include(GoogleTest)
gtest_discover_tests(boidsECSTests)