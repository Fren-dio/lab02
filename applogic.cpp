#include "applogic.h"

FuncReturningValue* getDataFromFile(const char *filename);
FuncReturningValue* solve(char ***data, size_t fields_num);
void clean(FuncArgument* args);

// Определение точки входа
FuncReturningValue* entryPoint(FuncType ft, FuncArgument* fa)
{
    FuncReturningValue* result;
    switch(ft)
    {
        case getData:
            result = getDataFromFile(fa->filename);
            break;
        case calculateData:
            result = solve(fa->data, fa->fields_num);
            break;
        case cleanData:
            clean(fa);
        default:
            result = NULL;
            break;
    }

    return result;
}


FuncReturningValue* getDataFromFile(const char* filename)
{
    //Инициализируем структуру
    FuncReturningValue *frv = (FuncReturningValue *)malloc(sizeof(FuncReturningValue));

    //Открываем файловый поток
    FILE* fp = fopen(filename, "r");
    size_t lines;
    size_t fields;
    char **rawData = readfile(fp, &lines);
    char ***data = (char***)malloc((lines - 1) * sizeof(char**));
    for (size_t i = 0; i < lines - 1; i++)
    {
        data[i] = strSplit(rawData[i+1], &fields, ',');
    }
    lines--;

    char **headers = strSplit(rawData[0], &fields, ',');

    clean2DArray(rawData, lines);

    //Заполняем струкутру
    frv->len = lines;
    frv->fields_num = fields;
    frv->headers = headers;
    frv->data = data;

    return frv;
}

FuncReturningValue* solve(char ***data, size_t fields_num)
{
    /*
        Какие-то вычисления...

        ... но мы просто вернем первую строку
    */
    FuncReturningValue *frv = (FuncReturningValue *)malloc(sizeof(FuncReturningValue));

    frv->solution = data[0];
    frv->fields_num = fields_num;

    return frv;
}

void clean(FuncArgument* args)
{
    if (args->data != NULL)
    {
        clean3DArray(args->data, args->len, args->fields_num);
    }
    if (args->filename != NULL)
    {
        free(args->filename);
    }
    if (args->headers != NULL)
    {
        clean2DArray(args->headers, args->fields_num);
    }
    if (args->solution != NULL)
    {
        clean2DArray(args->solution, args->fields_num);
    }
}


