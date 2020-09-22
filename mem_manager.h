#include <stdbool.h>
#include <pthread.h>

struct shared_mem_manager {
    struct stack *st;
    void *mem;
    int fd;
    size_t mem_block;
    size_t blocks_count;
    pthread_mutex_t mutex;
    pthread_mutexattr_t mutex_attr;
};

struct shared_mem_manager *create_shared_mem_manager(size_t mem_block, size_t blocks_count, int fd);
void free_shared_mem_manager(struct shared_mem_manager *sh_mm);
static void expand_mem(struct shared_mem_manager *sh_mm);
void *get_mem(struct shared_mem_manager *sh_mm);
void free_mem(struct shared_mem_manager *sh_mm, void *mem);

