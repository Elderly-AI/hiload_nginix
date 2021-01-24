/*
 * Описание методов для работы со структурой clone_pool.
 * Clone pool, реализует паттерн проектирования - fork pool.
 * Паттерн, при котором создаётся несколько процессов,
 * которые обращаются к shared_queue, для получения задачи для выполнения,
 * после чего она выполняется, если задач нет, то они ожидаются.
 * Корректное завершение работы clone pool, происходит после получения сигналов SIGINT и SIGHU.
 */

#include "clone_pool.h"

static int set_clone_pool_property (clone_pool *pool);
static void define_clone_pool_parameters (clone_pool *pool);
static int set_clone_pool_mutex (clone_pool *pool);
static int set_clone_pool_conditions (clone_pool *pool);

static int create_clones(clone_pool *pool);
static void *alloc_clone_stack ();
static int clone_pool_worker (void *arg);

static int stop_clone_pool (clone_pool *pool);
static void wait_exit_all_clones ();
static int destruct_clone_pool (clone_pool *pool);
static int destruct_clone_pool_mutex (clone_pool *pool);
static int destruct_clone_pool_conditions (clone_pool *pool);

clone_pool *init_clone_pool () {
    /// Функция, инициализирующая clone pool и запускающая его
    /// Возвращает указатель на структуру clone pool, при ошибке NULL

    clone_pool *pool = malloc(sizeof(clone_pool));
    if (pool == ERROR_POINTER) return_error("malloc error in pool", ERROR_POINTER);
    if (set_clone_pool_property(pool) != OK_CODE) return ERROR_POINTER;
    if (create_clones(pool) == ERROR_CODE) return ERROR_POINTER;
    return pool;
}

int free_clone_pool (clone_pool *pool) {
    /// Функция, деинициализирующая clone pool
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    int result = OK_CODE;
    if (stop_clone_pool(pool) != OK_CODE) result = ERROR_CODE;
    if (destruct_clone_pool(pool) != OK_CODE) result = ERROR_CODE;
    return result;
}

int clone_pool_wait(clone_pool *pool) {
    /// Функция, ожидающая полного выполнения всех задач, при этом новые задачи не добавляются
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    if (pool == ERROR_POINTER) return ERROR_CODE;
    pthread_mutex_lock(&(pool->property.mutex));
    while (true) {
        if (!pool->property.stop && pool->property.tasking_counter != 0) {
            pthread_cond_wait(&(pool->property.tasking_cond), &(pool->property.mutex));
        } else { break; }
    }
    pthread_mutex_unlock(&(pool->property.mutex));
    return OK_CODE;
}

int put_clone_pool_task (clone_pool *pool, shared_queue_task task) {
    /// Функция, добавляющая задачу в очередь задач
    /// pool - указатель на структуру clone pool
    /// task - заполненная структура shared_queue_task
    /// Возвращает 0, при ошибке -1

    pthread_mutex_lock(&(pool->property.mutex));
    shared_queue_task *push_task = push_shared_queue_task(pool->queue);
    if (push_task == ERROR_POINTER) {
        pthread_mutex_unlock(&(pool->property.mutex));
        return ERROR_CODE;
    }
    *push_task = task;
    pthread_cond_broadcast(&(pool->property.task_cond));
    pthread_mutex_unlock(&(pool->property.mutex));
    return OK_CODE;
}

shared_queue_task *get_clone_pool_task (clone_pool *pool) {
    /// Функция, получающая задачу из очереди задач
    /// pool - указатель на структуру clone pool
    /// Возвращает указатель на shared_queue_task, если очередь пуста, то NULL

    return pull_shared_queue_task(pool->queue);
}

static int set_clone_pool_property (clone_pool *pool) {
    /// Функция, инициализирующая clone pool property
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    define_clone_pool_parameters(pool);
    if (set_clone_pool_mutex(pool) != OK_CODE) return ERROR_CODE;
    if (set_clone_pool_conditions(pool) != OK_CODE) return ERROR_CODE;
    if ((pool->queue = init_shared_queue()) == ERROR_POINTER) return ERROR_CODE;
    return OK_CODE;
}

static void define_clone_pool_parameters (clone_pool *pool) {
    /// Функция, инициализирующая параметры clone pool
    /// pool - указатель на структуру clone pool

    pool->property.tasking_counter = 0;
    pool->property.clone_counter = 0;
    pool->property.stop = 0;
}

static int set_clone_pool_mutex (clone_pool *pool) {
    /// Функция, инициализирующая mutex, для clone pool
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    if (pthread_mutexattr_init(&pool->property.mutex_attr) != OK_CODE)
        return_error("pthread_mutexattr_init error in mutex_attr", ERROR_CODE);
    if (pthread_mutexattr_setpshared(&pool->property.mutex_attr, PTHREAD_PROCESS_SHARED) != OK_CODE)
        return_error("pthread_mutexattr_setpshared error in mutex_attr", ERROR_CODE);
    if (pthread_mutex_init(&pool->property.mutex, &pool->property.mutex_attr) != OK_CODE)
        return_error("pthread_mutex_init error in mutex", ERROR_CODE);
    return OK_CODE;
}

static int set_clone_pool_conditions (clone_pool *pool) {
    /// Функция, инициализирующая conditions, для clone pool
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    if (pthread_condattr_init(&pool->property.cond_attr) != OK_CODE)
       return_error("pthread_condattr_init error in cond_attr", ERROR_CODE);
    if (pthread_condattr_setpshared(&pool->property.cond_attr, PTHREAD_PROCESS_SHARED) != OK_CODE)
        return_error("pthread_condattr_setpshared error in cond_attr", ERROR_CODE);
    if (pthread_cond_init(&pool->property.task_cond, &pool->property.cond_attr) != OK_CODE)
        return_error("pthread_cond_init error in task_cond", ERROR_CODE);
    if (pthread_cond_init(&pool->property.tasking_cond, &pool->property.cond_attr) != OK_CODE)
        return_error("pthread_cond_init error in tasking_cond", ERROR_CODE);
    return OK_CODE;
}

