///Описание функции для обработки входящего клиентского http запроса

#pragma once

#include <stdio.h>
#include <memory.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../../utils/request_parser/request_parser.h"
#include "../../utils/response_constructor/response_constructor.h"
#include "../manager/manager.h"

#define MAX_BUF_SIZE 1024       // Максимальный размер буфера входного запроса
#define SERVER_ERROR 500        // Код ошибки сервера
#define METHOD_NOT_ALLOWED 405  // Код не разрешённого метода
#define NOT_FOUND 404           // Код не найденного файла
#define SUCCESS 200             // Код успешного запроса

void handler(int fd, manager *request_manager);
