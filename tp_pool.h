#include <pthread.h>
#include <stdbool.h>

struct tp_pool {
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutex_attr;

    pthread_cond_t task_cond;
    pthread_cond_t tasking_cond;
    pthread_condattr_t cond_attr;

    struct queue *task_queue;
    struct shared_mem_manager *manager;

    size_t process_count;
    size_t threads_count;

    size_t tasking_cnt;
    size_t thread_cnt;
    bool stop;
};

struct tp_pool_task {
    struct shared_mem_manager *manager;
    void (*function)(void *args);
    void *args;
};

static struct tp_pool_task *tp_pool_task_create(struct tp_pool *pool,
        void (*function)(void *args), void *args, struct shared_mem_manager *manager);

static void tp_pool_task_free(struct tp_pool *pool, struct tp_pool_task *task);
static struct tp_pool_task *tp_pool_task_get(struct tp_pool *pool);
static void *tp_pool_worker(void *args);
static void create_threads(void *args);
static void create_processes(void *args);
struct tp_pool *create_tp_pool(size_t process_count, size_t threads_count, size_t tasks_queue_size);
void free_tp_pool(struct tp_pool *pool);

bool tp_pool_add_task(struct tp_pool *pool, void (*function)(void *args),
        void *args, struct shared_mem_manager *manager);
