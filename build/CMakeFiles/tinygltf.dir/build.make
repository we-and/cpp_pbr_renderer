# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.27.9/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.27.9/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/jd/dev/consulting/cpp_pbr_renderer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/jd/dev/consulting/cpp_pbr_renderer/build

# Include any dependencies generated for this target.
include CMakeFiles/tinygltf.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tinygltf.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tinygltf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tinygltf.dir/flags.make

CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.o: CMakeFiles/tinygltf.dir/flags.make
CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.o: /Users/jd/dev/consulting/cpp_pbr_renderer/lib/tinygltf/tiny_gltf.cc
CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.o: CMakeFiles/tinygltf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/jd/dev/consulting/cpp_pbr_renderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.o -MF CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.o.d -o CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.o -c /Users/jd/dev/consulting/cpp_pbr_renderer/lib/tinygltf/tiny_gltf.cc

CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/jd/dev/consulting/cpp_pbr_renderer/lib/tinygltf/tiny_gltf.cc > CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.i

CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/jd/dev/consulting/cpp_pbr_renderer/lib/tinygltf/tiny_gltf.cc -o CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.s

# Object files for target tinygltf
tinygltf_OBJECTS = \
"CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.o"

# External object files for target tinygltf
tinygltf_EXTERNAL_OBJECTS =

libtinygltf.a: CMakeFiles/tinygltf.dir/lib/tinygltf/tiny_gltf.cc.o
libtinygltf.a: CMakeFiles/tinygltf.dir/build.make
libtinygltf.a: CMakeFiles/tinygltf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/jd/dev/consulting/cpp_pbr_renderer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libtinygltf.a"
	$(CMAKE_COMMAND) -P CMakeFiles/tinygltf.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tinygltf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tinygltf.dir/build: libtinygltf.a
.PHONY : CMakeFiles/tinygltf.dir/build

CMakeFiles/tinygltf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tinygltf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tinygltf.dir/clean

CMakeFiles/tinygltf.dir/depend:
	cd /Users/jd/dev/consulting/cpp_pbr_renderer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/jd/dev/consulting/cpp_pbr_renderer /Users/jd/dev/consulting/cpp_pbr_renderer /Users/jd/dev/consulting/cpp_pbr_renderer/build /Users/jd/dev/consulting/cpp_pbr_renderer/build /Users/jd/dev/consulting/cpp_pbr_renderer/build/CMakeFiles/tinygltf.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/tinygltf.dir/depend
