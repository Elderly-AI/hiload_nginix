/*
 * Описание функций для работы с структурой manager.
 * Manager отвечвет за разбор конфигурационного файла,
 * проверку корректности клиенсткого запроса,
 * поиск и получение отдаваемых файлов.
 */

#pragma once

#include "files/files.h"
#include "permission/permission.h"
#include "config_parser/config_parser.h"
#include "shared_hash_table/shared_hash_table.h"

/*
 * Структура, описывающая manager
 * file_manager - указатель shared hash table, описывающая file manager
 * permissions - указатель shared hash table, описывающая permission manager
 * config - указатель на структуру server_config
 */
typedef struct {
    shared_hash_table *file_manager;
    shared_hash_table *permissions;
    server_config *config;
} manager;

manager *init_manager (char *config_path);
int free_manager (manager *server_manager);
config_command *get_request_command (manager *server_manager, char *request_type_path);
file *get_request_file (manager *server_manager, config_command *command);
