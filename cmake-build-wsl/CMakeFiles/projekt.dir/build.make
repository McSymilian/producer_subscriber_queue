# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_SOURCE_DIR = /mnt/d/Studia/II/I/psw/projekt

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/Studia/II/I/psw/projekt/cmake-build-wsl

# Include any dependencies generated for this target.
include CMakeFiles/projekt.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/projekt.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/projekt.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/projekt.dir/flags.make

CMakeFiles/projekt.dir/queue.c.o: CMakeFiles/projekt.dir/flags.make
CMakeFiles/projekt.dir/queue.c.o: /mnt/d/Studia/II/I/psw/projekt/queue.c
CMakeFiles/projekt.dir/queue.c.o: CMakeFiles/projekt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/d/Studia/II/I/psw/projekt/cmake-build-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/projekt.dir/queue.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/projekt.dir/queue.c.o -MF CMakeFiles/projekt.dir/queue.c.o.d -o CMakeFiles/projekt.dir/queue.c.o -c /mnt/d/Studia/II/I/psw/projekt/queue.c

CMakeFiles/projekt.dir/queue.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/projekt.dir/queue.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/Studia/II/I/psw/projekt/queue.c > CMakeFiles/projekt.dir/queue.c.i

CMakeFiles/projekt.dir/queue.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/projekt.dir/queue.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/Studia/II/I/psw/projekt/queue.c -o CMakeFiles/projekt.dir/queue.c.s

CMakeFiles/projekt.dir/test.c.o: CMakeFiles/projekt.dir/flags.make
CMakeFiles/projekt.dir/test.c.o: /mnt/d/Studia/II/I/psw/projekt/test.c
CMakeFiles/projekt.dir/test.c.o: CMakeFiles/projekt.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/d/Studia/II/I/psw/projekt/cmake-build-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/projekt.dir/test.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/projekt.dir/test.c.o -MF CMakeFiles/projekt.dir/test.c.o.d -o CMakeFiles/projekt.dir/test.c.o -c /mnt/d/Studia/II/I/psw/projekt/test.c

CMakeFiles/projekt.dir/test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/projekt.dir/test.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/d/Studia/II/I/psw/projekt/test.c > CMakeFiles/projekt.dir/test.c.i

CMakeFiles/projekt.dir/test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/projekt.dir/test.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/d/Studia/II/I/psw/projekt/test.c -o CMakeFiles/projekt.dir/test.c.s

# Object files for target projekt
projekt_OBJECTS = \
"CMakeFiles/projekt.dir/queue.c.o" \
"CMakeFiles/projekt.dir/test.c.o"

# External object files for target projekt
projekt_EXTERNAL_OBJECTS =

projekt: CMakeFiles/projekt.dir/queue.c.o
projekt: CMakeFiles/projekt.dir/test.c.o
projekt: CMakeFiles/projekt.dir/build.make
projekt: CMakeFiles/projekt.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/d/Studia/II/I/psw/projekt/cmake-build-wsl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable projekt"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/projekt.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/projekt.dir/build: projekt
.PHONY : CMakeFiles/projekt.dir/build

CMakeFiles/projekt.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/projekt.dir/cmake_clean.cmake
.PHONY : CMakeFiles/projekt.dir/clean

CMakeFiles/projekt.dir/depend:
	cd /mnt/d/Studia/II/I/psw/projekt/cmake-build-wsl && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/Studia/II/I/psw/projekt /mnt/d/Studia/II/I/psw/projekt /mnt/d/Studia/II/I/psw/projekt/cmake-build-wsl /mnt/d/Studia/II/I/psw/projekt/cmake-build-wsl /mnt/d/Studia/II/I/psw/projekt/cmake-build-wsl/CMakeFiles/projekt.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/projekt.dir/depend

