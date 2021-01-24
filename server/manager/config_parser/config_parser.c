/*
 * Описание методов, для работы с config_parser
 * Config parser, производит разбор конфигурационного файла сервера на набор команд
 */

#include "config_parser.h"

static server_config *allocate_server_config (char *config_file_name);
static int set_server_config_property (server_config *config, char *config_file_name);
static int open_config_file (server_config *config);

static int allocate_config_commands (server_config *config);
static int parse_config_commands (server_config *config);
static int trim_config_commands (server_config *config, unsigned int lines_count);
static int expand_config_commands (server_config *config, unsigned int lines_count);

static int destruct_config_command_line (config_command *command);
static void free_config_commands (server_config *config);

static char *allocate_config_command_line (server_config *config);
static char *trim_config_command_line (server_config *config, char *line);

server_config *init_server_config (char *config_file_name) {
    /// Функция, инициализирующая server_config
    /// config_file_name - строка, описывабщая путь до конфигурационного файла
    /// Возвращаемое значение - указатель на структуру server_config, NULL при ошибке

    server_config *config = allocate_server_config(config_file_name);
    if (config == ERROR_POINTER) return ERROR_POINTER;
    if (set_server_config_property(config, config_file_name) == ERROR_CODE) {
        free(config);
        return ERROR_POINTER;
    }
    if (open_config_file(config) == ERROR_CODE) {
        free(config);
        return ERROR_POINTER;
    }
    return config;
}

int free_server_config (server_config *config) {
    /// Функция, деинициализирующая server_config
    /// config - указатель на структуру server_config
    /// Возвращаемое значение 0, при ошибке -1

    int result = OK_CODE;
    if (fclose(config->config_file) != OK_CODE)
        continue_error("config file close error", ERROR_CODE);
    free_config_commands(config);
    free(config);
    return result;
}

int construct_server_config (server_config *config) {
    /// Функция, производящая разбор конфигурационного файла сервера
    /// config - указатель на структуру server_config
    /// Возвращаемое значение 0, при ошибке -1

    if (allocate_config_commands(config) == ERROR_CODE) return ERROR_CODE;
    if (parse_config_commands(config) == ERROR_CODE) return ERROR_CODE;
    return OK_CODE;
}

static server_config *allocate_server_config (char *config_file_name) {
    /// Функция, аллоцирующая server_config
    /// config_file_name - строка, описывабщая путь до конфигурационного файла
    /// Возвращаемое значение указатель на структуру server_config, при ошибке NULL

    if (config_file_name == NULL)
      return_error("file_name is NULL", ERROR_POINTER);

    server_config *config = malloc(sizeof(server_config));
    if (config == ERROR_POINTER)
        return_error("malloc config error", ERROR_POINTER);
    return config;
}

static int set_server_config_property (server_config *config, char *config_file_name) {
    /// Функция, инициализирующая параметры server_config
    /// config - указатель на структуру server_config
    /// config_file_name - строка, описывабщая путь до конфигурационного файла
    /// Возвращаемое значение 0, при ошибке -1

    if (config_file_name == NULL)
        return_error("file_name is NULL", ERROR_CODE);
    config->file_name = config_file_name;
    config->commands_count = 0;
    return OK_CODE;
}

static int open_config_file (server_config *config) {
    /// Функция, открывающая конфигурационный файл
    /// config - указатель на структуру server_config
    /// Возвращаемое значение 0, при ошибке -1

    config->config_file = fopen(config->file_name, "r");
    if (config->config_file == ERROR_POINTER)
        return_error("cant open config file", ERROR_CODE);
    return OK_CODE;
}

static int allocate_config_commands (server_config *config) {
    /// Функция, аллоцирующая команды для server_config
    /// config - указатель на структуру server_config
    /// Возвращаемое значение 0, при ошибке -1

    config->commands = malloc(sizeof(config_command));
    if (config->commands == ERROR_POINTER) {
        free_config_commands(config);
        return_error("malloc config->commands_error", ERROR_CODE);
    }
    config->commands_count = 1;
    return OK_CODE;
}

static void free_config_commands (server_config *config) {
    /// Функция, деаллоцирующая команды для server_config
    /// config - указатель на структуру server_config

    for (unsigned int c = 0; c < config->commands_count; ++c) {
        if (config->commands[c].line != NULL) free(config->commands[c].line);
        if (config->commands[c].request_path_type != NULL) free(config->commands[c].request_path_type);
    }
    free(config->commands);
    config->commands_count = 0;
}

