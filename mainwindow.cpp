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
    ui->textEdit->setReadOnly(true);//textEdit � ������ readonly
    ui->answer->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//������� ������ ����� ������
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
    QTime timer;//��������� ����� ���������� ���������
    timer.start();

    ui->textEdit->clear();
    ui->answer->clear();
    model->clear();
    int bm_bc[256],//������� ��������
            i=0, j=0,
            index=0,//������� ������ ���������
    a=0,
    move=0,//���������� ����� ����� 0
    num_move=0,//���������� �������
    k=0;//���������� ��������� ��������

    //���������  ������ - haystack
    QString haystack;
    haystack=ui->haystack->text();
    int haystack_len=len(haystack);//����� ������

    int mas_move[haystack_len];//������ �������

    //���������  ������ - needle
    QString needle;
    needle=ui->needle->text();
    int needle_len=len(needle);//����� �������

    //���������, ������� �� ������
    if(haystack_len==0) //������ ������
        ui->answer->insertPlainText("\nInput Haystack!!!\n");
    else if(needle_len==0)//������ ������
        ui->answer->insertPlainText("\nInput Needle!!!\n");

    else if(haystack_len<needle_len) //������ ������ �������
        ui->answer->insertPlainText("\nHaystack is less than the Needle\n");

    else if(haystack_len>=needle_len)//��, ��� �����
    {

        /* ������� �������� */
        for ( i=0; i < 256; i++ ) bm_bc[ i ] = needle_len;//���������� ���� �������� ����������� �������� �������� = needle_len(����� �������)
        for ( j=0; j < needle_len-1; j++ )//������������� ��� ������
        {
            i=needle[j].unicode();//������� ��� j-���� ������� �������
            bm_bc[ i ] = needle_len-j-1 ;//������� �������� �������� � �������
        }

        /* �����*/
        index=0;//���������� ������ ��������� ����� 0
        i=needle_len-1;
        a=0;
        move=0,//���������� ����� ����� 0
        num_move=0,//���������� �������
        k=0;//���������� ��������� ��������
        j=0;
        while (move<=haystack_len-needle_len) //���� ����� �� ��������� ���������
        {
           for(i=0;i<needle_len;i++)//���� �� ����� �������
               if (needle[needle_len-i-1]==haystack[move+needle_len-i-1])//���� ������� �������
               {
                   k=k+1;//����������� ���������� ��������� ��������
               }
               else
               {
                   break;
               }
           if (k==needle_len) //���� ��� ������� ������� �������
           {

               index=move+1; //index = �������� ������ +1
               break;
           }

           else
           {
               k=0;//�������� ���������� ��������� ��������
               a=haystack[move+needle_len-1].unicode();
               move+=bm_bc[a];
               mas_move[j]=move;//��������� ��� ������ �������
               num_move+=1;
               j+=1;
           }
        }
        timer.elapsed();//��������� ��������� ����� ���������� ���������

        if(ui->checkBox->isChecked())//���� ����� ����������� �������
            ui->answer->insertPlainText("\nTime = " +QString::number(timer.msec())+ " ms");
        else
        {
/*����� ������� �� ����� � ���������� ������� */
            for(i=0;i<256;i++)//�������� ���� ������ bm_bc
            if(bm_bc[i]!=needle_len) //���� �������� �� ����� needle_len
                ui->textEdit->insertPlainText(QString(i) + "  -      " + QString::number(bm_bc[i]) + "\n" ); //������� ��� �� �����
            ui->textEdit->insertPlainText("Other- " + QString::number(needle_len) + "\n" ); //��� ��������� �������� �������� ����� needle_len

/*����� ������ ��������*/
            QStandardItem *item;

            //��������� ��������
            QStringList horizontalHeader;
            for(i=1;i<=haystack_len;i++) horizontalHeader.append(QString::number(i));

            //��������� �����
                QStringList verticalHeader;
                verticalHeader.append("Haystack");
                verticalHeader.append("Needle");
                for(i=0;i<num_move;i++)//num_move-���������� ��������� ��������� � ������� ������� mas_move[]
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
            for(i=2;i<num_move+2;i++)//num_move-���������� ��������� ��������� � ������� ������� mas_move[]
                for(j=0;j<needle_len;j++)
                {
                    item=new QStandardItem(QString(needle[j]));
                    model->setItem(i,j+mas_move[i-2],item);
                }
                ui->tableView->setModel(model);

                ui->tableView->resizeRowsToContents();
                ui->tableView->resizeColumnsToContents();



/*����� ������*/
            if (index==0)
                ui->answer->insertPlainText("\nNeedle not in haystack\n");//index=0 - ������ �� ������ � ������
            else
                ui->answer->insertPlainText("\nIndex = " +QString::number(index));//������� �� ����� ������ ��������� �������
       }
    }



}



void MainWindow::on_checkBox_clicked()
{

}

void MainWindow::on_checkBox_clicked(bool checked)
{

}
