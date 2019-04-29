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
    ui->lineEdit_7->setValidator(validator);
    ui->lineEdit_8->setValidator(validator);
    ui->lineEdit_9->setValidator(validator);
    ui->lineEdit_10->setValidator(validator);
    ui->lineEdit_11->setValidator(validator);
    ui->lineEdit_12->setValidator(validator);
    ui->lineEdit_13->setValidator(validator);
    ui->lineEdit_14->setValidator(validator);
    ui->lineEdit_15->setValidator(validator);
    ui->lineEdit_16->setValidator(validator);
    ui->lineEdit_17->setValidator(validator);
    ui->lineEdit_18->setValidator(validator);
    ui->lineEdit_19->setValidator(validator);
    ui->lineEdit_20->setValidator(validator);
    ui->lineEdit_21->setValidator(validator);
    ui->lineEdit_22->setValidator(validator);
    ui->lineEdit_23->setValidator(validator);
    ui->lineEdit_24->setValidator(validator);
    ui->lineEdit_25->setValidator(validator);
    ui->lineEdit_26->setValidator(validator);
    ui->lineEdit_27->setValidator(validator);
    ui->lineEdit_28->setValidator(validator);
    ui->lineEdit_29->setValidator(validator);
    ui->lineEdit_30->setValidator(validator);

}

add_model::~add_model()
{
    delete ui;
}

void add_model::on_ok_pushButton_clicked()
{

    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()||ui->lineEdit_3->text().isEmpty()||ui->lineEdit_4->text().isEmpty()||
            ui->lineEdit_5->text().isEmpty()||ui->lineEdit_6->text().isEmpty()||ui->lineEdit_7->text().isEmpty()||ui->lineEdit_8->text().isEmpty()||ui->lineEdit_9->text().isEmpty()||
            ui->lineEdit_10->text().isEmpty()||ui->lineEdit_11->text().isEmpty()||ui->lineEdit_12->text().isEmpty()||ui->lineEdit_13->text().isEmpty()||ui->lineEdit_14->text().isEmpty()||
            ui->lineEdit_15->text().isEmpty()||ui->lineEdit_16->text().isEmpty()||ui->lineEdit_17->text().isEmpty()||ui->lineEdit_18->text().isEmpty()||ui->lineEdit_19->text().isEmpty()||
            ui->lineEdit_20->text().isEmpty()||ui->lineEdit_21->text().isEmpty()||ui->lineEdit_22->text().isEmpty()||ui->lineEdit_23->text().isEmpty()||ui->lineEdit_24->text().isEmpty()||
            ui->lineEdit_25->text().isEmpty()||ui->lineEdit_26->text().isEmpty()||ui->lineEdit_27->text().isEmpty()||ui->lineEdit_28->text().isEmpty()||ui->lineEdit_29->text().isEmpty()||
            ui->lineEdit_30->text().isEmpty())
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("参数不能为空，请重新输入！"),NULL);
        return ;
    }


    QStringList parameterList;
    parameterList.append(ui->lineEdit->text());
    parameterList.append(ui->lineEdit_2->text());
    parameterList.append(ui->lineEdit_3->text());
    parameterList.append(ui->lineEdit_4->text());
    parameterList.append(ui->lineEdit_5->text());
    parameterList.append(ui->lineEdit_6->text());
    parameterList.append(ui->lineEdit_7->text());
    parameterList.append(ui->lineEdit_8->text());
    parameterList.append(ui->lineEdit_9->text());
    parameterList.append(ui->lineEdit_10->text());
    parameterList.append(ui->lineEdit_11->text());
    parameterList.append(ui->lineEdit_12->text());
    parameterList.append(ui->lineEdit_13->text());
    parameterList.append(ui->lineEdit_14->text());
    parameterList.append(ui->lineEdit_15->text());
    parameterList.append(ui->lineEdit_16->text());
    parameterList.append(ui->lineEdit_17->text());
    parameterList.append(ui->lineEdit_18->text());
    parameterList.append(ui->lineEdit_19->text());
    parameterList.append(ui->lineEdit_20->text());
    parameterList.append(ui->lineEdit_21->text());
    parameterList.append(ui->lineEdit_22->text());
    parameterList.append(ui->lineEdit_23->text());
    parameterList.append(ui->lineEdit_24->text());
    parameterList.append(ui->lineEdit_25->text());
    parameterList.append(ui->lineEdit_26->text());
    parameterList.append(ui->lineEdit_27->text());
    parameterList.append(ui->lineEdit_28->text());
    parameterList.append(ui->lineEdit_29->text());
    parameterList.append(ui->lineEdit_30->text());


    emit addPlane_signal(parameterList);

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->lineEdit_10->clear();
    ui->lineEdit_11->clear();
    ui->lineEdit_12->clear();
    ui->lineEdit_13->clear();
    ui->lineEdit_14->clear();
    ui->lineEdit_15->clear();
    ui->lineEdit_16->clear();
    ui->lineEdit_17->clear();
    ui->lineEdit_18->clear();
    ui->lineEdit_19->clear();
    ui->lineEdit_20->clear();
    ui->lineEdit_21->clear();
    ui->lineEdit_22->clear();
    ui->lineEdit_23->clear();
    ui->lineEdit_24->clear();
    ui->lineEdit_25->clear();
    ui->lineEdit_26->clear();
    ui->lineEdit_27->clear();
    ui->lineEdit_28->clear();
    ui->lineEdit_29->clear();
    ui->lineEdit_30->clear();

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
    ui->lineEdit_7->clear();
    ui->lineEdit_8->clear();
    ui->lineEdit_9->clear();
    ui->lineEdit_10->clear();
    ui->lineEdit_11->clear();
    ui->lineEdit_12->clear();
    ui->lineEdit_13->clear();
    ui->lineEdit_14->clear();
    ui->lineEdit_15->clear();
    ui->lineEdit_16->clear();
    ui->lineEdit_17->clear();
    ui->lineEdit_18->clear();
    ui->lineEdit_19->clear();
    ui->lineEdit_20->clear();
    ui->lineEdit_21->clear();
    ui->lineEdit_22->clear();
    ui->lineEdit_23->clear();
    ui->lineEdit_24->clear();
    ui->lineEdit_25->clear();
    ui->lineEdit_26->clear();
    ui->lineEdit_27->clear();
    ui->lineEdit_28->clear();
    ui->lineEdit_29->clear();
    ui->lineEdit_30->clear();

    emit cancel_signal();
    this->hide();

}


