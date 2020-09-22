#define _GNU_SOURCE

#include <sys/mman.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#include "stack.h"

struct stack *create_stack(size_t size, int fd) {
    struct stack *st = mmap(NULL, sizeof(struct stack), PROT_READ | PROT_WRITE,
                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    ftruncate(fd, size * sizeof(void *));
    st->array = mmap(NULL, size * sizeof(void *), PROT_READ | PROT_WRITE,
                     MAP_SHARED, fd, 0);

    st->size = size;
    st->fd = fd;
    st->first_index = 0;
    return st;
}

void free_stack(struct stack *st) {
    munmap(st->array, st->size * sizeof(void *));
    munmap(st, sizeof(struct stack));
}

static void expand_stack(struct stack *st) {
    ftruncate(st->fd,2 * st->size * sizeof(void *));
    st->array = mremap(st->array, 0, 2 * st->size * sizeof(void *), MREMAP_MAYMOVE);
    st->size *= 2;
}

void push_s(struct stack *st, void *pointer) {
    if(st->first_index == st->size) {
        expand_stack(st);
        printf("expand\n");
    }

    st->array[st->first_index] = pointer;
    st->first_index += 1;
}

void *pull_s(struct stack *st) { //TODO add shrink
    void *element = NULL;
    if(st->first_index != 0){
        st->first_index -= 1;
        element = st->array[st->first_index];
    }
    return element;
}
