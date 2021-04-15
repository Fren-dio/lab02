#include "applogic.h"

FuncReturningValue* getDataFromFile(const std::string filename);
FuncReturningValue* solve(std::vector<std::vector<std::string>*> *data);
void clean(std::vector<std::string> *headers, std::vector<std::vector<std::string>*> *data);

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
            result = solve(fa->data);
            break;
        default:
            result = NULL;
            break;
    }

    return result;
}

std::vector<std::string>* readDataRow(std::ifstream &fs)
{
    std::string headersRow;
    std::getline(fs, headersRow);
    std::vector<std::string> *fields = new std::vector<std::string>();
    fields->push_back("");
    size_t i = 0;
    for (char c : headersRow) {
        if (c == ',')
        {
            fields->push_back("");
            i++;
        }
        else
        {
            (*fields)[i].push_back(c);
        }
    }

    return fields;
}


std::vector<std::vector<std::string>*>* readData(std::ifstream &fs)
{
    std::vector<std::vector<std::string>*> *table = new std::vector<std::vector<std::string>*>();
    std::string row;
    while (!fs.eof())
    {
        if (fs.bad() || fs.fail())
        {
            break;
        }
        std::vector<std::string> *fields = readDataRow(fs);
        table->push_back(fields);
    }
    return table;
}

FuncReturningValue* getDataFromFile(const std::string filename)
{
    //Инициализируем структуру
    FuncReturningValue *frv = (FuncReturningValue *)malloc(sizeof(FuncReturningValue));

    //Открываем файловый поток
    std::ifstream file(filename);

    //Заполняем струкутру
    std::vector<std::string> *headers = readDataRow(file);                //заголовки таблицы
    std::vector<std::vector<std::string>*> *data = readData(file);         //основные данные

    frv->headers = headers;
    frv->data = data;

    return frv;
}

FuncReturningValue* solve(std::vector<std::vector<std::string>*> *data)
{
    /*
     Какие-то вычисления...

     ... но мы просто вернем первую строку
     */
    std::vector<std::string> *dummyResult = data->front();

    FuncReturningValue* frv = (FuncReturningValue *)malloc(sizeof(FuncReturningValue));
    frv->solution = dummyResult;
    return frv;
}

void clean(std::vector<std::string> *headers, std::vector<std::vector<std::string>*> *data)
{
    delete headers;
    for (std::vector<std::string>* vector : *data)
    {
        delete vector;
    }
    data->clear();
    delete data;
}


