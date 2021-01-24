/*
 * Описание методов file manager
 * Производит поиск и загрузку файлов в shared memory
 */

#include "files.h"

static int construct_file (file *loaded_file, char *file_name);

static int init_file_properties (file *loaded_file, int file_fd);
static int free_file (file *load_file);

shared_hash_table *init_file_manager (unsigned int max_files_count) {
    /// Функция инициализации file_manager
    /// max_files_count - максимальное число фалйов
    /// Возвращает указатель на структуру shared_hash_table, описывающую file manager, при ошибке NULL

    shared_hash_table *table = init_shared_hash_table(max_files_count * BASE_HASH_TABLE_EXPEND);
    if (table == ERROR_POINTER)
        return ERROR_POINTER;
    return table;
}

int free_file_manager (shared_hash_table *table) {
    /// Функция деинициализации file_manager
    /// table - указатель на структуру shared_hash_table, описывающую file manager
    /// Возвращает 0, при ошибке -1


    int result = OK_CODE;
    for (unsigned int c = 0; c < table->size; ++c) {
        if (table->array[c].key != NULL) {
            if (free_file(table->array[c].value) == ERROR_CODE)
                result = ERROR_CODE;
        }
    }
    if (free_shared_hash_table(table) == ERROR_CODE)
        result = ERROR_CODE;
    return result;
}

file *get_file (shared_hash_table *table, char *file_name) {
    /// Функция возвращающая указатель на область shared memory, где расположен файл
    /// table - указатель на структуру shared_hash_table, описывающую file manager
    /// file_name - имя файла
    /// Возвращает 0, при ошибке -1

    void **file = get_hash_table_value(table, file_name);
    if (file == NULL)
        return ERROR_POINTER;
    return *file;
}

int put_file (shared_hash_table *table, char *file_name) {
    /// Функция добавляющая файл в file manager
    /// table - указатель на структуру shared_hash_table, описывающую file manager
    /// file_name - имя файла
    /// Возвращает 0, при ошибке -1

    file *loaded_file = malloc(sizeof(file));
    if (loaded_file == NULL)
    return_error("malloc error in load_file", ERROR_CODE);
    if (construct_file(loaded_file, file_name) == ERROR_CODE) {
        free(loaded_file);
        return ERROR_CODE;
    }
    void **pointer = put_hash_table_value(table, file_name);
    if (pointer == ERROR_POINTER) {
        free(loaded_file);
        return ERROR_CODE;
    }
    *pointer = loaded_file;
    return OK_CODE;
}

static int construct_file (file *loaded_file, char *file_name) {
    /// Функция заполняющая структуру file
    /// loaded_file - заполняемая структура
    /// file_name - имя файла
    /// Возвращает 0, при ошибке -1

    int file_fd = open(file_name, O_NONBLOCK | O_RDONLY);
    if (file_fd == ERROR_CODE)
        return_error("open file error in load_file", ERROR_CODE);
    if (init_file_properties(loaded_file, file_fd) == ERROR_CODE)
        return ERROR_CODE;
    if (close(file_fd) == ERROR_CODE)
        return_error("close error in construct_file", ERROR_CODE);
    return OK_CODE;
}

static int init_file_properties (file *loaded_file, int file_fd) {
    /// Функция инициализирующая свойства структуры file
    /// loaded_file - заполняемая структура
    /// file_fd - файловый дескриптор файла
    /// Возвращает 0, при ошибке -1

    struct stat st;
    if (fstat(file_fd, &st) == ERROR_CODE)
        return_error("fstat error in load_file", ERROR_CODE);
    loaded_file->file_size = st.st_size;
    loaded_file->file = mmap(NULL, loaded_file->file_size, PROT_READ,
                             MAP_SHARED, file_fd, 0);
    if (loaded_file->file == MAP_FAILED)
        return_error("MAP_FAILED in load_file", ERROR_CODE);
    return OK_CODE;
}

static int free_file (file *load_file) {
    /// Функция деинициализирующая структуру file
    /// loaded_file - деинициализируемая структура
    /// Возвращает 0, при ошибке -1

    int result = OK_CODE;
    if (munmap(load_file->file, load_file->file_size) == ERROR_CODE)
    continue_error("munmap error in free_file", ERROR_CODE);
    free(load_file);
    return result;
}
