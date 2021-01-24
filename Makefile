# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/awerkiler/CLionProjects/2/nginx_v4

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/awerkiler/CLionProjects/2/nginx_v4

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/snap/clion/138/bin/cmake/linux/bin/ccmake -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/awerkiler/CLionProjects/2/nginx_v4/CMakeFiles /home/awerkiler/CLionProjects/2/nginx_v4//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/awerkiler/CLionProjects/2/nginx_v4/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named nginx_v4

# Build rule for target.
nginx_v4: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 nginx_v4
.PHONY : nginx_v4

# fast build rule for target.
nginx_v4/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/build
.PHONY : nginx_v4/fast

#=============================================================================
# Target rules for targets named testing

# Build rule for target.
testing: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 testing
.PHONY : testing

# fast build rule for target.
testing/fast:
	$(MAKE) $(MAKESILENT) -f testing/CMakeFiles/testing.dir/build.make testing/CMakeFiles/testing.dir/build
.PHONY : testing/fast

main.o: main.c.o

.PHONY : main.o

# target to build an object file
main.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/main.c.o
.PHONY : main.c.o

main.i: main.c.i

.PHONY : main.i

# target to preprocess a source file
main.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/main.c.i
.PHONY : main.c.i

main.s: main.c.s

.PHONY : main.s

# target to generate assembly for a file
main.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/main.c.s
.PHONY : main.c.s

server/clone_pool/clone_pool.o: server/clone_pool/clone_pool.c.o

.PHONY : server/clone_pool/clone_pool.o

# target to build an object file
server/clone_pool/clone_pool.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/clone_pool/clone_pool.c.o
.PHONY : server/clone_pool/clone_pool.c.o

server/clone_pool/clone_pool.i: server/clone_pool/clone_pool.c.i

.PHONY : server/clone_pool/clone_pool.i

# target to preprocess a source file
server/clone_pool/clone_pool.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/clone_pool/clone_pool.c.i
.PHONY : server/clone_pool/clone_pool.c.i

server/clone_pool/clone_pool.s: server/clone_pool/clone_pool.c.s

.PHONY : server/clone_pool/clone_pool.s

# target to generate assembly for a file
server/clone_pool/clone_pool.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/clone_pool/clone_pool.c.s
.PHONY : server/clone_pool/clone_pool.c.s

server/clone_pool/shared_queue/shared_queue.o: server/clone_pool/shared_queue/shared_queue.c.o

.PHONY : server/clone_pool/shared_queue/shared_queue.o

# target to build an object file
server/clone_pool/shared_queue/shared_queue.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/clone_pool/shared_queue/shared_queue.c.o
.PHONY : server/clone_pool/shared_queue/shared_queue.c.o

server/clone_pool/shared_queue/shared_queue.i: server/clone_pool/shared_queue/shared_queue.c.i

.PHONY : server/clone_pool/shared_queue/shared_queue.i

# target to preprocess a source file
server/clone_pool/shared_queue/shared_queue.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/clone_pool/shared_queue/shared_queue.c.i
.PHONY : server/clone_pool/shared_queue/shared_queue.c.i

server/clone_pool/shared_queue/shared_queue.s: server/clone_pool/shared_queue/shared_queue.c.s

.PHONY : server/clone_pool/shared_queue/shared_queue.s

# target to generate assembly for a file
server/clone_pool/shared_queue/shared_queue.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/clone_pool/shared_queue/shared_queue.c.s
.PHONY : server/clone_pool/shared_queue/shared_queue.c.s

server/handlers/handlers.o: server/handlers/handlers.c.o

.PHONY : server/handlers/handlers.o

# target to build an object file
server/handlers/handlers.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/handlers/handlers.c.o
.PHONY : server/handlers/handlers.c.o

server/handlers/handlers.i: server/handlers/handlers.c.i

.PHONY : server/handlers/handlers.i

# target to preprocess a source file
server/handlers/handlers.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/handlers/handlers.c.i
.PHONY : server/handlers/handlers.c.i

server/handlers/handlers.s: server/handlers/handlers.c.s

.PHONY : server/handlers/handlers.s

# target to generate assembly for a file
server/handlers/handlers.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/handlers/handlers.c.s
.PHONY : server/handlers/handlers.c.s

server/manager/config_parser/config_parser.o: server/manager/config_parser/config_parser.c.o

.PHONY : server/manager/config_parser/config_parser.o

# target to build an object file
server/manager/config_parser/config_parser.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/config_parser/config_parser.c.o
.PHONY : server/manager/config_parser/config_parser.c.o

