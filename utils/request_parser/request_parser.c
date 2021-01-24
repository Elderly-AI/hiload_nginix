/*
 * Функция позволяющая получить тип и путь входящего запроса
 */

#include "request_parser.h"

char *get_request_type_path (char *string) {
    /// Функция позволяющая получить тип и путь входящего запроса
    /// string - строка запроса
    /// Возвращает ип и путь входящего запроса

    char *line = string;
    while(line != NULL && *line != ' ' && *line != '\n' && *line != '\r' && *line != '\t') {line++;}
    while(line != NULL && (*line == ' ' || *line == '\n' || *line == '\r' || *line == '\t')) {line++;}
    while(line != NULL && *line != ' ' && *line != '\n' && *line != '\r' && *line != '\t') {line++;}
    if (line != NULL) {
        *line = '\0';
    }
    return string;
}