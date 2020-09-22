#define _GNU_SOURCE
#define QUEUE_ID "JkOuZgS9xliCRtuII66UYOIofYVdsnXA34UByoTu"

#include <memory.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <sys/prctl.h>
#include <fcntl.h>

#include "queue.h"
#include "mem_manager.h"
#include "tp_pool.h"

static struct tp_pool_task *tp_pool_task_create(struct tp_pool *pool,
        void (*function)(void *args), void *args, struct shared_mem_manager *manager) {

    struct tp_pool_task *task = NULL;
    if (function == NULL) {
        return task;
    }

    task = get_mem(pool->manager);
    task->manager = manager;
    task->function = function;
    task->args = args;
    return task;
}

static void tp_pool_task_free(struct tp_pool *pool, struct tp_pool_task *task) {
    if (task == NULL)
        return;

    if(task->manager != NULL) {
        free_mem(task->manager, task->args);
    }
    free_mem(pool->manager, task);
}

static struct tp_pool_task *tp_pool_task_get(struct tp_pool *pool) {
    struct tp_pool_task *task = NULL;
    if (pool != NULL)
        task = pull_q(pool->task_queue);
    return task;
}

static void *tp_pool_worker(void *args) {
    struct tp_pool *pool = args;
    struct tp_pool_task *task;

    while (1) {
        pthread_mutex_lock(&(pool->mutex));

        while (q_empty(pool->task_queue) && !pool->stop)
            pthread_cond_wait(&(pool->task_cond), &(pool->mutex));

        if(pool->stop)
            break;

        task = tp_pool_task_get(pool);
        pool->tasking_cnt++;

        pthread_mutex_unlock(&(pool->mutex));

        if (task != NULL) {
            task->function(task->args);
            tp_pool_task_free(pool, task);
        }

        pthread_mutex_lock(&(pool->mutex));
        pool->tasking_cnt--;
        if (!pool->stop && pool->tasking_cnt == 0 && q_empty(pool->task_queue))
            pthread_cond_signal(&(pool->tasking_cond));
        pthread_mutex_unlock(&(pool->mutex));
    }

    pool->thread_cnt--;
    pthread_cond_signal(&(pool->tasking_cond));
    pthread_mutex_unlock(&(pool->mutex));
    return NULL;
}

static void create_threads(void *args) {
    struct tp_pool *pool = args;

    pthread_t thread_id;
    for(int c = 0; c < pool->threads_count - 1; ++c){
        pthread_create(&thread_id, NULL, tp_pool_worker, pool);
        pthread_detach(thread_id);
    }
    tp_pool_worker(pool);
    pthread_exit(0);
}

static void create_processes(void *args) {
    struct tp_pool *pool= args;

    pid_t pid = fork();
    for(int c = 0; c < (pool->process_count - 1) && pid != 0 && pid != -1; ++c){
        pid = fork();
    }

    switch (pid) {
        case -1:
            perror("FORK CREATE ERROR");
            break;
        case 0:
            if (prctl(PR_SET_PDEATHSIG, SIGHUP) == -1) {
                perror("PRCTL ERROR");
            }
            create_threads(pool);
            break;
        default:
            break;
    }
}

void tp_pool_wait(struct tp_pool *pool) {
    if (pool == NULL)
        return;

    pthread_mutex_lock(&(pool->mutex));
    while (1) {
        if ((!pool->stop && pool->tasking_cnt != 0) || (pool->stop && pool->thread_cnt != 0)) {
            pthread_cond_wait(&(pool->tasking_cond), &(pool->mutex));
        } else {
            break;
        }
    }
    pthread_mutex_unlock(&(pool->mutex));
}

struct tp_pool *create_tp_pool(size_t process_count, size_t threads_count, size_t tasks_queue_size) {
    struct tp_pool *pool = mmap(NULL, sizeof(struct tp_pool), PROT_READ | PROT_WRITE,
                                MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    pthread_mutexattr_init(&pool->mutex_attr);
    pthread_mutexattr_setpshared(&pool->mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&pool->mutex, &pool->mutex_attr);

    pthread_condattr_init(&pool->cond_attr);
    pthread_condattr_setpshared(&pool->cond_attr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&pool->task_cond, &pool->cond_attr);
    pthread_cond_init(&pool->tasking_cond, &pool->cond_attr);

    int fd = shm_open(QUEUE_ID, O_CREAT | O_RDWR, 0);
    if(fd == -1) {
        perror("ERROR IN SHM_OPEN");
    }

    pool->task_queue = create_queue(tasks_queue_size, fd);
    pool->manager = create_shared_mem_manager(sizeof(struct tp_pool_task), tasks_queue_size, 0);
    pool->process_count = process_count;
    pool->threads_count = threads_count;
    pool->thread_cnt = process_count * threads_count;
    pool->tasking_cnt = 0;

    create_processes(pool);
    return pool;
}

void free_tp_pool(struct tp_pool *pool) {
    struct tp_pool_task *task = pull_q(pool->task_queue);
    pthread_mutex_lock(&(pool->mutex));

    while (task != NULL) {
        tp_pool_task_free(pool, task);
        task = pull_q(pool->task_queue);
    }
    pool->stop = true;
    pthread_cond_broadcast(&(pool->task_cond));
    pthread_mutex_unlock(&(pool->mutex));

    tp_pool_wait(pool);

    pthread_mutexattr_destroy(&pool->mutex_attr);
    pthread_condattr_destroy(&pool->cond_attr);

    pthread_cond_destroy(&(pool->task_cond));
    pthread_cond_destroy(&(pool->tasking_cond));
    pthread_mutex_destroy(&pool->mutex);

    free_queue(pool->task_queue);
    shm_unlink(QUEUE_ID);
    munmap(pool, sizeof(struct tp_pool));
}

bool tp_pool_add_task(struct tp_pool *pool, void (*function)(void *args),
        void *args, struct shared_mem_manager *manager) {

    struct tp_pool_task *task;

    if (pool == NULL) {
        return false;
    }

    task = tp_pool_task_create(pool, function, args, manager);
    if (task == NULL) {
        return false;
    }

    pthread_mutex_lock(&(pool->mutex));
    push_q(pool->task_queue, task);

    pthread_cond_broadcast(&(pool->task_cond));
    pthread_mutex_unlock(&(pool->mutex));

    return true;
}
