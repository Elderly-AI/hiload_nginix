#include <pthread.h>
#include <stdbool.h>

struct queue {
    size_t size;
    size_t first_index;
    size_t last_index;
    void **array;
    int fd;
};

struct queue *create_queue(size_t size, int fd);
void free_queue(struct queue *q);
bool q_empty(struct queue *q);
static void expand_queue(struct queue *q);
void push_q(struct queue *q, void *ptr);
void *pull_q(struct queue *q);