# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sam/development/boidsECS/test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sam/development/boidsECS/test/build/Release

# Include any dependencies generated for this target.
include CMakeFiles/boidsECSTests.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/boidsECSTests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/boidsECSTests.dir/flags.make

CMakeFiles/boidsECSTests.dir/ECS.test.cpp.o: CMakeFiles/boidsECSTests.dir/flags.make
CMakeFiles/boidsECSTests.dir/ECS.test.cpp.o: ../../ECS.test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/development/boidsECS/test/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/boidsECSTests.dir/ECS.test.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/boidsECSTests.dir/ECS.test.cpp.o -c /home/sam/development/boidsECS/test/ECS.test.cpp

CMakeFiles/boidsECSTests.dir/ECS.test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boidsECSTests.dir/ECS.test.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/development/boidsECS/test/ECS.test.cpp > CMakeFiles/boidsECSTests.dir/ECS.test.cpp.i

CMakeFiles/boidsECSTests.dir/ECS.test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boidsECSTests.dir/ECS.test.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/development/boidsECS/test/ECS.test.cpp -o CMakeFiles/boidsECSTests.dir/ECS.test.cpp.s

CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.o: CMakeFiles/boidsECSTests.dir/flags.make
CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.o: ../../gmeta.test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/sam/development/boidsECS/test/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.o"
	/usr/bin/clang++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.o -c /home/sam/development/boidsECS/test/gmeta.test.cpp

CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/sam/development/boidsECS/test/gmeta.test.cpp > CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.i

CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/sam/development/boidsECS/test/gmeta.test.cpp -o CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.s

# Object files for target boidsECSTests
boidsECSTests_OBJECTS = \
"CMakeFiles/boidsECSTests.dir/ECS.test.cpp.o" \
"CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.o"

# External object files for target boidsECSTests
boidsECSTests_EXTERNAL_OBJECTS =

bin/boidsECSTests: CMakeFiles/boidsECSTests.dir/ECS.test.cpp.o
bin/boidsECSTests: CMakeFiles/boidsECSTests.dir/gmeta.test.cpp.o
bin/boidsECSTests: CMakeFiles/boidsECSTests.dir/build.make
bin/boidsECSTests: CMakeFiles/boidsECSTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/sam/development/boidsECS/test/build/Release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable bin/boidsECSTests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/boidsECSTests.dir/link.txt --verbose=$(VERBOSE)
	/usr/bin/cmake -D TEST_TARGET=boidsECSTests -D TEST_EXECUTABLE=/home/sam/development/boidsECS/test/build/Release/bin/boidsECSTests -D TEST_EXECUTOR= -D TEST_WORKING_DIR=/home/sam/development/boidsECS/test/build/Release -D TEST_EXTRA_ARGS= -D TEST_PROPERTIES= -D TEST_PREFIX= -D TEST_SUFFIX= -D NO_PRETTY_TYPES=FALSE -D NO_PRETTY_VALUES=FALSE -D TEST_LIST=boidsECSTests_TESTS -D CTEST_FILE=/home/sam/development/boidsECS/test/build/Release/boidsECSTests[1]_tests.cmake -D TEST_DISCOVERY_TIMEOUT=5 -P /usr/share/cmake-3.16/Modules/GoogleTestAddTests.cmake

# Rule to build all files generated by this target.
CMakeFiles/boidsECSTests.dir/build: bin/boidsECSTests

.PHONY : CMakeFiles/boidsECSTests.dir/build

CMakeFiles/boidsECSTests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/boidsECSTests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/boidsECSTests.dir/clean

CMakeFiles/boidsECSTests.dir/depend:
	cd /home/sam/development/boidsECS/test/build/Release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sam/development/boidsECS/test /home/sam/development/boidsECS/test /home/sam/development/boidsECS/test/build/Release /home/sam/development/boidsECS/test/build/Release /home/sam/development/boidsECS/test/build/Release/CMakeFiles/boidsECSTests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/boidsECSTests.dir/depend
