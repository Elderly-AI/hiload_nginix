cmake_minimum_required(VERSION 3.17)
project(hiload_nginx C)

set(CMAKE_C_STANDARD 99)
add_library(hello SHARED hello.c hello.h)
find_package (Threads)

add_executable(hiload_nginx main.c stack.c stack.h mem_manager.c mem_manager.h queue.c queue.h tp_pool.c tp_pool.h hello.c hello.h)
target_link_libraries (hiload_nginx ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} rt)