server/manager/config_parser/config_parser.i: server/manager/config_parser/config_parser.c.i

.PHONY : server/manager/config_parser/config_parser.i

# target to preprocess a source file
server/manager/config_parser/config_parser.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/config_parser/config_parser.c.i
.PHONY : server/manager/config_parser/config_parser.c.i

server/manager/config_parser/config_parser.s: server/manager/config_parser/config_parser.c.s

.PHONY : server/manager/config_parser/config_parser.s

# target to generate assembly for a file
server/manager/config_parser/config_parser.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/config_parser/config_parser.c.s
.PHONY : server/manager/config_parser/config_parser.c.s

server/manager/files/files.o: server/manager/files/files.c.o

.PHONY : server/manager/files/files.o

# target to build an object file
server/manager/files/files.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/files/files.c.o
.PHONY : server/manager/files/files.c.o

server/manager/files/files.i: server/manager/files/files.c.i

.PHONY : server/manager/files/files.i

# target to preprocess a source file
server/manager/files/files.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/files/files.c.i
.PHONY : server/manager/files/files.c.i

server/manager/files/files.s: server/manager/files/files.c.s

.PHONY : server/manager/files/files.s

# target to generate assembly for a file
server/manager/files/files.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/files/files.c.s
.PHONY : server/manager/files/files.c.s

server/manager/manager.o: server/manager/manager.c.o

.PHONY : server/manager/manager.o

# target to build an object file
server/manager/manager.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/manager.c.o
.PHONY : server/manager/manager.c.o

server/manager/manager.i: server/manager/manager.c.i

.PHONY : server/manager/manager.i

# target to preprocess a source file
server/manager/manager.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/manager.c.i
.PHONY : server/manager/manager.c.i

server/manager/manager.s: server/manager/manager.c.s

.PHONY : server/manager/manager.s

# target to generate assembly for a file
server/manager/manager.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/manager.c.s
.PHONY : server/manager/manager.c.s

server/manager/permission/permission.o: server/manager/permission/permission.c.o

.PHONY : server/manager/permission/permission.o

# target to build an object file
server/manager/permission/permission.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/permission/permission.c.o
.PHONY : server/manager/permission/permission.c.o

server/manager/permission/permission.i: server/manager/permission/permission.c.i

.PHONY : server/manager/permission/permission.i

# target to preprocess a source file
server/manager/permission/permission.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/permission/permission.c.i
.PHONY : server/manager/permission/permission.c.i

server/manager/permission/permission.s: server/manager/permission/permission.c.s

.PHONY : server/manager/permission/permission.s

# target to generate assembly for a file
server/manager/permission/permission.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/permission/permission.c.s
.PHONY : server/manager/permission/permission.c.s

server/manager/shared_hash_table/shared_hash_table.o: server/manager/shared_hash_table/shared_hash_table.c.o

.PHONY : server/manager/shared_hash_table/shared_hash_table.o

# target to build an object file
server/manager/shared_hash_table/shared_hash_table.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/shared_hash_table/shared_hash_table.c.o
.PHONY : server/manager/shared_hash_table/shared_hash_table.c.o

server/manager/shared_hash_table/shared_hash_table.i: server/manager/shared_hash_table/shared_hash_table.c.i

.PHONY : server/manager/shared_hash_table/shared_hash_table.i

# target to preprocess a source file
server/manager/shared_hash_table/shared_hash_table.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/shared_hash_table/shared_hash_table.c.i
.PHONY : server/manager/shared_hash_table/shared_hash_table.c.i

server/manager/shared_hash_table/shared_hash_table.s: server/manager/shared_hash_table/shared_hash_table.c.s

.PHONY : server/manager/shared_hash_table/shared_hash_table.s

# target to generate assembly for a file
server/manager/shared_hash_table/shared_hash_table.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/manager/shared_hash_table/shared_hash_table.c.s
.PHONY : server/manager/shared_hash_table/shared_hash_table.c.s

server/server.o: server/server.c.o

.PHONY : server/server.o

# target to build an object file
server/server.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/server.c.o
.PHONY : server/server.c.o

server/server.i: server/server.c.i

.PHONY : server/server.i

# target to preprocess a source file
server/server.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/server.c.i
.PHONY : server/server.c.i

server/server.s: server/server.c.s

.PHONY : server/server.s

# target to generate assembly for a file
server/server.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/server/server.c.s
.PHONY : server/server.c.s

utils/request_parser/request_parser.o: utils/request_parser/request_parser.c.o

.PHONY : utils/request_parser/request_parser.o

