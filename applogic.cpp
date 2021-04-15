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

void clean2DArray(char **arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        free(arr[i]);
    }
    free(arr);
}

void clean3DArray(char ***arr, size_t sizeX, size_t sizeY)
{
    for (size_t i = 0; i < sizeX; i++)
    {
        clean2DArray(arr[i], sizeY);
    }
    free(arr);
}

// Функция чтения строк из файла
char** readfile(FILE* fp, size_t *lines)
{
    char line[BUFF_SIZE];
    size_t llen;
    size_t counter = 0;
    size_t max_size = 1;
    char **data = (char **)calloc(max_size, sizeof(char *));
    if (data == NULL)
    {
        exit(1);
    }

    while (fgets(line,120,fp))
    {
        if (counter >= max_size-1)
        {
            data = (char **)realloc(data,max_size * 2 * sizeof(char *));
            if (data == NULL)
            {
                exit(1);
            }
            max_size *= 2;
        }
        llen = strlen(line);
        data[counter] = (char *)calloc(sizeof(char), llen+1);
        strcpy(data[counter], line);
        counter++;
    }

    *lines = counter;

    return data;
}


//Функция разбития строки на массив слов
char** strSplit(char* a_str, size_t *len, const char a_delim)
{
    char** result = 0;
    size_t count = 0;
    char* tmp = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    *len = count + 1;

    count += last_comma < (a_str + strlen(a_str) - 1);
    count++;
    result = (char**)malloc(sizeof(char*) * count);
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        *(result + idx) = 0;
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


