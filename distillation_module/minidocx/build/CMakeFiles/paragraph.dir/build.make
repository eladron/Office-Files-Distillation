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
CMAKE_SOURCE_DIR = /mnt/c/Users/elad.ron/Downloads/minidocx

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/elad.ron/Downloads/minidocx/build

# Include any dependencies generated for this target.
include CMakeFiles/paragraph.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/paragraph.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/paragraph.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/paragraph.dir/flags.make

CMakeFiles/paragraph.dir/examples/paragraph.cpp.o: CMakeFiles/paragraph.dir/flags.make
CMakeFiles/paragraph.dir/examples/paragraph.cpp.o: ../examples/paragraph.cpp
CMakeFiles/paragraph.dir/examples/paragraph.cpp.o: CMakeFiles/paragraph.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/elad.ron/Downloads/minidocx/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/paragraph.dir/examples/paragraph.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/paragraph.dir/examples/paragraph.cpp.o -MF CMakeFiles/paragraph.dir/examples/paragraph.cpp.o.d -o CMakeFiles/paragraph.dir/examples/paragraph.cpp.o -c /mnt/c/Users/elad.ron/Downloads/minidocx/examples/paragraph.cpp

CMakeFiles/paragraph.dir/examples/paragraph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/paragraph.dir/examples/paragraph.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/elad.ron/Downloads/minidocx/examples/paragraph.cpp > CMakeFiles/paragraph.dir/examples/paragraph.cpp.i

CMakeFiles/paragraph.dir/examples/paragraph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/paragraph.dir/examples/paragraph.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/elad.ron/Downloads/minidocx/examples/paragraph.cpp -o CMakeFiles/paragraph.dir/examples/paragraph.cpp.s

# Object files for target paragraph
paragraph_OBJECTS = \
"CMakeFiles/paragraph.dir/examples/paragraph.cpp.o"

# External object files for target paragraph
paragraph_EXTERNAL_OBJECTS =

paragraph: CMakeFiles/paragraph.dir/examples/paragraph.cpp.o
paragraph: CMakeFiles/paragraph.dir/build.make
paragraph: libminidocx.a
paragraph: CMakeFiles/paragraph.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/elad.ron/Downloads/minidocx/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable paragraph"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/paragraph.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/paragraph.dir/build: paragraph
.PHONY : CMakeFiles/paragraph.dir/build

CMakeFiles/paragraph.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/paragraph.dir/cmake_clean.cmake
.PHONY : CMakeFiles/paragraph.dir/clean

CMakeFiles/paragraph.dir/depend:
	cd /mnt/c/Users/elad.ron/Downloads/minidocx/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/elad.ron/Downloads/minidocx /mnt/c/Users/elad.ron/Downloads/minidocx /mnt/c/Users/elad.ron/Downloads/minidocx/build /mnt/c/Users/elad.ron/Downloads/minidocx/build /mnt/c/Users/elad.ron/Downloads/minidocx/build/CMakeFiles/paragraph.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/paragraph.dir/depend