static int create_clones(clone_pool *pool) {
    /// Функция, создающая процессы для выполнения задач из очереди
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    for (unsigned int c = 0; c < CLONE_POOL_SIZE; ++c) {
        void *stack_top = alloc_clone_stack();
        if (stack_top == ERROR_POINTER) return ERROR_CODE;
        pid_t pid = clone(clone_pool_worker, stack_top, CLONE_VM | CLONE_FILES | SIGCHLD, pool);
        if (pid == ERROR_CODE) return_error("Clone error", ERROR_CODE);
    }
    return OK_CODE;
}

static void *alloc_clone_stack () {
    /// Функция, аллоцирующая стек для процесса clone pool
    /// Возвращает указатель на конец стека, при ошибке NULL

    void *stack = mmap(NULL, CLONE_STACK_SIZE, PROT_READ | PROT_WRITE,
                       MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack == MAP_FAILED) return_error("Mmap clone stack error", ERROR_POINTER);
    return stack + CLONE_STACK_SIZE;
}

static int clone_pool_worker (void *args) {
    /// Функция, вызывающаяся в процессе, который выполняет задачи из очереди
    /// Возвращает 0, при ошибке -1

    prctl(PR_SET_PDEATHSIG, SIGHUP); // Параметр, при котором при завершении процесса родителя потомок тоже завершается
    clone_pool *pool = args;
    shared_queue_task *task_pointer;
    shared_queue_task task;

    while (1) {
        pthread_mutex_lock(&(pool->property.mutex));
        while (shared_queue_is_empty(pool->queue) && !pool->property.stop) { // Ожидание появления задач
            pthread_cond_wait(&(pool->property.task_cond), &(pool->property.mutex));
        }

        if(pool->property.stop) { // Выход из цикла, если сервер остановлен
            break;
        }

        task_pointer = get_clone_pool_task(pool);
        if (task_pointer != ERROR_POINTER) {
            task = *task_pointer; // получение задачи из очереди
        }
        pool->property.tasking_counter++;
        pthread_mutex_unlock(&(pool->property.mutex));

        if (task_pointer != ERROR_POINTER) {
            task.func(task.fd, task.request_manager); // выполенине полученной задачи
        }

        pthread_mutex_lock(&(pool->property.mutex));
        pool->property.tasking_counter--;
        if (!pool->property.stop && pool->property.tasking_counter == 0 && shared_queue_is_empty(pool->queue)) {
            pthread_cond_signal(&(pool->property.tasking_cond)); // если clone pool остановлен, освобождает
                                                                 // tasking_cond, чтобы процессы могли
                                                                 // корректно завершиться
        }
        pthread_mutex_unlock(&(pool->property.mutex));
    }
    pool->property.clone_counter--;
    pthread_cond_signal(&(pool->property.tasking_cond));
    pthread_mutex_unlock(&(pool->property.mutex));
    return OK_CODE;
}

static int stop_clone_pool (clone_pool *pool) {
    /// Функция, останавливающая clone pool
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    pthread_mutex_lock(&(pool->property.mutex));
    pool->property.stop = true;
    while (pull_shared_queue_task(pool->queue) != ERROR_POINTER) {}
    pthread_cond_broadcast(&(pool->property.task_cond));
    pthread_mutex_unlock(&(pool->property.mutex));
    clone_pool_wait(pool);
    wait_exit_all_clones();
    return OK_CODE;
}

static void wait_exit_all_clones () {
    /// Функция, ожидающая заверщения всеп процессов потомков

    pid_t pid;
    int status = 0;
    while ((pid = wait(&status)) > 0);
    errno = OK_CODE;
}

static int destruct_clone_pool (clone_pool *pool) {
    /// Функция, освобождающая память параметров clone_pool
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    int result = OK_CODE;
    if (destruct_clone_pool_mutex(pool) != OK_CODE) result = ERROR_CODE;
    if (destruct_clone_pool_conditions(pool) != OK_CODE) result = ERROR_CODE;
    free(pool);
    return result;
}

static int destruct_clone_pool_mutex (clone_pool *pool) {
    /// Функция, деинициализирующая mutex clone_pool
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    int result = OK_CODE;
    if (pthread_mutexattr_destroy(&pool->property.mutex_attr) != OK_CODE)
        continue_error("pthread_mutex_destroy error in mutex_attr", ERROR_CODE);
    if (pthread_mutex_destroy(&pool->property.mutex) != OK_CODE)
        continue_error("pthread_mutex_destroy error in mutex", ERROR_CODE);
    return result;
}

static int destruct_clone_pool_conditions (clone_pool *pool) {
    /// Функция, деинициализирующая conditions clone_pool
    /// pool - указатель на структуру clone pool
    /// Возвращает 0, при ошибке -1

    int result = OK_CODE;
    if (pthread_condattr_destroy(&pool->property.cond_attr) != OK_CODE)
        continue_error("pthread condattr destroy error in cond_attr", ERROR_CODE);
    if (pthread_cond_destroy(&(pool->property.task_cond)) != OK_CODE)
        continue_error("pthread_cond_destroy error in task_cond", ERROR_CODE);
    if (pthread_cond_destroy(&(pool->property.tasking_cond)) != OK_CODE)
        continue_error("pthread_cond_destroy error in tasking_cond", ERROR_CODE);
    return result;
}
