/*
 * Описание методов file manager
 * Производит поиск и загрузку файлов в shared memory
 */

#pragma once

#define _GNU_SOURCE

#define BASE_HASH_TABLE_EXPEND 4

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../shared_hash_table/shared_hash_table.h"

/*
 * Структура, описывающая файл в разделяемой памяти
 * file_size - размер файла в bit
 * file - указатель на область разделяемой памяти, где хранится файл
 */
typedef struct {
    unsigned int file_size;
    void *file;
} file;

shared_hash_table *init_file_manager (unsigned int max_files_count);
int free_file_manager (shared_hash_table *table);
file *get_file (shared_hash_table *table, char *file_name);
int put_file (shared_hash_table *table, char *file_name);

