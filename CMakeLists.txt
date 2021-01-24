cmake_minimum_required(VERSION 3.17)
project(nginx_v4 C)

set(CMAKE_C_STANDARD 99)

find_package (Threads REQUIRED)

add_executable(nginx_v4 main.c server/clone_pool/shared_queue/shared_queue.c server/clone_pool/shared_queue/shared_queue.h utils/macros/macros.h utils/properties/properties.h server/clone_pool/clone_pool.c server/clone_pool/clone_pool.h server/server.c server/server.h server/handlers/handlers.c server/handlers/handlers.h utils/request_parser/request_parser.c utils/request_parser/request_parser.h utils/response_constructor/response_constructor.c utils/response_constructor/response_constructor.h server/manager/config_parser/config_parser.c server/manager/config_parser/config_parser.h utils/utilities/utilities.c utils/utilities/utilities.h server/manager/shared_hash_table/shared_hash_table.c server/manager/shared_hash_table/shared_hash_table.h server/manager/files/files.c server/manager/files/files.h server/manager/permission/permission.c server/manager/permission/permission.h server/manager/manager.c server/manager/manager.h)

add_subdirectory(testing)

target_link_libraries (nginx_v4 ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT} rt)