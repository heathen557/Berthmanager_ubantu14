#include "add_detetion.h"
#include "ui_add_detetion.h"
#include<QMessageBox>
#include<QDebug>

add_detetion::add_detetion(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_detetion)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);

    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^[0-9 | .]*$"), this);

//    ui->lineEdit_2->setValidator(validator);
//    ui->lineEdit_3->setValidator(validator);
//    ui->lineEdit_4->setValidator(validator);
//    ui->lineEdit_5->setValidator(validator);
//    ui->lineEdit_6->setValidator(validator);
//    ui->lineEdit_7->setValidator(validator);
//    ui->lineEdit_8->setValidator(validator);
//    ui->lineEdit_9->setValidator(validator);
}

void add_detetion::updateModel_slot(QStringList str)
{

    allModel_list.clear();
    allModel_list = str;

    qDebug()<<QString::fromLocal8Bit("列表中的数据")<<str;

    ui->comboBox->clear();
    ui->comboBox->addItems(str);
}


add_detetion::~add_detetion()
{
    delete ui;
}

void add_detetion::on_ok_pushButton_clicked()
{
    //    QString planeModel = ui->lineEdit->text();
        QString planeModel = ui->comboBox->currentText();
//        QString stop1_x = ui->lineEdit_2->text();
//        QString stop1_y = ui->lineEdit_3->text();
//        QString stop2_x = ui->lineEdit_4->text();
//        QString stop2_y = ui->lineEdit_5->text();
//        QString mid1_x = ui->lineEdit_6->text();
//        QString mid1_y = ui->lineEdit_7->text();
//        QString mid2_x = ui->lineEdit_8->text();
//        QString mid2_y = ui->lineEdit_9->text();

//        if(planeModel.isEmpty() || stop1_x.isEmpty() || stop1_y.isEmpty() || stop2_x.isEmpty() || stop2_y.isEmpty() ||
//                mid1_x.isEmpty() || mid1_y.isEmpty() || mid2_x.isEmpty() || mid2_y.isEmpty())
//        {
//            QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("输入不能为空，请重新输入！"),NULL);
//            return;
//        }

        if(planeModel.isEmpty())
        {
            QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("输入不能为空，请重新输入！"),NULL);
            return;
        }

        emit addDetectionPlane_signal(planeModel);



    //    ui->lineEdit->clear();

        ui->comboBox->setCurrentIndex(0);
//        ui->lineEdit_2->clear();
//        ui->lineEdit_3->clear();
//        ui->lineEdit_4->clear();
//        ui->lineEdit_5->clear();
//        ui->lineEdit_6->clear();
//        ui->lineEdit_7->clear();
//        ui->lineEdit_8->clear();
//        ui->lineEdit_9->clear();

        this->hide();
}

void add_detetion::on_cancel_pushButton_clicked()
{
    //    ui->lineEdit->clear();
        ui->comboBox->setCurrentIndex(0);
//        ui->lineEdit_2->clear();
//        ui->lineEdit_3->clear();
//        ui->lineEdit_4->clear();
//        ui->lineEdit_5->clear();
//        ui->lineEdit_6->clear();
//        ui->lineEdit_7->clear();
//        ui->lineEdit_8->clear();
//        ui->lineEdit_9->clear();

        emit cancel_signal();
        this->hide();
}
