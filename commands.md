# Using conan (for both src and test folders)

## Using clang
```bash
export CC=/usr/bin/clang
export CXX=/usr/bin/clang++
```

## builds all
```bash
cd build
conan install .. -b
```
## or build missing
```bash
cd build
conan install .. --build=missing
```

# Using CMake

## Release
```bash
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

## Debug
```bash
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

# Build and run tests
```bash
./build.xsh --build_dir test --build_missing_conan --build_type Release ; ./run_tests.bash
```

# Installing a new conan package
1. Edit `conanfile.txt` `[requires]` to contain our dependency
2. Go check the options of the package and set wanted options in `[options]`
3. Edit our `CMakeLists.txt` to link it using `target_link_libraries(boidsECS ${CONAN_LIBS})`
4. Navigate to `build` directory, and `conan install .. -b`