# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mykola/CLionProjects/path-tracing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mykola/CLionProjects/path-tracing/build

# Include any dependencies generated for this target.
include CMakeFiles/path_tracing.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/path_tracing.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/path_tracing.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/path_tracing.dir/flags.make

CMakeFiles/path_tracing.dir/main.cpp.o: CMakeFiles/path_tracing.dir/flags.make
CMakeFiles/path_tracing.dir/main.cpp.o: ../main.cpp
CMakeFiles/path_tracing.dir/main.cpp.o: CMakeFiles/path_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mykola/CLionProjects/path-tracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/path_tracing.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/path_tracing.dir/main.cpp.o -MF CMakeFiles/path_tracing.dir/main.cpp.o.d -o CMakeFiles/path_tracing.dir/main.cpp.o -c /home/mykola/CLionProjects/path-tracing/main.cpp

CMakeFiles/path_tracing.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/path_tracing.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mykola/CLionProjects/path-tracing/main.cpp > CMakeFiles/path_tracing.dir/main.cpp.i

CMakeFiles/path_tracing.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/path_tracing.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mykola/CLionProjects/path-tracing/main.cpp -o CMakeFiles/path_tracing.dir/main.cpp.s

CMakeFiles/path_tracing.dir/compute_shader.cpp.o: CMakeFiles/path_tracing.dir/flags.make
CMakeFiles/path_tracing.dir/compute_shader.cpp.o: ../compute_shader.cpp
CMakeFiles/path_tracing.dir/compute_shader.cpp.o: CMakeFiles/path_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mykola/CLionProjects/path-tracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/path_tracing.dir/compute_shader.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/path_tracing.dir/compute_shader.cpp.o -MF CMakeFiles/path_tracing.dir/compute_shader.cpp.o.d -o CMakeFiles/path_tracing.dir/compute_shader.cpp.o -c /home/mykola/CLionProjects/path-tracing/compute_shader.cpp

CMakeFiles/path_tracing.dir/compute_shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/path_tracing.dir/compute_shader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mykola/CLionProjects/path-tracing/compute_shader.cpp > CMakeFiles/path_tracing.dir/compute_shader.cpp.i

CMakeFiles/path_tracing.dir/compute_shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/path_tracing.dir/compute_shader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mykola/CLionProjects/path-tracing/compute_shader.cpp -o CMakeFiles/path_tracing.dir/compute_shader.cpp.s

CMakeFiles/path_tracing.dir/shader.cpp.o: CMakeFiles/path_tracing.dir/flags.make
CMakeFiles/path_tracing.dir/shader.cpp.o: ../shader.cpp
CMakeFiles/path_tracing.dir/shader.cpp.o: CMakeFiles/path_tracing.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/mykola/CLionProjects/path-tracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/path_tracing.dir/shader.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/path_tracing.dir/shader.cpp.o -MF CMakeFiles/path_tracing.dir/shader.cpp.o.d -o CMakeFiles/path_tracing.dir/shader.cpp.o -c /home/mykola/CLionProjects/path-tracing/shader.cpp

CMakeFiles/path_tracing.dir/shader.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/path_tracing.dir/shader.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/mykola/CLionProjects/path-tracing/shader.cpp > CMakeFiles/path_tracing.dir/shader.cpp.i

CMakeFiles/path_tracing.dir/shader.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/path_tracing.dir/shader.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/mykola/CLionProjects/path-tracing/shader.cpp -o CMakeFiles/path_tracing.dir/shader.cpp.s

# Object files for target path_tracing
path_tracing_OBJECTS = \
"CMakeFiles/path_tracing.dir/main.cpp.o" \
"CMakeFiles/path_tracing.dir/compute_shader.cpp.o" \
"CMakeFiles/path_tracing.dir/shader.cpp.o"

# External object files for target path_tracing
path_tracing_EXTERNAL_OBJECTS =

path_tracing: CMakeFiles/path_tracing.dir/main.cpp.o
path_tracing: CMakeFiles/path_tracing.dir/compute_shader.cpp.o
path_tracing: CMakeFiles/path_tracing.dir/shader.cpp.o
path_tracing: CMakeFiles/path_tracing.dir/build.make
path_tracing: /usr/lib/x86_64-linux-gnu/libGL.so
path_tracing: /usr/lib/x86_64-linux-gnu/libglfw.so.3.3
path_tracing: /usr/lib/x86_64-linux-gnu/libGLEW.so
path_tracing: CMakeFiles/path_tracing.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/mykola/CLionProjects/path-tracing/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable path_tracing"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/path_tracing.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/path_tracing.dir/build: path_tracing
.PHONY : CMakeFiles/path_tracing.dir/build

CMakeFiles/path_tracing.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/path_tracing.dir/cmake_clean.cmake
.PHONY : CMakeFiles/path_tracing.dir/clean

CMakeFiles/path_tracing.dir/depend:
	cd /home/mykola/CLionProjects/path-tracing/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mykola/CLionProjects/path-tracing /home/mykola/CLionProjects/path-tracing /home/mykola/CLionProjects/path-tracing/build /home/mykola/CLionProjects/path-tracing/build /home/mykola/CLionProjects/path-tracing/build/CMakeFiles/path_tracing.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/path_tracing.dir/depend

