#include "add_model.h"
#include "ui_add_model.h"
#include<QMessageBox>

add_model::add_model(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_model)
{
    ui->setupUi(this);

    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint);

    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^[0-9 | .]*$"), this);
    ui->lineEdit_2->setValidator(validator);
    ui->lineEdit_3->setValidator(validator);
    ui->lineEdit_4->setValidator(validator);
    ui->lineEdit_5->setValidator(validator);
    ui->lineEdit_6->setValidator(validator);
}

add_model::~add_model()
{
    delete ui;
}

void add_model::on_ok_pushButton_clicked()
{

    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()||ui->lineEdit_4->text().isEmpty()||
            ui->lineEdit_5->text().isEmpty()||ui->lineEdit_6->text().isEmpty())
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("参数不能为空，请重新输入！"),NULL);
        return ;
    }


    QString model_ = ui->lineEdit->text();
    float wingLength_ = ui->lineEdit_2->text().toFloat();
    float engine_Inner_ = ui->lineEdit_3->text().toFloat();
    float engine_Outer_ = ui->lineEdit_4->text().toFloat();
    float plane_length_ = ui->lineEdit_5->text().toFloat();
    float nose_height_ = ui->lineEdit_6->text().toFloat();

    emit addPlane_signal(model_,wingLength_,engine_Inner_,engine_Outer_,plane_length_,nose_height_);

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();

    this->hide();


}

void add_model::on_cancel_pushButton_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();

    emit cancel_signal();
    this->hide();

}


