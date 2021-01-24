/*
 * Файл, описывающий методы для работы с permission manager
 */

#include "permission.h"

static int put_permission (shared_hash_table *table, config_command *command);

shared_hash_table *init_permissions (unsigned int max_permissions_count) {
    /// Функция производящая инициализацию permission manager
    /// max_permissions_count - максимальное колличество записей в shared hash table
    /// Возвращает указатель на структуру shared_hash_table, при ошибке NULL

    shared_hash_table *table = init_shared_hash_table(BASE_HASH_TABLE_EXPEND * max_permissions_count);
    if (table == ERROR_POINTER)
        return ERROR_POINTER;
    return table;
}

int free_permissions (shared_hash_table *table) {
    /// Функция производящая деинициализацию permission manager
    /// table - указатель на структуру shared_hash_table, которая описывает permission manager
    /// Возвращает 0, при ошибке -1

    return free_shared_hash_table(table);
}

int set_permissions (shared_hash_table *table, server_config *config) {
    // Функция производящая установку прав запросов из server_config
    /// table - указатель на структуру shared_hash_table, которая описывает permission manager
    /// config - указатель на структуру разобранного конфигурационного файла
    /// Возвращает 0, при ошибке -1

    int result = OK_CODE;
    if (config == NULL)
        return ERROR_CODE;
    for (unsigned int c = 0; c < config->commands_count; ++c) {
        if (put_permission(table, &config->commands[c]) == ERROR_CODE)
            continue_error("put_permission error in set_permissions", ERROR_CODE);
    }
    return result;
}

config_command *check_permission (shared_hash_table *table, char *permission) {
    // Функция проверяющая права доступа к запросу
    /// table - указатель на структуру shared_hash_table, которая описывает permission manager
    /// permission - строка, содержащая тип и путь запроса
    /// Возвращает указатель на конкретную команду из server_config, если не найденно, то NULL

    void **value = get_hash_table_value(table, permission);
    if (value == NULL)
        return ERROR_POINTER;
    return *value;
}

static int put_permission (shared_hash_table *table, config_command *command) {
    // Функция проверяющая права доступа к запросу
    /// table - указатель на структуру shared_hash_table, которая описывает permission manager
    /// permission - строка, содержащая тип и путь запроса
    /// Возвращает указатель на конкретную команду из server_config, если не найденно, то NULL

    void **put_command = put_hash_table_value(table, command->request_path_type);
    if (put_command == ERROR_POINTER)
        return ERROR_CODE;
    *put_command = command;
    return OK_CODE;
}
