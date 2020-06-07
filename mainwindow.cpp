#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextCodec>
#include <QWidget>
#include <QTime>
#include "QStandardItemModel"
#include "QStandardItem"

QStandardItemModel *model = new QStandardItemModel;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);//textEdit в режими readonly
    ui->answer->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//Функция поиска длины строки
int len(QString str)
{
    return (str).size();
}


/*EXIT*/
void MainWindow::on_close_clicked()
{
    exit(0);
}

/*CLEAR*/
void MainWindow::on_clear_clicked()
{
    ui->haystack->clear();
    ui->needle->clear();
    ui->textEdit->clear();
    ui->answer->clear();
    model->clear();


}

/*GO*/
void MainWindow::on_go_clicked()
{
    QTime timer;//посчитаем время выполнения программы
    timer.start();

    ui->textEdit->clear();
    ui->answer->clear();
    model->clear();
    int bm_bc[256],//таблица смещений
            i=0, j=0,
            index=0,//искомый индекс вхождения
    a=0,
    move=0,//изначально сдвиг равен 0
    num_move=0,//количество сдвигов
    k=0;//количество совпавших символов

    //Считываем  строку - haystack
    QString haystack;
    haystack=ui->haystack->text();
    int haystack_len=len(haystack);//Длина строки

    int mas_move[haystack_len];//массив сдвигов

    //Считываем  шаблон - needle
    QString needle;
    needle=ui->needle->text();
    int needle_len=len(needle);//Длина шаблона

    //Проверяем, введены ли данные
    if(haystack_len==0) //Строка пустая
        ui->answer->insertPlainText("\nInput Haystack!!!\n");
    else if(needle_len==0)//Шаблон пустой
        ui->answer->insertPlainText("\nInput Needle!!!\n");

    else if(haystack_len<needle_len) //Строка меньше шаблона
        ui->answer->insertPlainText("\nHaystack is less than the Needle\n");

    else if(haystack_len>=needle_len)//то, что нужно
    {

        /* Таблица смещений */
        for ( i=0; i < 256; i++ ) bm_bc[ i ] = needle_len;//изначально всем символам присваиваем величину смещения = needle_len(длине шаблона)
        for ( j=0; j < needle_len-1; j++ )//просматриваем наш шаблон
        {
            i=needle[j].unicode();//Находим код j-того символа шаблона
            bm_bc[ i ] = needle_len-j-1 ;//заносим величину смещения в таблицу
        }

        /* Поиск*/
        index=0;//изначально индекс вхождения равен 0
        i=needle_len-1;
        a=0;
        move=0,//изначально сдвиг равен 0
        num_move=0,//количество сдвигов
        k=0;//количество совпавших символов
        j=0;
        while (move<=haystack_len-needle_len) //пока сдвиг не превышает возможный
        {
           for(i=0;i<needle_len;i++)//цикл по длине шаблона
               if (needle[needle_len-i-1]==haystack[move+needle_len-i-1])//если символы совпали
               {
                   k=k+1;//увеличиваем количество совпавших символов
               }
               else
               {
                   break;
               }
           if (k==needle_len) //если все символы шаблона совпали
           {

               index=move+1; //index = величине сдвига +1
               break;
           }

           else
           {
               k=0;//Обнуляем количество совпавших символов
               a=haystack[move+needle_len-1].unicode();
               move+=bm_bc[a];
               mas_move[j]=move;//заполняем наш массив сдвигов
               num_move+=1;
               j+=1;
           }
        }
        timer.elapsed();//закончили считывать время выполнения программы

        if(ui->checkBox->isChecked())//Если режим определения времени
            ui->answer->insertPlainText("\nTime = " +QString::number(timer.msec())+ " ms");
        else
        {
/*Вывод таблицы на экран в алфавитном порядке */
            for(i=0;i<256;i++)//проходим весь массив bm_bc
            if(bm_bc[i]!=needle_len) //если смещение не равно needle_len
                ui->textEdit->insertPlainText(QString(i) + "  -      " + QString::number(bm_bc[i]) + "\n" ); //Выводим его на экран
            ui->textEdit->insertPlainText("Other- " + QString::number(needle_len) + "\n" ); //Для остальных символов смещение равно needle_len

/*Вывод поиска пошагово*/
            QStandardItem *item;

            //Заголовки столбцов
            QStringList horizontalHeader;
            for(i=1;i<=haystack_len;i++) horizontalHeader.append(QString::number(i));

            //Заголовки строк
                QStringList verticalHeader;
                verticalHeader.append("Haystack");
                verticalHeader.append("Needle");
                for(i=0;i<num_move;i++)//num_move-количество ненулевых элементов в массиве сдвигов mas_move[]
                    verticalHeader.append("Move-"+QString::number(i+1));

            model->setHorizontalHeaderLabels(horizontalHeader);
            model->setVerticalHeaderLabels(verticalHeader);

            //Haystack
            for(i=0;i<haystack_len;i++)
            {
                item = new QStandardItem(QString(haystack[i]));
                model->setItem(0, i, item);
            }

            //Needle
            for(i=0;i<needle_len;i++)
            {
                item = new QStandardItem(QString(needle[i]));
                model->setItem(1, i, item);
            }

            //Move
            for(i=2;i<num_move+2;i++)//num_move-количество ненулевых элементов в массиве сдвигов mas_move[]
                for(j=0;j<needle_len;j++)
                {
                    item=new QStandardItem(QString(needle[j]));
                    model->setItem(i,j+mas_move[i-2],item);
                }
                ui->tableView->setModel(model);

                ui->tableView->resizeRowsToContents();
                ui->tableView->resizeColumnsToContents();



/*Вывод ответа*/
            if (index==0)
                ui->answer->insertPlainText("\nNeedle not in haystack\n");//index=0 - Шаблон не входит в строку
            else
                ui->answer->insertPlainText("\nIndex = " +QString::number(index));//Выводим на экран индекс вхождения шаблона
       }
    }



}



void MainWindow::on_checkBox_clicked()
{

}

void MainWindow::on_checkBox_clicked(bool checked)
{

}
