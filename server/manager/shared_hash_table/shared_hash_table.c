/*
 * Файл, описывающий методы для работы с shared hash table
 * Shared hash table - структура данных,
 * описывающая hash таблицу фиксированного размера,
 * расположенную в shared memory
 */

#include "shared_hash_table.h"

static void set_shared_hash_table_property (shared_hash_table *table, unsigned int max_size);
static unsigned int get_hash (char *string, unsigned int max_size);

shared_hash_table *init_shared_hash_table (unsigned int max_size) {
    /// Функция, инициализирующая shared hash table
    /// max_size - максимальный размер shared hash table
    /// Возвращает указатель на структуру, описывающую shared_hash_table, при ошибке NULL

    shared_hash_table *table = malloc(sizeof(shared_hash_table));
    if (table == NULL)
        return_error("malloc hash table error", ERROR_POINTER);
    table->array = mmap(NULL, max_size * sizeof(shared_hash_table_node), PROT_READ | PROT_WRITE,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (table->array == MAP_FAILED) {
        free(table);
        return_error("mmap failed in init_hash_table", ERROR_POINTER);
    }
    set_shared_hash_table_property(table, max_size);
    return table;
}

int free_shared_hash_table (shared_hash_table *table) {
    /// Функция, деинициализирующая shared hash table
    /// table - указатель на структуру, описывающую shared_hash_table
    /// Возвращает 0, при ошибке -1

    int result = OK_CODE;
    if (table == NULL)
        return_error("shared hash table is NULL", ERROR_CODE);
    if (munmap(table->array, table->size * sizeof(shared_hash_table_node)) != OK_CODE)
        continue_error("munmap error in free_shared_hash_table", ERROR_CODE);
    free(table);
    return result;
}

void **put_hash_table_value (shared_hash_table *table, char *key) {
    /// Функция, помещающая занчение в shared hash table
    /// table - указатель на структуру, описывающую shared_hash_table
    /// key - стока-ключ по которому кладётся значение
    /// Возвращает указатель на указатель для размещения данных

    unsigned int hash = get_hash(key, table->size);
    unsigned int step = 1;
    while (table->array[hash].key != NULL) {
        hash = (hash + step) % table->size;
        step *= 2;
    }
    table->array[hash].key = key;
    return &table->array[hash].value;
}

void **get_hash_table_value (shared_hash_table *table, char *key) {
    /// Функция, получающая занчение из shared hash table по ключю
    /// table - указатель на структуру, описывающую shared_hash_table
    /// key - стока-ключ по которому получается значение
    /// Возвращает указатель на указатель размещения данных

    unsigned int hash = get_hash(key, table->size);
    unsigned int step = 1;

    while (table->array[hash].key != NULL && strcmp(key, table->array[hash].key) != OK_CODE) {
        hash = (hash + step) % table->size;
        step *= 2;
    }
    if (table->array[hash].key == NULL)
        return ERROR_POINTER;
    return &table->array[hash].value;
}

static void set_shared_hash_table_property (shared_hash_table *table, unsigned int max_size) {
    /// Функция, устанавливающая параметры shared hash table
    /// table - указатель на структуру, описывающую shared_hash_table
    /// max_size - максимаьный размер hash таблицы

    table->size = max_size;
    for (unsigned int c = 0; c < max_size; ++c) {
        table->array[c].key = NULL;
    }
}

static unsigned int get_hash (char *string, unsigned int max_size) {
    /// Функция, возвращающая нормированный hash по строке
    /// string - строка по которой берётся значение hash
    /// max_size - максимаьный размер hash таблицы
    /// Возвращает hash строки

    unsigned long int hash = HASH_SEED;
    int c = (int)*string;
    while (c) {
        hash = ((hash << 5) + hash) + c;
        c = (int)*string++;
    }
    return hash % max_size;
}
