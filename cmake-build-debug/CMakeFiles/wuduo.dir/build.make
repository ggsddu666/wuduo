# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/wuay/CLionProjects/git/wuduo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/wuay/CLionProjects/git/wuduo/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/wuduo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/wuduo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/wuduo.dir/flags.make

CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o: CMakeFiles/wuduo.dir/flags.make
CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o: ../src/Logging_unittest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wuay/CLionProjects/git/wuduo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o -c /Users/wuay/CLionProjects/git/wuduo/src/Logging_unittest.cpp

CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wuay/CLionProjects/git/wuduo/src/Logging_unittest.cpp > CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.i

CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wuay/CLionProjects/git/wuduo/src/Logging_unittest.cpp -o CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.s

CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o.requires:

.PHONY : CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o.requires

CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o.provides: CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o.requires
	$(MAKE) -f CMakeFiles/wuduo.dir/build.make CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o.provides.build
.PHONY : CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o.provides

CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o.provides.build: CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o


CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o: CMakeFiles/wuduo.dir/flags.make
CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o: ../src/Timestamp_unittest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wuay/CLionProjects/git/wuduo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o -c /Users/wuay/CLionProjects/git/wuduo/src/Timestamp_unittest.cpp

CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wuay/CLionProjects/git/wuduo/src/Timestamp_unittest.cpp > CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.i

CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wuay/CLionProjects/git/wuduo/src/Timestamp_unittest.cpp -o CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.s

CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o.requires:

.PHONY : CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o.requires

CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o.provides: CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o.requires
	$(MAKE) -f CMakeFiles/wuduo.dir/build.make CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o.provides.build
.PHONY : CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o.provides

CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o.provides.build: CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o


CMakeFiles/wuduo.dir/src/base/Condition.cpp.o: CMakeFiles/wuduo.dir/flags.make
CMakeFiles/wuduo.dir/src/base/Condition.cpp.o: ../src/base/Condition.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wuay/CLionProjects/git/wuduo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/wuduo.dir/src/base/Condition.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wuduo.dir/src/base/Condition.cpp.o -c /Users/wuay/CLionProjects/git/wuduo/src/base/Condition.cpp

CMakeFiles/wuduo.dir/src/base/Condition.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wuduo.dir/src/base/Condition.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wuay/CLionProjects/git/wuduo/src/base/Condition.cpp > CMakeFiles/wuduo.dir/src/base/Condition.cpp.i

CMakeFiles/wuduo.dir/src/base/Condition.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wuduo.dir/src/base/Condition.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wuay/CLionProjects/git/wuduo/src/base/Condition.cpp -o CMakeFiles/wuduo.dir/src/base/Condition.cpp.s

CMakeFiles/wuduo.dir/src/base/Condition.cpp.o.requires:

.PHONY : CMakeFiles/wuduo.dir/src/base/Condition.cpp.o.requires

CMakeFiles/wuduo.dir/src/base/Condition.cpp.o.provides: CMakeFiles/wuduo.dir/src/base/Condition.cpp.o.requires
	$(MAKE) -f CMakeFiles/wuduo.dir/build.make CMakeFiles/wuduo.dir/src/base/Condition.cpp.o.provides.build
.PHONY : CMakeFiles/wuduo.dir/src/base/Condition.cpp.o.provides

CMakeFiles/wuduo.dir/src/base/Condition.cpp.o.provides.build: CMakeFiles/wuduo.dir/src/base/Condition.cpp.o


CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o: CMakeFiles/wuduo.dir/flags.make
CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o: ../src/base/Mutex.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wuay/CLionProjects/git/wuduo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o -c /Users/wuay/CLionProjects/git/wuduo/src/base/Mutex.cpp

CMakeFiles/wuduo.dir/src/base/Mutex.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wuduo.dir/src/base/Mutex.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wuay/CLionProjects/git/wuduo/src/base/Mutex.cpp > CMakeFiles/wuduo.dir/src/base/Mutex.cpp.i

CMakeFiles/wuduo.dir/src/base/Mutex.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wuduo.dir/src/base/Mutex.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wuay/CLionProjects/git/wuduo/src/base/Mutex.cpp -o CMakeFiles/wuduo.dir/src/base/Mutex.cpp.s

CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o.requires:

