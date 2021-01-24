/// Описание методов, работающих с структурой сервера

#include "server.h"

volatile sig_atomic_t stop = 0; // Остановленн ли сервер 1 - да, 0 - нет

static int set_server_property (clone_server *server);
static int configure_socket_address (struct sockaddr_in *server_address);
static int create_socket ();
static int bind_and_listen_socket (int socket_fd, struct sockaddr_in serv_addr);

static void *start_accepting_clients (void *args);
static int start_handle_clients (clone_server *server);

static void register_sigterm_handler ();
static void sigterm_handler ();

clone_server *init_clone_server (char *config_path) {
    /// Функция инициализации сервера
    /// config_path - стрка, расположения конфигурационного файла
    /// Возвращает указатель на clone_server, при ошибке NULL

    register_sigterm_handler(); // установка обработчика системного прирывания
    clone_server *server = malloc(sizeof(clone_server));
    if (server == ERROR_POINTER)
        return_error("malloc error in server", ERROR_POINTER);
    if (set_server_property(server) != OK_CODE) {
        free(server);
        return ERROR_POINTER;
    }
    if ((server->request_manager = init_manager(config_path)) == ERROR_POINTER) {
        free(server);
        return ERROR_POINTER;
    }
    if ((server->pool = init_clone_pool(server->pool)) == ERROR_POINTER) {
        free(server);
        return ERROR_POINTER;
    }
    return server;
}

int free_clone_server (clone_server *server) {
    /// Функция деинициализации сервера
    /// server - указатель на стркуктуру clone_server
    /// Возвращает 0, при ошибке -1

    int result = OK_CODE;
    if (server == ERROR_POINTER)
        return_error("Server is NULL in free_clone_server", ERROR_CODE);
    if (clone_pool_wait(server->pool) == ERROR_CODE)
        result = ERROR_CODE;
    if (free_clone_pool(server->pool) == ERROR_CODE)
        result = ERROR_CODE;
    if (free_manager(server->request_manager) == ERROR_CODE)
        result = ERROR_CODE;
    if (close(server->socket_fd) == ERROR_CODE)
        continue_error("Close socket error in close(server->socket_fd)", ERROR_CODE);
    if (close(server->epoll_fd) == ERROR_CODE)
        continue_error("Close socket error in close(server->epoll_fd)", ERROR_CODE);
    free(server);
    return result;
}

int start_server (clone_server *server) {
    /// Функция старта сервера
    /// server - указатель на стркуктуру clone_server
    /// Возвращает 0, при ошибке -1

    int result = OK_CODE;
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, start_accepting_clients, server) != OK_CODE)
        return_error("pthread_create error in start_server", ERROR_CODE);
    if (pthread_detach(thread_id) != OK_CODE)
        continue_error("pthread_detach error in start_server", ERROR_CODE);
    if (start_handle_clients(server) != OK_CODE)
        continue_error("server hard stopping", ERROR_CODE);
    return result;
}

static int set_server_property (clone_server *server) {
    /// Функция инициализации параметров сервера
    /// server - указатель на стркуктуру clone_server
    /// Возвращает 0, при ошибке -1

    configure_socket_address(&server->server_address);
    if ((server->socket_fd = create_socket()) == ERROR_CODE)
        return_error("create_socket error in server->socket_fd", ERROR_CODE);
    if (bind_and_listen_socket(server->socket_fd, server->server_address) == ERROR_CODE)
        return_error("bind_and_listen_socket error in server->socket_fd", ERROR_CODE);
    if ((server->epoll_fd = epoll_create(1)) == ERROR_CODE)
        return_error("epoll_create error in server->epoll_fd", ERROR_CODE);
    return OK_CODE;
}

static int configure_socket_address (struct sockaddr_in *server_address) {
    /// Функция конфигурирубщая адресс сокета сервера, для приёма клиентов
    /// server_address - структура типа sockaddr_in
    /// Возвращает 0, при ошибке -1

    if (server_address == ERROR_POINTER)
        return_error("NULL pointer in configure_socket_address", ERROR_CODE);
    server_address->sin_family = AF_INET;
    server_address->sin_addr.s_addr = INADDR_ANY;
    server_address->sin_port = htons(SERVER_PORT);
    return OK_CODE;
}

static int create_socket () {
    /// Функция создающая сокет, при приёма клиентов
    /// Возвращает файловый дескриптор сокета, при ошибке -1

    int socket_fd;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR_CODE)
        return_error("socket error in socket_fd", ERROR_CODE);
    if (setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == ERROR_CODE)
        return_error("setsockopt error in socket_fd", ERROR_CODE);
    return socket_fd;
}

static int bind_and_listen_socket (int socket_fd, struct sockaddr_in serv_addr) {
    /// Функция, связывающая сокет с портом, и начинающая его прослушивание
    /// socket_fd - файловый дескриптор сокета
    /// serv_addr - структура типа sockaddr_in
    /// Возвращает 0, при ошибке -1

    if (bind(socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == ERROR_CODE) {
        close(socket_fd);
        return_error("Socket bind error in socket_fd", ERROR_CODE);
    }
    if (listen(socket_fd, MAX_CONNECTIONS_COUNT) == ERROR_CODE) {
        close(socket_fd);
        return_error("Socket listen error in socket_fd", ERROR_CODE);
    }
    return OK_CODE;
}

static void *start_accepting_clients (void *args) {
    /// Функция, которая принимает новых клиентов, после чего отправляет их в epoll
    /// args - указатель на структуру сервера
    /// Ничего не возвращает

    clone_server *server = args;
    int client_socket = -1;
    struct epoll_event event;
    struct sockaddr_storage incoming_address;
    socklen_t address_size = sizeof(incoming_address);

    while (!stop) {
        if ((client_socket = accept(server->socket_fd, (struct sockaddr *) &incoming_address, &address_size)) == ERROR_CODE)
            perror("accept error in client_socket");

        if (client_socket != ERROR_CODE) {
            event.data.fd = client_socket;
            event.events = EPOLLIN;
            if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, client_socket, &event) == ERROR_CODE)
                perror("epoll_ctl error");
        }
    }
    pthread_exit(0);
}

static int start_handle_clients (clone_server *server) {
    /// Функция, обрабатывает готовых клиентов из epoll,
    /// после чего формирует задачу и отправляет её в clone pool
    /// server - указатель на структуру сервера
    /// Возвращает 0, при ошибке -1

    printf("server started ...\n");
    struct epoll_event event_list[MAX_EVENTS_COUNT];
    while (!stop) {
        int ready = epoll_wait(server->epoll_fd, event_list, MAX_EVENTS_COUNT, -1);
        if (ready == ERROR_CODE && !stop) return ERROR_CODE;
        for (int i = 0; i < ready; i++) {
            shared_queue_task task;
            task.func = &handler;
            task.request_manager = server->request_manager;
            task.fd = event_list[i].data.fd;
            if (put_clone_pool_task(server->pool, task) != OK_CODE) {
                close(event_list[i].data.fd);
            }
            epoll_ctl(server->epoll_fd, EPOLL_CTL_DEL, event_list[i].data.fd, NULL);
        }
    }
    errno = OK_CODE;
    return OK_CODE;
}

static void register_sigterm_handler () {
    /// Функция, регистрирующая обработчик системных сигналов SIGINT и SIGHUP

    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = sigterm_handler;
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGHUP, &action, NULL);
}

static void sigterm_handler (int signum) {
    /// Обработчик системных сигналов SIGINT и SIGHUP, останавливающий сервер

    stop = true;
}
