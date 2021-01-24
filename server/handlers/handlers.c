///Описание функции для обработки входящего клиентского http запроса

#include "handlers.h"

void handler(int fd, manager *request_manager) {
    /// Функция для обработки входящего клиентского http запроса
    /// fd - файловый дескриптор клиентского сокета
    /// request_manager - указатель на структуру manager

    response_data response;
    file *loaded_file = NULL;
    char buf[MAX_BUF_SIZE];
    char response_string[MAX_BUF_SIZE];
    char content_type[] = "text/html;charset=utf-8";

    int byte_count = read(fd, buf, MAX_BUF_SIZE); // чтение запроса в буфер из клиентского сокета
    if (byte_count == -1) { perror("read"); }

    if (get_request_type_path(buf) == ERROR_POINTER) { // получение типа и пути запроса
        response.content_length = 0;
        response.code = SERVER_ERROR;
    }
    else {
        // получение команды, которая соответствует типу и пути запроса
        config_command *command = get_request_command(request_manager, buf);
        if (command == ERROR_POINTER) {
            response.content_length = 0;
            response.code = METHOD_NOT_ALLOWED;
            response.content_type = content_type;
        }
        else {
            // получение файла для отдачи клиену из полученной команды
            loaded_file = get_request_file(request_manager, command);
            if (loaded_file == ERROR_POINTER) {
                response.content_length = 0;
                response.code = NOT_FOUND;
            }
            response.content_length = loaded_file->file_size;
            response.code = SUCCESS;
            response.content_type = command->file_type;
        }
    }

    // построение строки ответа
    if (construct_response_string(&response, response_string) == ERROR_CODE) {
        close(fd);
        return;
    }

    // отправка строки ответа
    if (send(fd, response_string, strlen(response_string), 0) == ERROR_CODE) {
        perror("send error");
        close(fd);
        return;
    }

    // если отправляемый файл найден, он тоже отправляется
    if (loaded_file!= NULL && loaded_file->file != NULL) {
        if (send(fd, loaded_file->file, loaded_file->file_size, 0) == ERROR_CODE) {
            perror("send error");
            close(fd);
            return;
        }
        if (send(fd, "\r\n", 2, 0) == ERROR_CODE) {
            perror("send error");
            close(fd);
            return;
        }
    }

    close(fd); // закрытие клиентского сокета
}
