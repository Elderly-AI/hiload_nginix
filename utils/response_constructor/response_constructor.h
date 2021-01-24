/*
 * Описание функции, которая конструирует ответ сервреа из структуры response_data
 */

#pragma once

#define _GNU_SOURCE
#define MAX_BUF_SIZE 1024 // максимальный размер буффера текстового ответа сервера

#include <stdio.h>
#include <stdlib.h>

#include "../../utils/macros/macros.h"

/*
 * Структура, описывающая параметры ответа сервера
 * code - код ответа
 * content_length - размер содержимого ответа
 * content_type - тип отправляемого файла
 */
typedef struct {
    unsigned int code;
    unsigned int content_length;
    char *content_type;
} response_data;

int construct_response_string (response_data *response, char string[MAX_BUF_SIZE]);
