/*
 * Описание функций для работы с структурой manager.
 * Manager отвечвет за разбор конфигурационного файла,
 * проверку корректности клиенсткого запроса,
 * поиск и получение отдаваемых файлов.
 */

#include "manager.h"

static int fill_server_config (manager *server_manager, char *config_path);
static int fill_file_manager (manager *server_manager);
static int fill_permissions (manager *server_manager);
static int set_files (shared_hash_table *table, server_config *config);

manager *init_manager (char *config_path) {
    /// Функция, инициализирующая manager
    /// config_path - строка, описывающая путь до конфигурационного файла сервера
    /// Возвращаемое значение - указатель на структуру manager, NULL при ошибке

    manager *server_manager = malloc(sizeof(manager));
    if (server_manager == ERROR_POINTER)
        return_error("malloc error in init_manager", ERROR_POINTER);
    if (fill_server_config(server_manager, config_path) == ERROR_CODE) {
        free_manager(server_manager);
        return ERROR_POINTER;
    }
    if (fill_file_manager(server_manager) == ERROR_CODE) {
        free_manager(server_manager);
        return ERROR_POINTER;
    }
    if (fill_permissions(server_manager) == ERROR_CODE) {
        free_manager(server_manager);
        return ERROR_POINTER;
    }
    return server_manager;
}

int free_manager (manager *server_manager) {
    /// Функция, деинициализирующая manager
    /// server_manager - указатель на структуру manager
    /// Возвращаемое значение 0, при ошибке -1

    int result = OK_CODE;
    if (server_manager == NULL)
        return_error("server_manager is NULL in free_manager", ERROR_CODE);
    if (server_manager->file_manager != NULL) {
        if (free_file_manager(server_manager->file_manager) == ERROR_CODE)
            result = ERROR_CODE;
    }
    if (server_manager->permissions != NULL) {
        if (free_permissions(server_manager->permissions) == ERROR_CODE)
            result = ERROR_CODE;
    }
    if (server_manager->config != NULL) {
        if (free_server_config(server_manager->config) == ERROR_CODE)
            result = ERROR_CODE;
    }
    free(server_manager);
    return result;
}

config_command *get_request_command (manager *server_manager, char *request_type_path) {
    /// Функция, получающая команду, соответствующую клиентскому запросу
    /// server_manager - указатель на структуру manager
    /// request_type_path - строка, в которой содержится тип запроса и его путь, записанные через пробел
    /// Возвращаемое значение указатель на config_command, NULL если команда не найдена

    if (server_manager == NULL || request_type_path == NULL)
        return_error("server_manager or request_path or request_type is NULL", ERROR_POINTER);
    config_command *command = check_permission(server_manager->permissions, request_type_path);
    if (command == ERROR_POINTER)
        return ERROR_POINTER;
    return command;
}

file *get_request_file (manager *server_manager, config_command *command) {
    /// Функция, получающая файл, соответствующий входящей команде
    /// server_manager - указатель на структуру manager
    /// command - команада в которой содержится требуемый файл
    /// Возвращаемое значение указатель на структуру file,
    /// которая содержит указатель на начало разделяемой памяти и длинну файла

    if (server_manager == NULL || command == NULL)
        return_error("server_manager or command is NULL", ERROR_POINTER);
    return get_file(server_manager->file_manager, command->file_name);
}

static int fill_server_config (manager *server_manager, char *config_path) {
    server_manager->config = init_server_config(config_path);
    if (server_manager->config == ERROR_POINTER)
        return ERROR_CODE;
    if (construct_server_config(server_manager->config) == ERROR_CODE) {
        free(server_manager->config);
        return ERROR_CODE;
    }
    return OK_CODE;
}

static int fill_file_manager (manager *server_manager) {
    /// Функция, инициализирующая file_manager и заполняющая его файлами, для поиска
    /// server_manager - указатель на структуру manager
    /// Возвращаемое значение 0, при ошибке -1

    server_manager->file_manager = init_file_manager(server_manager->config->commands_count);
    if (server_manager->file_manager == ERROR_POINTER)
        return ERROR_CODE;
    if (set_files(server_manager->file_manager, server_manager->config) == ERROR_CODE) {
        free_file_manager(server_manager->file_manager);
        return ERROR_CODE;
    }
    return OK_CODE;
}

static int set_files (shared_hash_table *table, server_config *config) {
    /// Функция, заполняющая file_manager файлами, для поиска
    /// table - hash таблица, описывающая file_manager
    /// config - структура, описывающая конфигурационный файл сервера
    /// Возвращаемое значение 0, -1 при ошибке

    for (unsigned int c = 0; c < config->commands_count; ++c) {
        if (put_file(table, config->commands[c].file_name) == ERROR_CODE) {
            return ERROR_CODE;
        }
    }
    return OK_CODE;
}

static int fill_permissions (manager *server_manager) {
    /// Функция, инициализирующая permission manager
    /// server_manager - указатель на структуру manager
    /// Возвращаемое значение 0, -1 при ошибке

    server_manager->permissions = init_permissions(server_manager->config->commands_count);
    if (server_manager->permissions == ERROR_POINTER)
        return ERROR_CODE;
    if (set_permissions(server_manager->permissions, server_manager->config) == ERROR_CODE) {
        free_permissions(server_manager->permissions);
        return ERROR_CODE;
    }
    return OK_CODE;
}