.PHONY : CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o.requires

CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o.provides: CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o.requires
	$(MAKE) -f CMakeFiles/wuduo.dir/build.make CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o.provides.build
.PHONY : CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o.provides

CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o.provides.build: CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o


CMakeFiles/wuduo.dir/src/base/Logging.cpp.o: CMakeFiles/wuduo.dir/flags.make
CMakeFiles/wuduo.dir/src/base/Logging.cpp.o: ../src/base/Logging.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wuay/CLionProjects/git/wuduo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/wuduo.dir/src/base/Logging.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/wuduo.dir/src/base/Logging.cpp.o -c /Users/wuay/CLionProjects/git/wuduo/src/base/Logging.cpp

CMakeFiles/wuduo.dir/src/base/Logging.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/wuduo.dir/src/base/Logging.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wuay/CLionProjects/git/wuduo/src/base/Logging.cpp > CMakeFiles/wuduo.dir/src/base/Logging.cpp.i

CMakeFiles/wuduo.dir/src/base/Logging.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/wuduo.dir/src/base/Logging.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wuay/CLionProjects/git/wuduo/src/base/Logging.cpp -o CMakeFiles/wuduo.dir/src/base/Logging.cpp.s

CMakeFiles/wuduo.dir/src/base/Logging.cpp.o.requires:

.PHONY : CMakeFiles/wuduo.dir/src/base/Logging.cpp.o.requires

CMakeFiles/wuduo.dir/src/base/Logging.cpp.o.provides: CMakeFiles/wuduo.dir/src/base/Logging.cpp.o.requires
	$(MAKE) -f CMakeFiles/wuduo.dir/build.make CMakeFiles/wuduo.dir/src/base/Logging.cpp.o.provides.build
.PHONY : CMakeFiles/wuduo.dir/src/base/Logging.cpp.o.provides

CMakeFiles/wuduo.dir/src/base/Logging.cpp.o.provides.build: CMakeFiles/wuduo.dir/src/base/Logging.cpp.o


# Object files for target wuduo
wuduo_OBJECTS = \
"CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o" \
"CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o" \
"CMakeFiles/wuduo.dir/src/base/Condition.cpp.o" \
"CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o" \
"CMakeFiles/wuduo.dir/src/base/Logging.cpp.o"

# External object files for target wuduo
wuduo_EXTERNAL_OBJECTS =

bin/wuduo: CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o
bin/wuduo: CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o
bin/wuduo: CMakeFiles/wuduo.dir/src/base/Condition.cpp.o
bin/wuduo: CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o
bin/wuduo: CMakeFiles/wuduo.dir/src/base/Logging.cpp.o
bin/wuduo: CMakeFiles/wuduo.dir/build.make
bin/wuduo: CMakeFiles/wuduo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wuay/CLionProjects/git/wuduo/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable bin/wuduo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/wuduo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/wuduo.dir/build: bin/wuduo

.PHONY : CMakeFiles/wuduo.dir/build

CMakeFiles/wuduo.dir/requires: CMakeFiles/wuduo.dir/src/Logging_unittest.cpp.o.requires
CMakeFiles/wuduo.dir/requires: CMakeFiles/wuduo.dir/src/Timestamp_unittest.cpp.o.requires
CMakeFiles/wuduo.dir/requires: CMakeFiles/wuduo.dir/src/base/Condition.cpp.o.requires
CMakeFiles/wuduo.dir/requires: CMakeFiles/wuduo.dir/src/base/Mutex.cpp.o.requires
CMakeFiles/wuduo.dir/requires: CMakeFiles/wuduo.dir/src/base/Logging.cpp.o.requires

.PHONY : CMakeFiles/wuduo.dir/requires

CMakeFiles/wuduo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/wuduo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/wuduo.dir/clean

CMakeFiles/wuduo.dir/depend:
	cd /Users/wuay/CLionProjects/git/wuduo/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wuay/CLionProjects/git/wuduo /Users/wuay/CLionProjects/git/wuduo /Users/wuay/CLionProjects/git/wuduo/cmake-build-debug /Users/wuay/CLionProjects/git/wuduo/cmake-build-debug /Users/wuay/CLionProjects/git/wuduo/cmake-build-debug/CMakeFiles/wuduo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/wuduo.dir/depend

