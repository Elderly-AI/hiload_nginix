/*
 * Описание функции, которая конструирует ответ сервреа из структуры response_data
 */

#include "response_constructor.h"

int construct_response_string (response_data *response, char string[MAX_BUF_SIZE]) {
    /// Функция, конструирующая ответ сервреа из структуры response_data
    /// response - структура из которой конструируется ответ
    /// string - буффер ответа
    /// Возвращает 0, при ошибке -1

    int size = sprintf(string, "HTTP/1.1 %d \r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n",
            response->code, response->content_type, response->content_length);
    if (size == ERROR_CODE)
        return ERROR_CODE;
    return OK_CODE;
}
