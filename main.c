#include <stdio.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <unistd.h>
#include <fcntl.h>

#include "tp_pool.h"
#include "mem_manager.h"
#include "stack.h"


int main() {
    void *my_so = mmap(NULL, sizeof(void *), PROT_READ | PROT_WRITE,
                      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    void (*func)(void *) = mmap(NULL, sizeof(void(*)(void *)), PROT_READ | PROT_WRITE,
                              MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    my_so = dlopen("./libhello.so", RTLD_NOW);
    func = dlsym(my_so, "hello");

    int fd = shm_open("/storage", O_CREAT | O_RDWR, 0);
    if(fd == -1) {
        perror("ERROR IN SHM_OPEN");
    }

    struct shared_mem_manager *mem = create_shared_mem_manager(sizeof(int), 5000, 0);
    struct tp_pool *pool = create_tp_pool(10,10,5000);

    printf("OK\n");
    for(int c = 0; c < 5000; ++c) {
        int *ptr = get_mem(mem);
        *ptr = c;
        tp_pool_add_task(pool, func, ptr, mem);
        printf("SIZE %lu\n", mem->st->first_index);
    }

    sleep(5);
    free_tp_pool(pool);
    free_shared_mem_manager(mem);
    shm_unlink("/storage");
    dlclose(my_so);
    munmap(my_so, sizeof(void *));
    munmap(func, sizeof(void(*)(void *)));
}
