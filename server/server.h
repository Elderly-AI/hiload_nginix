/// Описание методов, работающих с структурой сервера

#pragma once

#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <pthread.h>
#include <errno.h>

#include "clone_pool/clone_pool.h"
#include "handlers/handlers.h"
#include "manager/manager.h"
#include "../utils/macros/macros.h"
#include "../utils/properties/properties.h"

#define MAX_EVENTS_COUNT 1024 // максимальное колличество одновременно обрабатываемых events в epoll

/*
 * Структура, описывающая сервер
 * server_address - структура описывающая адресс сервера и др.
 * request_manager - manager для обработки запроса
 * socket_fd - файловый дескриптор сокета сервера
 * epoll_fd - файловый дескриптор epoll
 */
typedef struct {
    struct sockaddr_in server_address;
    manager *request_manager;
    clone_pool *pool;
    int socket_fd;
    int epoll_fd;
} clone_server;

clone_server *init_clone_server (char *config_path);
int free_clone_server (clone_server *server);
int start_server (clone_server *server);
