#include "shared_queue.h"

/*
 * Описание методов структуры shared_queue
 * Shared_queue - структура данных, с набором методов,
 * которая реализует циклическую очередь фиксированного размера,
 * расположенную в разделяемой памяти, хранит структуры типа shared_queue_task.
 * Основные настройки shared_queue, находятся в структуре shared_queue_property
 */

static shared_queue *alloc_shared_queue ();
static void set_shared_queue_property (shared_queue *queue);

shared_queue *init_shared_queue () {
    /// Функция, инициализирующая структуру shared_queue
    /// Возвращаемое значение - указатель на shared_queue или NULL, при ошибке.

    shared_queue *queue = alloc_shared_queue ();
    if (queue == NULL) return ERROR_POINTER;
    set_shared_queue_property(queue);
    return queue;
}

void free_shared_queue (shared_queue *queue) {
    /// Функция, деинициализирующая структуру shared_queue
    /// queue - указатель на структуру shared_queue

    if (munmap(queue, SHARED_QUEUE_SIZE) != OK_CODE) perror("munmap error in queue");
}

shared_queue_task *push_shared_queue_task (shared_queue *queue) {
    /// Функция, добавляющая задачу в очередь shared_queue
    /// queue - указатель на структуру shared_queue
    /// возвращаемое значение - указатель на shared_queue_task или NULL, если нет места

    unsigned int head = (queue->property.head + 1) % queue->property.max_size;
    if (head == queue->property.tail) return ERROR_POINTER;
    shared_queue_task *task = &queue->array[queue->property.head];
    queue->property.head = head;
    return task;
}

shared_queue_task *pull_shared_queue_task (shared_queue *queue) {
    /// Функция, позволяющая получить задачу из очереди shared_queue
    /// queue - указатель на структуру shared_queue
    /// возвращаемое значение - указатель на shared_queue_task или NULL, если очередь пуста

    if (queue->property.tail == queue->property.head) return ERROR_POINTER;
    shared_queue_task *task = &queue->array[queue->property.tail];
    queue->property.tail = (queue->property.tail + 1) % queue->property.max_size;
    return task;
}

bool shared_queue_is_empty (shared_queue *queue) {
    /// Функция, позволяющая проверить, пуста ли очередь
    /// queue - указатель на структуру shared_queue
    /// возвращаемое значение - bool, пуста ли очередь или нет

    return queue->property.head == queue->property.tail;
}

static shared_queue *alloc_shared_queue () {
    /// Функция, аллоцирующая структуру shared_queue
    /// возвращаемое значение - указатель на shared_queue или NULL, при ошибке

    shared_queue *queue = mmap(NULL, SHM_MAX_SIZE, PROT_READ | PROT_WRITE,
                               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (queue == MAP_FAILED) return_error("mmap error in queue", ERROR_POINTER);
    return queue;
}

static void set_shared_queue_property (shared_queue *queue) {
    /// Функция, инициализирущая параметры структуры shared_queue
    /// queue - указатель на структуру shared_queue

    queue->property.max_size = (SHARED_QUEUE_SIZE - sizeof(shared_queue_property)) / sizeof(shared_queue_task);
    queue->property.head = 0;
    queue->property.tail = 0;
}
