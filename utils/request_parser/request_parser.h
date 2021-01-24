/*
 * Функция позволяющая получить тип и путь входящего запроса
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

#include "../macros/macros.h"
#include "../utilities/utilities.h"

char *get_request_type_path (char *string);
