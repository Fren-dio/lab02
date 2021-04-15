#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

char* QstringToCharArray(QString qstr)
{
    char *str = (char*)malloc(sizeof(char)*(qstr.size() + 1));
    size_t i;
    for (i = 0; i < qstr.size(); i++)
    {
        str[i] = qstr.at(i).unicode();
    }
    str[i] = 0;
    return str;
}

//Преобразование массива строк в формат QStringList
QStringList ConvertRowToQTFormat(char **row, size_t size)
{
    QStringList qsl = {};

    for(size_t i = 0; i < size; i++)
    {
        qsl.append(QString::fromUtf8(row[i]));
    }

    return qsl;
}

//Отображение данных в виджете таблицы
void MainWindow::showData(FuncReturningValue* frv)
{
    ui->tableWidget->setColumnCount(frv->fields_num);
    QStringList QColumns = ConvertRowToQTFormat(frv->headers, frv->fields_num);
    ui->tableWidget->setHorizontalHeaderLabels(QColumns);
    if (frv->data != NULL)
    {
        ui->tableWidget->setRowCount(0);
        for (size_t i = 0; i < frv->len; i++)
        {
            QStringList currentRow = ConvertRowToQTFormat(frv->data[i], frv->fields_num);
            ui->tableWidget->setRowCount(i + 1);
            for (int j = 0; j < currentRow.count(); j++)
            {
                QTableWidgetItem *item = new QTableWidgetItem();
                item->setData(Qt::EditRole, currentRow.at(j).toDouble());
                item->setText(currentRow.at(j));
                ui->tableWidget->setItem(i, j, item);
            }
        }
    }
}

char*** MainWindow::getDataFromTable()
{
    char ***data = (char ***)malloc(sizeof(char**) * ui->tableWidget->rowCount());
    for (size_t i = 0; i < (size_t)ui->tableWidget->rowCount(); i++)
    {
        data[i] = (char **)malloc(sizeof(char*) * ui->tableWidget->columnCount());
        for (size_t j = 0; j < (size_t)ui->tableWidget->columnCount(); j++)
        {
            //Получаем значение в i-ой строке и j-ом столбце
            QTableWidgetItem *item = ui->tableWidget->item(i,j);\
            //Приводим значение ячейки к стандартному типу строки
            char* str = QstringToCharArray(item->text());
            data[i][j] = str;
        }
    }
    return data;
}


//Эта функция вызывается при нажатии на кнопку "Открыть таблицу"
void MainWindow::on_OpenButton_clicked()
{
    //Открытие диалогового окна (Допускаем к открытию только csv файлы)
    QString pathFileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C://", "CSV File (*.csv)");
    //Вывод полного имени файла на экран
    ui->FilePathLabel->setText(pathFileName);
    if (pathFileName.isEmpty())
        return;
    else
    {

        FuncArgument fa = {
            .filename = QstringToCharArray(pathFileName) //Приводим строку с путем к файлу от типа QString к стандартному типу строки
        };
        FuncReturningValue* frv = entryPoint(getData, &fa);
        //Выводим на экран данные
        showData(frv);

        FuncArgument fa2 = {
            .filename = fa.filename,
            .data = frv->data,
            .headers = frv->headers,
            .len = frv->len,
            .fields_num = frv->fields_num
        };
        entryPoint(cleanData, &fa2);
        free(frv);
    }

}

//Эта функция вызывается при нажатии на кнопку "Посчитать"
void MainWindow::on_CalculateButton_clicked()
{
    FuncArgument fa = {
        .data = getDataFromTable(),
        .len = (size_t)ui->tableWidget->rowCount(),
        .fields_num = (size_t)ui->tableWidget->columnCount()
    };
    FuncReturningValue* frv = entryPoint(calculateData, &fa);

    QString result = "";
    for (size_t i = 0; i < frv->fields_num; i++)
    {
        result += QString::fromUtf8(frv->solution[i]);
        if (i != frv->fields_num - 1)
        {
            result += ", ";
        }
    }

    //Вывод результата на экран
    ui->ResultLabel->setText(result);

    entryPoint(cleanData, &fa);
    free(frv);
}
