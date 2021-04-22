#include "filemanager.h"

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
