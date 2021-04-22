#ifndef APPLOGIC_H
#define APPLOGIC_H

#include "filemanager.h"
#include "memory.h"
#include "stringworks.h"

// Перечисление доступных функций, выделенных в сегрегированный файл логики
enum FuncType
{
    getData,                                    // функция чтения данных из csv файла
    calculateData,                              // функция для вычислений
    cleanData                                   // освобождение памяти
};

/*
 Структура, описывающая возможные варианты передаваемых аргументов для функций (один из вариантов
реализации передачи различных аргументов в точку входа). При необходимости вызова некоторой функции,
соответствующее поле структуры инициализируется некоторым значением.
 */
typedef struct
{
    char* filename;                             // имя файла (функция getData())
    char ***data;                                     // данные (функция calculateData())
    char **headers;                                   // заголовки таблицы (функция getData())
    char **solution;                                  // результат вычислений (для освобождения памяти)
    size_t len;                                       // число строк в таблице
    size_t fields_num;                                // число столбцов в таблице
} FuncArgument;

/*
 Структура, описывающая возможные варианты возвращемого значения функций <...>
 */
typedef struct
{
    char ***data;                                     // данные (функция getData())
    char **headers;                                   // заголовки таблицы (функция getData())
    char **solution;                                  // результат вычислений (функция calculateData())
    size_t len;                                       // число строк в таблице
    size_t fields_num;                                // число столбцов в таблице
} FuncReturningValue;

// Объявление точки входа
FuncReturningValue* entryPoint(FuncType ft, FuncArgument* fa);

#endif // APPLOGIC_H
