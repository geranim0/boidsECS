#!/bin/bash

./build.xsh --build_dir test --build_missing_conan --build_type Release

if [ $? -ne 0 ]; then
echo Compilation failed
else
echo Compilation success, running tests
test/build/Release/bin/boidsECSTests
fi