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

//Преобразование массива строк в формат QStringList
QStringList convertRowToQTFormat(std::vector<std::string> *row)
{
    QStringList qsl = {};

    for(size_t i = 0; i < row->size(); i++)
    {
        qsl.append(QString::fromStdString(row->at(i)));
    }

    return qsl;
}

//Отображение данных в виджете таблицы
void MainWindow::showData(std::vector<std::string> *headers, std::vector<std::vector<std::string>*> *data)
{
    ui->tableWidget->setColumnCount(headers->size());
    QStringList QColumns = convertRowToQTFormat(headers);
    ui->tableWidget->setHorizontalHeaderLabels(QColumns);
    if (data != nullptr)
    {
        ui->tableWidget->setRowCount(0);
        for (size_t i = 0; i < data->size(); i++)
        {
            QStringList currentRow = convertRowToQTFormat(data->at(i));
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

std::vector<std::vector<std::string>*>* MainWindow::getDataFromTable()
{
    std::vector<std::vector<std::string>*> *table = new std::vector<std::vector<std::string>*>();
    for (size_t i = 0; i < (size_t)ui->tableWidget->rowCount(); i++)
    {
        table->push_back(new std::vector<std::string>());
        for (size_t j = 0; j < (size_t)ui->tableWidget->columnCount(); j++)
        {
            //Получаем значение в i-ой строке и j-ом столбце
            QTableWidgetItem *item = ui->tableWidget->item(i,j);\
            //Приводим значение ячейки к стандартному типу строки
            std::string str = item->text().toLocal8Bit().constData();

            table->at(i)->push_back(str);
        }
    }
    return table;
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
            .filename = pathFileName.toLocal8Bit().constData() //Приводим строку с путем к файлу от типа QString к стандартному типу строки
        };
        FuncReturningValue* frv = entryPoint(getData, &fa);
        //Выводим на экран данные
        showData(frv->headers, frv->data);

        FuncArgument fa2 = {
            .data = frv->data,
            .headers = frv->headers
        };
        entryPoint(cleanData, &fa);
        free(frv);
    }

}

//Эта функция вызывается при нажатии на кнопку "Посчитать"
void MainWindow::on_CalculateButton_clicked()
{
    FuncArgument fa = {
        .data = getDataFromTable()
    };
    FuncReturningValue* frv = entryPoint(calculateData, &fa);

    std::string result = "";
    for (size_t i = 0; i < frv->solution->size(); i++)
    {
        result += frv->solution->at(i);
        if (i != frv->solution->size() - 1)
        {
            result += ", ";
        }
    }

    //Вывод результата на экран
    ui->ResultLabel->setText(QString::fromStdString(result));

    entryPoint(cleanData, &fa);
    free(frv);
}
