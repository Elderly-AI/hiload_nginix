/*
 * Описание методов структуры shared_queue
 * Shared_queue - структура данных, с набором методов,
 * которая реализует циклическую очередь фиксированного размера,
 * расположенную в разделяемой памяти, хранит структуры типа shared_queue_task.
 * Основные настройки shared_queue, находятся в структуре shared_queue_property
 */

#pragma once

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>

#include "../../../utils/macros/macros.h"
#include "../../../utils/properties/properties.h"
#include "../../manager/manager.h"

#define SHARED_QUEUE_SIZE SHM_MAX_SIZE // максимальный размер очереди в bit

/*
 * Cтруктура, описывающая задачу, для расположения в shared_queue.
 * func - указатель на функцию, для выполнения.
 * manager - указатель на структуру request_manager.
 * fd - файловый дескриптор, клиентского сокета.
 */
typedef struct {
    void (*func)(int fd, manager *request_manager);
    manager *request_manager;
    int fd;
} shared_queue_task;

/*
 * Cтруктура, описывающая параметры циклической очереди фиксированного рамера.
 * head - индекс головы очереди.
 * tail - индекс хвоста очереди.
 * max_size - максимальный размер очереди.
 */
typedef struct {
    unsigned int head;
    unsigned int tail;
    unsigned int max_size;
} shared_queue_property;

/*
 * Cтруктура, описывающая циклическую очередь фиксированного рамера, расположенную в shared_queue.
 * property - структура, описывающая параметры циклической очереди фиксированного рамера.
 * array - массив задач, для размещения в циклической очереди фиксированного рамера
 */
typedef struct {
    shared_queue_property property;
    shared_queue_task array[];
} shared_queue;

shared_queue *init_shared_queue ();
void free_shared_queue (shared_queue *queue);

shared_queue_task *push_shared_queue_task (shared_queue *queue);
shared_queue_task *pull_shared_queue_task (shared_queue *queue);

bool shared_queue_is_empty (shared_queue *queue);
