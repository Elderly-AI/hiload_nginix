/*
 * Файл, описывающий методы для работы с permission manager
 */

#pragma once

#define BASE_HASH_TABLE_EXPEND 4 // базовый коэффициент,

#include <stdio.h>
#include <stdlib.h>

#include "../shared_hash_table/shared_hash_table.h"
#include "../config_parser/config_parser.h"

shared_hash_table *init_permissions (unsigned int max_permissions_count);
int free_permissions (shared_hash_table *table);
int set_permissions (shared_hash_table *table, server_config *config);
config_command *check_permission (shared_hash_table *table, char *permission);

