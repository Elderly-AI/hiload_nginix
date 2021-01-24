/*
 * Описание методов для работы со структурой clone_pool.
 * Clone pool, реализует паттерн проектирования - fork pool.
 * Паттерн, при котором создаётся несколько процессов,
 * которые обращаются к shared_queue, для получения задачи для выполнения,
 * после чего она выполняется, если задач нет, то они ожидаются.
 * Корректное завершение работы clone pool, происходит после получения сигналов SIGINT и SIGHU.
 */

#pragma once

#define _GNU_SOURCE

#include <sched.h>
#include <sys/mman.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/prctl.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

#include "../../utils/macros/macros.h"
#include "../../utils/properties/properties.h"
#include "shared_queue/shared_queue.h"

#define CLONE_POOL_SIZE (MAX_PROCESSES_COUNT - 1) // Кооличество процессов, выполнябщих задачи,
                                                  // зависит от общего числа процессов сервера.
#define CLONE_STACK_SIZE (1024 * 1024) // размер стека каждого процесса, выполняющего задачи.

/*
 * Структура, описывающая параметры clone_pool
 * mutex - мьютекс
 * mutex_attr - настройки мьютекса
 * task_cond - состояние задачи
 * tasking_cond - сосотояние выполнения задач
 * cond_attr - настойки состояний
 * tasking_counter - счётчик колличества выполняемых задач
 * clone_counter - счётчик колличества процессов, выполняющих задачи
 * stop - остановленн ли сервер true - да, false - нет
 */
typedef struct {
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutex_attr;

    pthread_cond_t task_cond;
    pthread_cond_t tasking_cond;
    pthread_condattr_t cond_attr;

    unsigned int tasking_counter;
    unsigned int clone_counter;
    bool stop;
} clone_pool_property;

/*
 * Структра, описывающая clone pool
 * property - параметры clone pool
 * queue - указатель на структуру shared_queue
 */
typedef struct {
    clone_pool_property property;
    shared_queue *queue;
} clone_pool;

clone_pool *init_clone_pool ();
int free_clone_pool (clone_pool *pool);

int put_clone_pool_task (clone_pool *pool, shared_queue_task task);
shared_queue_task *get_clone_pool_task (clone_pool *pool);

int clone_pool_wait(clone_pool *pool);
