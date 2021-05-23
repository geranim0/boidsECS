#Simple project wrapper
#Entry point for development
#src abd test subdirectories added with add_subdirectory()

cmake_minimum_required(VERSION 3.10)
project(boidsECS)

#add project code
add_subdirectory(src)

#add unit tests
enable_testing()
add_subdirectory(test)