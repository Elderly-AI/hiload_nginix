cmake_minimum_required(VERSION 3.17)
project(testing C)

set(CMAKE_C_STANDARD 99)
find_package (Threads REQUIRED)

add_executable(testing testing.c testing.h)
target_link_libraries (testing ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} rt)