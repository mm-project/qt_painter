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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/levibyte/git/N/qt_painter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/levibyte/git/N/qt_painter

# Utility rule file for commands_autogen.

# Include the progress variables for this target.
include src/commands/CMakeFiles/commands_autogen.dir/progress.make

src/commands/CMakeFiles/commands_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/levibyte/git/N/qt_painter/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC for target commands"
	cd /home/levibyte/git/N/qt_painter/src/commands && /usr/bin/cmake -E cmake_autogen /home/levibyte/git/N/qt_painter/src/commands/CMakeFiles/commands_autogen.dir ""

commands_autogen: src/commands/CMakeFiles/commands_autogen
commands_autogen: src/commands/CMakeFiles/commands_autogen.dir/build.make

.PHONY : commands_autogen

# Rule to build all files generated by this target.
src/commands/CMakeFiles/commands_autogen.dir/build: commands_autogen

.PHONY : src/commands/CMakeFiles/commands_autogen.dir/build

src/commands/CMakeFiles/commands_autogen.dir/clean:
	cd /home/levibyte/git/N/qt_painter/src/commands && $(CMAKE_COMMAND) -P CMakeFiles/commands_autogen.dir/cmake_clean.cmake
.PHONY : src/commands/CMakeFiles/commands_autogen.dir/clean

src/commands/CMakeFiles/commands_autogen.dir/depend:
	cd /home/levibyte/git/N/qt_painter && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/levibyte/git/N/qt_painter /home/levibyte/git/N/qt_painter/src/commands /home/levibyte/git/N/qt_painter /home/levibyte/git/N/qt_painter/src/commands /home/levibyte/git/N/qt_painter/src/commands/CMakeFiles/commands_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/commands/CMakeFiles/commands_autogen.dir/depend

