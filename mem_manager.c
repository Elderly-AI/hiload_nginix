#define _GNU_SOURCE
#define STACK_ID "j784WxaBLC0kQAmCkAbbNUACNbVMxyOxGUDVcKL8"

#include <sys/mman.h>
#include <stddef.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "stack.h"
#include "mem_manager.h"


struct shared_mem_manager *create_shared_mem_manager(size_t mem_block, size_t blocks_count, int fd) {

    struct shared_mem_manager *sh_mm = mmap(NULL, sizeof(struct shared_mem_manager), PROT_READ | PROT_WRITE,
                                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    ftruncate(fd, mem_block * blocks_count);
    sh_mm->mem = mmap(NULL, mem_block * blocks_count, PROT_READ | PROT_WRITE,
                      MAP_SHARED, fd, 0);

    pthread_mutexattr_init(&sh_mm->mutex_attr);
    pthread_mutexattr_setpshared(&sh_mm->mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&sh_mm->mutex, &sh_mm->mutex_attr);


    int stack_fd = shm_open(STACK_ID, O_CREAT | O_RDWR, 0);
    if(stack_fd == -1) {
        perror("ERROR IN SHM_OPEN");
    }

    sh_mm->st = create_stack(blocks_count, stack_fd);
    for(size_t block = 0; block < blocks_count; ++block) {
        push_s(sh_mm->st, sh_mm->mem + block * mem_block);
    }

    sh_mm->mem_block = mem_block;
    sh_mm->blocks_count = blocks_count;
    sh_mm->fd = fd;

    return sh_mm;
}

void free_shared_mem_manager(struct shared_mem_manager *sh_mm) {
    free_stack(sh_mm->st);
    pthread_mutex_destroy(&sh_mm->mutex);
    pthread_mutexattr_destroy(&sh_mm->mutex_attr);

    munmap(sh_mm->mem, sh_mm->mem_block * sh_mm->blocks_count);

    if(shm_unlink(STACK_ID) == -1) {
        perror("ERROR IN SHM_UNLINK");
    }

    munmap(sh_mm, sizeof(struct shared_mem_manager));
}

static void expand_mem(struct shared_mem_manager *sh_mm) {
    ftruncate(sh_mm->fd, 2 * sh_mm->blocks_count * sh_mm->mem_block);
    sh_mm->mem = mremap(sh_mm->mem, 0, 2 * sh_mm->blocks_count * sh_mm->mem_block, MREMAP_MAYMOVE);

    for(size_t block = sh_mm->blocks_count; block < 2 * sh_mm->blocks_count; ++block) {
        push_s(sh_mm->st, sh_mm->mem + block * sh_mm->mem_block);
    }
    sh_mm->blocks_count *= 2;
}

void *get_mem(struct shared_mem_manager *sh_mm) {
    pthread_mutex_lock(&sh_mm->mutex);
    void *pointer = pull_s(sh_mm->st);
    if (pointer == NULL) {
        expand_mem(sh_mm);
        pointer = pull_s(sh_mm->st);
    }
    pthread_mutex_unlock(&sh_mm->mutex);
    return pointer;
}

void free_mem(struct shared_mem_manager *sh_mm, void *mem) {
    pthread_mutex_lock(&sh_mm->mutex);
    push_s(sh_mm->st, mem);
    pthread_mutex_unlock(&sh_mm->mutex);
}


//int main() {
//
//    int fd = shm_open("/storage", O_CREAT | O_RDWR, 0);
//    if(fd == -1) {
//        perror("ERROR IN SHM_OPEN");
//    }
//    struct shared_mem_manager *manager = create_shared_mem_manager(sizeof(int), 2, fd);
//
//    int *arr[16384];
//
//    for(int c = 0; c < 16384; ++c) {
//        int *mem = get_mem(manager);
//        *mem = c;
//        arr[c] = mem;
//    }
//
//    for(int c = 0; c < 16384; ++c) {
//        printf("%d\n", *arr[c]);
//        free_mem(manager, arr[c]);
//    }
//
//    free_shared_mem_manager(manager);
//    shm_unlink("/storage");
//}