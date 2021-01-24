/*
 * Описание методов, для работы с config_parser
 * Config parser, производит разбор конфигурационного файла сервера на набор команд
 */

#pragma once

#define _GNU_SOURCE
#define LINE_MAX_SIZE 1024 // максималный размер буффера строки для конфигурационного файла

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../../../utils/macros/macros.h"
#include "../../../utils/utilities/utilities.h"
#include "../files/files.h"

/*
 * Структура, описывабщая разобранную команду из конфигурационного фала
 * line - копия строки конфигурационного файла
 * request_path - строка пути запроса
 * request_type - строка типа запроса
 * file_name - строка имени файла
 * file_type - строка типа фалйа
 * request_path_type - строка, содержащая тип и путь запроса
 */
typedef struct {
    char *line;
    char *request_path;
    char *request_type;
    char *file_name;
    char *file_type;
    char *request_path_type;
} config_command;

/*
 * Структура, config parser
 * commands - указатель на массив команд
 * commands_count - колличесво команд
 * file_name - стрка пути до конфигурационного файла
 * config_file - сам конфигурационный файл
 */
typedef struct {
    config_command *commands;
    unsigned int commands_count;
    char *file_name;
    FILE *config_file;
} server_config;

server_config *init_server_config (char *file_name);
int free_server_config (server_config *config);
int construct_server_config (server_config *config);
