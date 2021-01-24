/*
 * Используемые макросы и определения в программе
 */

#pragma once

#define NULL ((void *)0)
#define OK_CODE (0)         // Код корректного завершения функции, возвращающей число
#define ERROR_CODE (-1)     // Код некорректного завершения функции, возвращающей число
#define ERROR_POINTER NULL  // Код некорректного завершения функции, возвращающей указатель

// макрос выводящий ошибку и возвращяющий значение
#define return_error(msg, value) { perror(msg); return(value); } while (0)

// макрос выводящий ошибку и устанавливающий занчение переменной result
#define continue_error(msg, value) { perror(msg); result = value; } while (0)
