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