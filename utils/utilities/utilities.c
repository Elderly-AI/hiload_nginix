/*
 * Описание часто используемых функций работы со строками
 */

#include "utilities.h"

char *get_next_word (char *string) {
    /// Функция возвращающая указатель на начало следующего слова в строке
    /// string - строка в которой ищется слово
    /// Возвращаемое значение - указатель на начало слова

    string = step_word(string);
    return step_spacing(string);
}

char *step_word (char *string) {
    /// Функция пропускающая слово в строке
    /// string - строка в которой пропускается слово
    /// Возвращаемое значение - указатель на символ после слова

    while (*string != '\0' && *string != ' ' && *string != '\n' && *string != '\r' && *string != '\t') {string++;}
    if (*string == '\0') return string;
    *string = '\0';
    return ++string;
}

char *step_spacing (char *string) {
    /// Функция пропускающая набор пробелов, символов табуляции и др.
    /// string - строка в которой пропускается слово
    /// Возвращаемое значение - на первый символ слова

    while (*string == ' ' || *string == '\n' || *string == '\r' || *string == '\t') {string++;}
    return string;
}

char *construct_request_type_path (char *request_path, char *request_type) {
    /// Функция оъеденняющая две строки через пробел
    /// request_path - первая строка (путь запроса)
    /// request_type - вторая строка (тип запроса)
    /// Возвращаемое значение - указатель на полученную строку, при ошибке NULL

    if (request_path == NULL || request_type == NULL)
        return_error("request_path or request_type is NULL", ERROR_POINTER);
    char *request_type_path;
    if (asprintf(&request_type_path, "%s %s", request_type, request_path) == ERROR_CODE)
        return_error("asprintf error in construct_request_type_path", ERROR_POINTER);
    return request_type_path;
}