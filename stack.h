#include <stdbool.h>
#include <pthread.h>

struct stack {
    size_t size;
    size_t first_index;
    int fd;
    void **array;
};

struct stack *create_stack(size_t size, int fd);
void free_stack(struct stack *st);
static void expand_stack(struct stack *st);
void push_s(struct stack *st, void *pointer);
void *pull_s(struct stack *st);