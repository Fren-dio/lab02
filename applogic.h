#ifndef APPLOGIC_H
#define APPLOGIC_H

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

// ыПеречисление доступных функций, выделенных в сегрегированный файл логики
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
    const std::string filename;                       // имя файла (функция getData())
    std::vector<std::vector<std::string>*> *data;     // данные (функция calculateData())
    std::vector<std::string> *headers;                // заголовки таблицы (функция getData())
} FuncArgument;

/*
 Структура, описывающая возможные варианты возвращемого значения функций <...>
 */
typedef struct
{
    std::vector<std::vector<std::string>*> *data;     // данные (функция getData())
    std::vector<std::string> *headers;                // заголовки таблицы (функция getData())
    std::vector<std::string> *solution;               // результат вычислений (функция calculateData())
} FuncReturningValue;

// Объявление точки входа
FuncReturningValue* entryPoint(FuncType ft, FuncArgument* fa);

#endif // APPLOGIC_H