static int parse_config_commands (server_config *config) {
    /// Функция, производящая разбор конфигурационного фалйа на набор команд
    /// config - указатель на структуру server_config
    /// Возвращаемое значение 0, при ошибке -1

    char *line = allocate_config_command_line(config);
    if (line == NULL) return ERROR_CODE;

    unsigned int lines_count = 0;
    while (fgets(line, LINE_MAX_SIZE, config->config_file) != NULL) {
        if (expand_config_commands(config, lines_count) == ERROR_CODE)
            return ERROR_CODE;
        if ((config->commands[lines_count].line = trim_config_command_line(config, line)) == ERROR_POINTER)
            return ERROR_CODE;
        if (destruct_config_command_line(&config->commands[lines_count]) == ERROR_CODE)
            return ERROR_CODE;
        if ((config->commands[lines_count].request_path_type =
                construct_request_type_path(config->commands[lines_count].request_path,
                                            config->commands[lines_count].request_type))
                                            == ERROR_POINTER) return ERROR_CODE;
        line = allocate_config_command_line(config);
        if (line == NULL) return ERROR_CODE;
        lines_count++;
    }
    if (trim_config_commands(config, lines_count) == ERROR_CODE)
        return ERROR_CODE;
    return OK_CODE;
}

static int trim_config_commands (server_config *config, unsigned int lines_count) {
    /// Функция, уменьшающая колличество config_command, под заданное колличество, не используемая память освобождается
    /// config - указатель на структуру server_config
    /// lines_count - колличество команд в конфигурационном файле
    /// Возвращаемое значение 0, при ошибке -1

    config_command *trimmed_commands = realloc(config->commands, lines_count * sizeof(config_command));
    if (trimmed_commands == NULL) {
        free_config_commands(config);
        return_error("realloc error in trim_config_commands", ERROR_CODE);
    }
    config->commands = trimmed_commands;
    config->commands_count = lines_count;
    return OK_CODE;
}

static char *allocate_config_command_line (server_config *config) {
    /// Функция, аллоцирующая память под строку конфигурационнго фала
    /// config - указатель на структуру server_config
    /// Возвращаемое значение указатель на начало строки, при ошибке NULL

    char *line = malloc(LINE_MAX_SIZE);
    if (line == NULL) {
        free_config_commands(config);
        return_error("malloc config line error", ERROR_POINTER);
    }
    return line;
}

static char *trim_config_command_line (server_config *config, char *line) {
    /// Функция, обрезающая строку под её реальный размер, не используемая часть строки освобождается
    /// config - указатель на структуру server_config
    /// line - указатель на строку
    /// Возвращаемое значение указатель на начало строки, при ошибке NULL

    if (line == NULL) {
        free_config_commands(config);
        return_error("line is NULL", ERROR_POINTER);
    }
    char *trimmed_line = realloc(line, strlen(line));
    if (trimmed_line == NULL) {
        free_config_commands(config);
        return_error("realloc line error in trim_config_command_line", ERROR_POINTER);
    }
    return trimmed_line;
}

static int expand_config_commands (server_config *config, unsigned int lines_count) {
    /// Функция, увелицивающая колличество команд в server_config вдвое
    /// config - указатель на структуру server_config
    /// lines_count - количество команд в server_config
    /// Возвращаемое значение 0, при ошибке -1

    if (lines_count != config->commands_count) return OK_CODE;
    config_command *expanded_config_commands = realloc(config->commands, 2 * config->commands_count * sizeof(config_command));
    if (expanded_config_commands == ERROR_POINTER) {
        free_config_commands(config);
        return_error("error realloc config commands in expand_config_commands", ERROR_CODE);
    }
    config->commands = expanded_config_commands;
    config->commands_count *= 2;
    return OK_CODE;
}

static int destruct_config_command_line (config_command *command) {
    /// Функция, производящая разбор строки конфигурационного файла
    /// command - указатель на структруру config_command, для хранения результата разобранной строки
    /// Возвращаемое значение 0, при ошибке -1

    if (command == NULL)
        return_error("config command is NULL in destruct_config_command_line", ERROR_CODE);
    char *string = command->line;
    command->request_type = string;
    string = command->request_path = get_next_word(string);
    string = command->file_name = get_next_word(string);
    string = command->file_type = get_next_word(string);
    get_next_word(string);
    return OK_CODE;
}
