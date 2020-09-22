#define _GNU_SOURCE

#include <memory.h>
#include <sys/mman.h>
#include <stdbool.h>
#include <unistd.h>

#include "queue.h"

struct queue *create_queue(size_t size, int fd) {

    struct queue *q = mmap(NULL, sizeof(struct queue), PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    ftruncate(fd, size * sizeof(void *));
    q->array = mmap(NULL, size * sizeof(void *), PROT_READ | PROT_WRITE,
                    MAP_SHARED, fd, 0);
    q->size = size;
    q->fd = fd;
    return q;
}

void free_queue(struct queue *q) {
    munmap(q->array, q->size * sizeof(void *));
    munmap(q, sizeof(struct queue));
}

bool q_empty(struct queue *q) {
    return (q->first_index == q->last_index);
}

static void expand_queue(struct queue *q) {
    ftruncate(q->fd, 2 * q->size * sizeof(void *));
    q->array = mremap(q->array, 0, 2 * q->size * sizeof(void *), MREMAP_MAYMOVE);

    if(q->first_index > q->last_index) {
        memcpy(&q->array[q->size], q->array, sizeof(void *) * q->last_index);
        q->last_index += q->size;
    }

    q->size *= 2;
}

void push_q(struct queue *q, void *ptr) {
    if((q->last_index + 1) % q->size == q->first_index) {
        expand_queue(q);
    }

    q->array[q->last_index] = ptr;
    q->last_index = (q->last_index + 1) % q->size;
}

void *pull_q(struct queue *q) {
    void *ptr = NULL;
    if(!q_empty(q)){
        ptr = q->array[q->first_index];
        q->first_index = (q->first_index + 1) % q->size;
    }
    return ptr;
}
