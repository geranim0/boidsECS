#!/bin/bash

./build.xsh --build_dir boidsECS --build_missing_conan --build_type Release

if [ $? -ne 0 ]; then
echo Compilation failed
else
echo Compilation success, running boidsECS
(cd boidsECS/build/Release/bin/ ; ./boidsECS)
fi