/*
 * Описание часто используемых функций работы со строками
 */

#pragma once

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../macros/macros.h"

char *get_next_word (char *string);
char *step_word (char *string);
char *step_spacing (char *string);
char *construct_request_type_path (char *request_path, char *request_type);
