/*
 * Файл main запускающий сервер.
 *
 * Функция init_clone_server - инициализирует сервер.
 * Принимаемый аргумент - строка, путь до конфигурационного файла сервера.
 * Возвращаемое значение - указатель на структуру, описывающую сервер, при ошибке NULL.
 *
 * Функция start_server - запускает сервер.
 * Принимаемый аргумент - указатель на структуру сервера.
 * Возвращаемое значение - 0 при корректном завершении работы сервера, -1 при ошибке.
 * Сервер корректно завершает свою работу при получении сигналов SIGINT и SIGHUB.
 *
 * Функция free_clone_server - деинициализирует объект сервера, освобождая выделенные ресурсы.
 * Принимаемый аргумент - указатель на структуру сервера.
 * Возвращаемое значение - 0 при корректной деинициализации объекта сервера, -1 при ошибке.
 */

#include <stdio.h>
#include "server/server.h"

int main() {
    clone_server *server = init_clone_server("server.config");
    if (server == ERROR_POINTER)
        return_error("init_clone_server in main exit", ERROR_CODE);
    printf("starting server ...\n");
    if (start_server(server) == ERROR_CODE)
        return_error("start_server error in main", ERROR_CODE);
    printf("\nstopping server ...\n");
    if (free_clone_server(server) == ERROR_CODE)
        return_error("free_clone_server in main exit", ERROR_CODE);
}
