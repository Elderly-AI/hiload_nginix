/*
 * Файл, описывающий методы для работы с shared hash table
 * Shared hash table - структура данных,
 * описывающая hash таблицу фиксированного размера,
 * расположенную в shared memory
 */

#pragma once

#define _GNU_SOURCE
#define HASH_SEED 5381 // число от которого заваисит распределение hash

#include <stdlib.h>
#include <stdio.h>
#include <sys/mman.h>
#include <string.h>

#include "../../../utils/macros/macros.h"

/*
 * Структура, описывающая ячейку shared hash table
 * key - строка-значение hash table
 * value - указатель на хранящеся значение
 */
typedef struct {
    char *key;
    void *value;
} shared_hash_table_node;

/*
 * Структура, описывающая shared hash table
 * size - колличество ячеек hash таблицы
 * array - массив ячеек shared hash table
 */
typedef struct {
    unsigned int size;
    shared_hash_table_node *array;
} shared_hash_table;

shared_hash_table *init_shared_hash_table (unsigned int max_size);
int free_shared_hash_table (shared_hash_table *table);

void **put_hash_table_value (shared_hash_table *table, char *key);
void **get_hash_table_value (shared_hash_table *table, char *key);