# target to build an object file
utils/request_parser/request_parser.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/utils/request_parser/request_parser.c.o
.PHONY : utils/request_parser/request_parser.c.o

utils/request_parser/request_parser.i: utils/request_parser/request_parser.c.i

.PHONY : utils/request_parser/request_parser.i

# target to preprocess a source file
utils/request_parser/request_parser.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/utils/request_parser/request_parser.c.i
.PHONY : utils/request_parser/request_parser.c.i

utils/request_parser/request_parser.s: utils/request_parser/request_parser.c.s

.PHONY : utils/request_parser/request_parser.s

# target to generate assembly for a file
utils/request_parser/request_parser.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/utils/request_parser/request_parser.c.s
.PHONY : utils/request_parser/request_parser.c.s

utils/response_constructor/response_constructor.o: utils/response_constructor/response_constructor.c.o

.PHONY : utils/response_constructor/response_constructor.o

# target to build an object file
utils/response_constructor/response_constructor.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/utils/response_constructor/response_constructor.c.o
.PHONY : utils/response_constructor/response_constructor.c.o

utils/response_constructor/response_constructor.i: utils/response_constructor/response_constructor.c.i

.PHONY : utils/response_constructor/response_constructor.i

# target to preprocess a source file
utils/response_constructor/response_constructor.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/utils/response_constructor/response_constructor.c.i
.PHONY : utils/response_constructor/response_constructor.c.i

utils/response_constructor/response_constructor.s: utils/response_constructor/response_constructor.c.s

.PHONY : utils/response_constructor/response_constructor.s

# target to generate assembly for a file
utils/response_constructor/response_constructor.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/utils/response_constructor/response_constructor.c.s
.PHONY : utils/response_constructor/response_constructor.c.s

utils/utilities/utilities.o: utils/utilities/utilities.c.o

.PHONY : utils/utilities/utilities.o

# target to build an object file
utils/utilities/utilities.c.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/utils/utilities/utilities.c.o
.PHONY : utils/utilities/utilities.c.o

utils/utilities/utilities.i: utils/utilities/utilities.c.i

.PHONY : utils/utilities/utilities.i

# target to preprocess a source file
utils/utilities/utilities.c.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/utils/utilities/utilities.c.i
.PHONY : utils/utilities/utilities.c.i

utils/utilities/utilities.s: utils/utilities/utilities.c.s

.PHONY : utils/utilities/utilities.s

# target to generate assembly for a file
utils/utilities/utilities.c.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/nginx_v4.dir/build.make CMakeFiles/nginx_v4.dir/utils/utilities/utilities.c.s
.PHONY : utils/utilities/utilities.c.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... nginx_v4"
	@echo "... testing"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... server/clone_pool/clone_pool.o"
	@echo "... server/clone_pool/clone_pool.i"
	@echo "... server/clone_pool/clone_pool.s"
	@echo "... server/clone_pool/shared_queue/shared_queue.o"
	@echo "... server/clone_pool/shared_queue/shared_queue.i"
	@echo "... server/clone_pool/shared_queue/shared_queue.s"
	@echo "... server/handlers/handlers.o"
	@echo "... server/handlers/handlers.i"
	@echo "... server/handlers/handlers.s"
	@echo "... server/manager/config_parser/config_parser.o"
	@echo "... server/manager/config_parser/config_parser.i"
	@echo "... server/manager/config_parser/config_parser.s"
	@echo "... server/manager/files/files.o"
	@echo "... server/manager/files/files.i"
	@echo "... server/manager/files/files.s"
	@echo "... server/manager/manager.o"
	@echo "... server/manager/manager.i"
	@echo "... server/manager/manager.s"
	@echo "... server/manager/permission/permission.o"
	@echo "... server/manager/permission/permission.i"
	@echo "... server/manager/permission/permission.s"
	@echo "... server/manager/shared_hash_table/shared_hash_table.o"
	@echo "... server/manager/shared_hash_table/shared_hash_table.i"
	@echo "... server/manager/shared_hash_table/shared_hash_table.s"
	@echo "... server/server.o"
	@echo "... server/server.i"
	@echo "... server/server.s"
	@echo "... utils/request_parser/request_parser.o"
	@echo "... utils/request_parser/request_parser.i"
	@echo "... utils/request_parser/request_parser.s"
	@echo "... utils/response_constructor/response_constructor.o"
	@echo "... utils/response_constructor/response_constructor.i"
	@echo "... utils/response_constructor/response_constructor.s"
	@echo "... utils/utilities/utilities.o"
	@echo "... utils/utilities/utilities.i"
	@echo "... utils/utilities/utilities.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system
