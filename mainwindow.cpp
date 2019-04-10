//2019-03-26 GitHub测试版

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<iostream>
#include<qdebug.h>
#include<QMessageBox>
#include<QJsonParseError>
#include<QJsonObject>
#include<QJsonArray>
#include<QValidator>
#include <QRegExpValidator>



using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton->setDisabled(true);
    ui->selDetePoint_pushButton->setDisabled(true);

//    ui->x_horizontalSlider->setValue(-100);
//    ui->y_horizontalSlider->setValue(-100);

    airplaneModel_Alertflag = false;  //控件初始化时不进入修改函数
    Detection_Alertflag = false;      //同上
    click_flag = true;                //初始化允许点击

    currentDetePoints_index = 0;          //curent tableWidgetItem show

//    this->showMaximized();

    connect(this,SIGNAL(xchange_signal(float)),ui->widget,SLOT(xchange_slot(float)));
    connect(this,SIGNAL(ychange_signal(float)),ui->widget,SLOT(ychange_slot(float)));
    connect(this,SIGNAL(lidarStart_signal(QString,QString)),ui->widget,SLOT(lidarStart_slot(QString,QString)));

    connect(ui->widget,SIGNAL(sliderRangechange_signal(float,float,float,float)),this,SLOT(sliderRangechange_slot(float,float,float,float)));
    connect(ui->widget,SIGNAL(initSliderPos_signal(float,float)),this,SLOT(initSliderPos_slot(float,float)));
    connect(&tcpClient,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(&addModel,SIGNAL(addPlane_signal(QString,float,float,float,float,float)),this,SLOT(addPlaneModel_slot(QString,float,float,float,float,float)));
    connect(&addDetetion_diag,SIGNAL(addDetectionPlane_signal(QString)),
            this,SLOT(addDetetion_slot(QString)));
    connect(&addModel,SIGNAL(cancel_signal()),this,SLOT(recvStateModel_slot()));
    connect(&addDetetion_diag,SIGNAL(cancel_signal()),this,SLOT(recvDetetion_slot()));

    connect(this,SIGNAL(updateModel_signal(QStringList)),&addDetetion_diag,SLOT(updateModel_slot(QStringList)));

    connect(&click_timer,SIGNAL(timeout()),this,SLOT(click_timeSlot()));

    connect(&heartBeat_timer,SIGNAL(timeout()),this,SLOT(heartBeat_slot()));
    heartBeat_timer.start(5000);


    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^[0-9 | .]*$"), this);
    ui->lineEdit->setValidator(validator);
    ui->lineEdit_2->setValidator(validator);
    ui->lineEdit_3->setValidator(validator);
    ui->lineEdit_4->setValidator(validator);
    ui->lineEdit_5->setValidator(validator);
    ui->lineEdit_6->setValidator(validator);
    ui->lineEdit_7->setValidator(validator);
    ui->lineEdit_8->setValidator(validator);
    ui->lineEdit_9->setValidator(validator);
    ui->lineEdit_10->setValidator(validator);

    ui->lineEdit_13->setValidator(validator);
    ui->lineEdit_23->setValidator(validator);
    ui->lineEdit_24->setValidator(validator);
    ui->lineEdit_25->setValidator(validator);
    ui->lineEdit_26->setValidator(validator);
    ui->lineEdit_27->setValidator(validator);
    ui->lineEdit_28->setValidator(validator);
    ui->lineEdit_29->setValidator(validator);
    ui->lineEdit_30->setValidator(validator);



    ui->tableWidget->setRowCount(128);
    ui->tableWidget_2->setRowCount(128);

    ui->tableWidget_2->setColumnWidth(1,170);
    ui->tableWidget_2->setColumnWidth(2,170);
    ui->tableWidget_2->setColumnWidth(3,170);
    ui->tableWidget_2->setColumnWidth(4,170);

    for(int i=0; i<128; i++)
    {
        planeModel_Item[i].setTextAlignment(Qt::AlignCenter);
        wingLength_Item[i].setTextAlignment(Qt::AlignCenter);
        engineInner_Item[i].setTextAlignment(Qt::AlignCenter);
        engineOuter_Item[i].setTextAlignment(Qt::AlignCenter);
        planeLength_Item[i].setTextAlignment(Qt::AlignCenter);
        noseHeight_Item[i].setTextAlignment(Qt::AlignCenter);

        planeModel_Item[i].setFlags(Qt::NoItemFlags);

        ui->tableWidget->setItem(i,0,&planeModel_Item[i]);
        ui->tableWidget->setItem(i,1,&wingLength_Item[i]);
        ui->tableWidget->setItem(i,2,&engineInner_Item[i]);
        ui->tableWidget->setItem(i,3,&engineOuter_Item[i]);
        ui->tableWidget->setItem(i,4,&planeLength_Item[i]);
        ui->tableWidget->setItem(i,5,&noseHeight_Item[i]);
        ui->tableWidget->hideRow(i);


        /********************************************************************/

        planeModel_Item2[i].setTextAlignment(Qt::AlignCenter);
        stop1_x_Item[i].setTextAlignment(Qt::AlignCenter);
        stop1_y_Item[i].setTextAlignment(Qt::AlignCenter);
        stop2_x_Item[i].setTextAlignment(Qt::AlignCenter);
        stop2_y_Item[i].setTextAlignment(Qt::AlignCenter);
        center1_x_Item[i].setTextAlignment(Qt::AlignCenter);
        center1_y_Item[i].setTextAlignment(Qt::AlignCenter);
        center2_x_Item[i].setTextAlignment(Qt::AlignCenter);
        center2_y_Item[i].setTextAlignment(Qt::AlignCenter);

        planeModel_Item2[i].setFlags(Qt::NoItemFlags);

        ui->tableWidget_2->setItem(i,0,&planeModel_Item2[i]);
        ui->tableWidget_2->setItem(i,1,&stop1_x_Item[i]);
//        ui->tableWidget_2->setItem(i,2,&stop1_y_Item[i]);
        ui->tableWidget_2->setItem(i,2,&stop2_x_Item[i]);
//        ui->tableWidget_2->setItem(i,4,&stop2_y_Item[i]);
        ui->tableWidget_2->setItem(i,3,&center1_x_Item[i]);
//        ui->tableWidget_2->setItem(i,6,&center1_y_Item[i]);
        ui->tableWidget_2->setItem(i,4,&center2_x_Item[i]);
//        ui->tableWidget_2->setItem(i,8,&center2_y_Item[i]);
        ui->tableWidget_2->hideRow(i);

    }


    //add the detec point tableWidget
    ui->DetePoint_tableWidget->setRowCount(50);
    ui->DetePoint_tableWidget->setColumnWidth(1,200);

    for(int i=0; i<50; i++)
    {
        index_Item[i].setTextAlignment(Qt::AlignCenter);
        DetecPoint_Item[i].setTextAlignment(Qt::AlignCenter);
        index_Item[i].setFlags(Qt::NoItemFlags);
        index_Item[i].setText(QString::number(i+1));


        ui->DetePoint_tableWidget->setItem(i,0,&index_Item[i]);
        ui->DetePoint_tableWidget->setItem(i,1,&DetecPoint_Item[i]);
    }




    if(false == udp_socket.bind(6667))
    {
         QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("UDP端口已经占用!!!"),NULL);
         return;
    }
    connect(&udp_socket,SIGNAL(readyRead()),this,SLOT(receUdpMsg_slot()));


}

void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug()<<"the close event has coming!!!!"<<endl;
    tcpClient.close();
}

void MainWindow::click_timeSlot()
{
    click_flag = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sendMsg(QByteArray array)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::ReadWrite);
    out.setByteOrder(QDataStream::BigEndian);

    out<<array;
//    out.device()->seek(0);
//    out<<quint8(170);
//    out<<quint8(170);
//    out << (quint16)(block.size() - sizeof(quint32));
//    tcpClient.write(block);

    tcpClient.write(array);

    qDebug()<<QString::fromLocal8Bit("要发送的数据为：")<<array;
}

void MainWindow::readMessage()
{
    quint32   mesg_len=0;
    qint64 readbyte=tcpClient.bytesAvailable();
    if(readbyte<=0)
    {
        qDebug()<<QString::fromLocal8Bit("接收到的数据长度为空 reqthread()！")<<endl;
        return;
    }
    QByteArray buffer;
    quint16 flag,len;
    buffer =tcpClient.readAll();
    m_buffer.append(buffer);
    int totallen = m_buffer.size();

    while(totallen)
    {
        qDebug()<<QString::fromLocal8Bit("m_buffer的长度为")<<m_buffer.length()<<"  "<<QString::fromLocal8Bit("m_buffer中数据为：")<<m_buffer<<endl;

        if(totallen < 4)    //不足四个字节（头部）
        {
            qDebug()<<QString::fromLocal8Bit("头部不足四个字节");
            break;
        }

        QDataStream packet(m_buffer);


        packet>>flag>>len;       //获取长度 len  信息头暂时不用

        qDebug()<<QString::fromLocal8Bit("数据部分的长度")<<len  ;

        QByteArray json_Array;
        if(totallen<len)         //说明长度不够，返回
        {
            break;
        }else                    //若是数据部分长度大于 或者等于 指定长度，说明后面可能有冗余数据
        {
            json_Array = m_buffer.mid(4,len);

            if(totallen-4>=len)
            {
                m_buffer.clear();
                totallen = 0 ;
                qDebug()<<QString::fromLocal8Bit("数据接收正好或多了！");
            }
            else
            {
                qDebug()<<QString::fromLocal8Bit("数据接收不足！");
                break;
            }
        }
        qDebug()<<QString::fromLocal8Bit("标记头为：")<<flag<<"len = "<<len <<" all len ="<<json_Array;
        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(json_Array, &jsonError);                     // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {                                                                                            // 解析未发生错误
            if (doucment.isObject()) {
                QJsonObject object = doucment.object();                                              // 转化为对象
                if (object.contains("@table"))
                {                                                                                    // 包含指定的 key
                    QJsonValue val_flag = object.value("@table");                                    // 获取指定 key 对应的 value
                    int flag = val_flag.toInt();
                    qDebug() << "@table " << flag;
                    if(5 == flag)              //机型基本参数返回表
                    {
                      QJsonValue value_msg = object.value("msg");
                      if(value_msg.isArray())
                      {
                          QJsonArray msgArr = value_msg.toArray();
                            int size_ = msgArr.size();

                            AllPlaneModel_list.clear();

                          for(int i=0; i<msgArr.size(); i++)
                          {
                              ui->tableWidget->showRow(i);

                              QJsonObject airModelObject =  msgArr[i].toObject();
                              if (airModelObject.contains("CFTP"))
                              {                                                                   // 包含指定的 key
                                  QJsonValue val_cftp = airModelObject.value("CFTP");             // 获取指定 key 对应的 value
                                  QString str_CFTP = val_cftp.toString();
                                  planeModel_Item[i].setText(str_CFTP);


                                  AllPlaneModel_list.append(str_CFTP);                            //存储所有的机型
                              }
                              if (airModelObject.contains("wing"))
                              {                                                                                      // 包含指定的 key
                                  QJsonValue val_wing = airModelObject.value("wing");                                // 获取指定 key 对应的 value
                                  double wing_ = val_wing.toDouble();
                                  wingLength_Item[i].setText(QString::number(wing_));
                              }
                              if (airModelObject.contains("engineInner"))
                              {                                                                                      // 包含指定的 key
                                  QJsonValue val_engineInner = airModelObject.value("engineInner");                  // 获取指定 key 对应的 value
                                  double engineInner_ = val_engineInner.toDouble();
                                  engineInner_Item[i].setText(QString::number(engineInner_));

                              }
                              if (airModelObject.contains("engineOuter"))
                              {                                                                                      // 包含指定的 key
                                  QJsonValue val_engineOuter = airModelObject.value("engineOuter");                 // 获取指定 key 对应的 value
                                  double engineOuter_ = val_engineOuter.toDouble();
                                  engineOuter_Item[i].setText(QString::number(engineOuter_));

                              }
                              if (airModelObject.contains("length"))
                              {                                                                                      // 包含指定的 key
                                  QJsonValue val_cftp = airModelObject.value("length");                              // 获取指定 key 对应的 value
                                  double length_ = val_cftp.toDouble();
                                  planeLength_Item[i].setText(QString::number(length_));

                              }
                              if (airModelObject.contains("noseheight"))
                              {                                                                                      // 包含指定的 key
                                  QJsonValue val_cftp = airModelObject.value("noseheight");                         // 获取指定 key 对应的 value
                                  double noseHeight_ = val_cftp.toDouble();
                                  noseHeight_Item[i].setText(QString::number(noseHeight_));

                              }

                          }
                      }

                      airplaneModel_Alertflag = true;    //显示完成后，可以被修改了

                    }else if(8 == flag)
                    {
                        QJsonValue value_msg = object.value("msg");
                        if(value_msg.isArray())
                        {

                            QJsonArray msgArr = value_msg.toArray();
                            int size_ = msgArr.size();

                            AllDetectionModel_list.clear();

                            for(int i=0; i<msgArr.size(); i++)
                            {
                                ui->tableWidget_2->showRow(i);

                                QJsonObject airModelObject =  msgArr[i].toObject();
                                if (airModelObject.contains("CFTP"))
                                {                                                                   // 包含指定的 key
                                    QJsonValue val_cftp = airModelObject.value("CFTP");             // 获取指定 key 对应的 value
                                    QString str_CFTP = val_cftp.toString();
                                    planeModel_Item2[i].setText(str_CFTP);
                                    AllDetectionModel_list.append(str_CFTP);
                                }
                                if (airModelObject.contains("stop1x") && airModelObject.contains("stop1y"))
                                {                                                                                      // 包含指定的 key
                                    QJsonValue val_stop1x = airModelObject.value("stop1x");                            // 获取指定 key 对应的 value
                                    double stop1_x = val_stop1x.toDouble();
                                    QJsonValue val_stop1y = airModelObject.value("stop1y");                            // 获取指定 key 对应的 value
                                    double stop1_y = val_stop1y.toDouble();
                                    QString str1_xy = "("+QString::number(stop1_x)+" , "+QString::number(stop1_y)+")";
                                    stop1_x_Item[i].setText(str1_xy);
                                }
                                if (airModelObject.contains("stop1y"))
                                {                                                                                      // 包含指定的 key
                                    QJsonValue val_stop1y = airModelObject.value("stop1y");                           // 获取指定 key 对应的 value
                                    double stop1_y = val_stop1y.toDouble();
                                    stop1_y_Item[i].setText(QString::number(stop1_y));

                                }
                                if (airModelObject.contains("stop2x") && airModelObject.contains("stop2y"))
                                {                                                                                      // 包含指定的 key
                                    QJsonValue val_stop2x = airModelObject.value("stop2x");                 // 获取指定 key 对应的 value
                                    double stop2_x = val_stop2x.toDouble();
                                    QJsonValue val_stop2y = airModelObject.value("stop2y");                              // 获取指定 key 对应的 value
                                    double stop2_y = val_stop2y.toDouble();
                                    QString str2_xy = "("+QString::number(stop2_x)+" , "+QString::number(stop2_y)+")";
                                    stop2_x_Item[i].setText(str2_xy);

                                }
                                if (airModelObject.contains("stop2y"))
                                {                                                                                      // 包含指定的 key
                                    QJsonValue val_stop2y = airModelObject.value("stop2y");                              // 获取指定 key 对应的 value
                                    double stop2_y = val_stop2y.toDouble();
                                    stop2_y_Item[i].setText(QString::number(stop2_y));

                                }
                                if (airModelObject.contains("middle1x") && airModelObject.contains("middle1y"))
                                {                                                                                      // 包含指定的 key
                                    QJsonValue val_middle1x = airModelObject.value("middle1x");                         // 获取指定 key 对应的 value
                                    double middle1_x = val_middle1x.toDouble();
                                    QJsonValue val_middle1y = airModelObject.value("middle1y");                         // 获取指定 key 对应的 value
                                    double middle1_y = val_middle1y.toDouble();
                                    QString mid1_xy = "("+QString::number(middle1_x)+" , "+QString::number(middle1_y)+")";
                                    center1_x_Item[i].setText(mid1_xy);
                                }
                                if (airModelObject.contains("middle1y"))
                                {                                                                                      // 包含指定的 key
                                    QJsonValue val_middle1y = airModelObject.value("middle1y");                         // 获取指定 key 对应的 value
                                    double middle1_y = val_middle1y.toDouble();
                                    center1_y_Item[i].setText(QString::number(middle1_y));
                                }
                                if (airModelObject.contains("middle2x") && airModelObject.contains("middle2y"))
                                {                                                                                      // 包含指定的 key
                                    QJsonValue val_middle2x = airModelObject.value("middle2x");                         // 获取指定 key 对应的 value
                                    double middle2_x = val_middle2x.toDouble();
                                    QJsonValue val_middle2y = airModelObject.value("middle2y");                         // 获取指定 key 对应的 value
                                    double middle2_y = val_middle2y.toDouble();
                                    QString mid2_xy = "("+QString::number(middle2_x)+" , "+QString::number(middle2_y)+")";
                                    center2_x_Item[i].setText(mid2_xy);
                                }
                                if (airModelObject.contains("middle2y"))
                                {                                                                                      // 包含指定的 key
                                    QJsonValue val_middle2y = airModelObject.value("middle2y");                         // 获取指定 key 对应的 value
                                    double middle2_y = val_middle2y.toDouble();
                                    center2_y_Item[i].setText(QString::number(middle2_y));
                                }
                            }

                            Detection_Alertflag = true;
                        }
                    }else if(6 == flag)   //机型添加成功
                    {
                        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("操作成功！"),NULL);
                    }else if(7 == flag)   //机型修改成功
                    {
                        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("操作成功！"),NULL);
                    }else if(9 == flag)   //机型检测参数添加成功
                    {
                        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("添加机型成功！请配置检测参数"),NULL);
                    }else if(10 == flag)   //机型检测参数修改成功
                    {
                        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("操作成功！"),NULL);
                    }else if(11 == flag)    //接收行人的检测参数
                    {
                        QJsonValue val_clustermin = object.value("clustermin");                         // 获取指定 key 对应的 value
                        double clustermin_ = val_clustermin.toDouble();
                        ui->lineEdit->setText(QString::number(clustermin_));

                        QJsonValue val_clustermax = object.value("clustermax");                         // 获取指定 key 对应的 value
                        double clustermax_ = val_clustermax.toDouble();
                        ui->lineEdit_2->setText(QString::number(clustermax_));

                        QJsonValue val_tolerance = object.value("tolerance");                         // 获取指定 key 对应的 value
                        double tolerance_ = val_tolerance.toDouble();
                        ui->lineEdit_3->setText(QString::number(tolerance_));

                        QJsonValue val_clipmin = object.value("clipmin");                         // 获取指定 key 对应的 value
                        double clipmin_ = val_clipmin.toDouble();
                        ui->lineEdit_4->setText(QString::number(clipmin_));

                        QJsonValue val_clipmax = object.value("clipmax");
                        double clipmax_ = val_clipmax.toDouble();
                        ui->lineEdit_5->setText(QString::number(clipmax_));

                        QJsonValue val_clipleft = object.value("clipleft");
                        double clipleft_ = val_clipleft.toDouble();
                        ui->lineEdit_6->setText(QString::number(clipleft_));

                        QJsonValue val_clipright = object.value("clipright");
                        double clipright_ = val_clipright.toDouble();
                        ui->lineEdit_7->setText(QString::number(clipright_));

                        QJsonValue val_angular = object.value("angular");
                        double angular_ = val_angular.toDouble();
                        ui->lineEdit_10->setText(QString::number(angular_));
                    }else if(13 == flag)
                    {
                        QJsonValue val_right_point0x = object.value("right_point0x");
                        double right_point0x = val_right_point0x.toDouble();
                        ui->lineEdit_13->setText(QString::number(right_point0x));

                        QJsonValue val_right_point0y = object.value("right_point0y");
                        double right_point0y = val_right_point0y.toDouble();
                        ui->lineEdit_23->setText(QString::number(right_point0y));

                        QJsonValue val_right_point1x = object.value("right_point1x");
                        double right_point1x = val_right_point1x.toDouble();
                        ui->lineEdit_24->setText(QString::number(right_point1x));

                        QJsonValue val_right_point1y = object.value("right_point1y");
                        double right_point1y = val_right_point1y.toDouble();
                        ui->lineEdit_25->setText(QString::number(right_point1y));

                        QJsonValue val_left_point0x = object.value("left_point0x");
                        double left_point0x = val_left_point0x.toDouble();
                        ui->lineEdit_26->setText(QString::number(left_point0x));

                        QJsonValue val_left_point0y = object.value("left_point0y");
                        double left_point0y = val_left_point0y.toDouble();
                        ui->lineEdit_27->setText(QString::number(left_point0y));

                        QJsonValue val_left_point1x = object.value("left_point1x");
                        double left_point1x = val_left_point1x.toDouble();
                        ui->lineEdit_28->setText(QString::number(left_point1x));

                        QJsonValue val_left_point1y = object.value("left_point1y");
                        double left_point1y = val_left_point1y.toDouble();
                        ui->lineEdit_29->setText(QString::number(left_point1y));

                        QJsonValue val_height_distance_threshold = object.value("height_distance_threshold");
                        double height_distance_threshold = val_height_distance_threshold.toDouble();
                        ui->lineEdit_30->setText(QString::number(height_distance_threshold));

                    }else if(16 == flag)   //detection area opeation is ok!
                    {
                        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("操作成功！"),NULL);
                    }else if(18 == flag)  //场景参数set ok
                    {
                         QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("操作成功！"),NULL);
                    }else if(19 == flag)
                    {
                        if(!object.contains("apronscene") || !object.contains("fuselagedetect"))
                        {
                            QMessageBox::information(NULL,QString::fromLocal8Bit("警告"),QString::fromLocal8Bit("接受的字段有误，珂珂检查之"));
                            break;
                        }

                        QJsonValue fuselageDetect = object.value("fuselagedetect");
                        int fuselageDetect_ = fuselageDetect.toInt();
                        qDebug()<<"fuselageDetect_ =  "<<fuselageDetect_<<endl;

                        if(0 == fuselageDetect_)
                        {
                            ui->radioButton_2->setChecked(true);
                            ui->radioButton->setChecked(false);
                        }else if(1 == fuselageDetect_)
                        {
                            ui->radioButton_2->setChecked(false);
                            ui->radioButton->setChecked(true);
                        }


                        QJsonValue apronSecne = object.value("apronscene");
                        int apronScene_ = apronSecne.toInt();

                        qDebug()<<"the apronScene_ ="<<apronScene_<<endl;
                        if(0 == apronScene_)
                        {
                            ui->radioButton_4->setChecked(true);
                            ui->radioButton_3->setChecked(false);
                        }else if(1 == apronScene_)
                        {
                            ui->radioButton_4->setChecked(false);
                            ui->radioButton_3->setChecked(true);
                        }

                    }
                }

            }else     //不是Json对象
            {
                qDebug()<<QString::fromLocal8Bit("不是json对象");

                break;
            }
        }else        //documnt is not null
        {
            qDebug()<<QString::fromLocal8Bit("数据解析失败");
            break;
        }

    }//while
}

void MainWindow::on_x_horizontalSlider_sliderMoved(int position)
{
    float pos = position/10000.0;
    currentSlider_x = pos;
    ui->label_x->setText(QString::number(pos));
    emit xchange_signal(pos);
}

void MainWindow::on_y_horizontalSlider_sliderMoved(int position)
{
    float pos = position/10000.0;
    currentSlider_y = pos;
    ui->label_y->setText(QString::number(pos));
    emit ychange_signal(pos);
}

void MainWindow::sliderRangechange_slot(float x_min,float x_max,float y_min,float y_max)
{
//   std::cout<<"range ="<<x_min;
   ui->x_horizontalSlider->setMinimum(x_min);
   ui->x_horizontalSlider->setMaximum(x_max);
   ui->y_horizontalSlider->setMinimum(y_min);
   ui->y_horizontalSlider->setMaximum(y_max);

}

void MainWindow::initSliderPos_slot(float x_pos,float y_pos)
{
    ui->x_horizontalSlider->setValue(x_pos);
    ui->y_horizontalSlider->setValue(y_pos);

    if(10000 == x_pos)
    {
        ui->label_x->setText("0");
    }else
    {
        ui->label_x->setText(QString::number(x_pos));
    }

    if(-10000 == y_pos)
    {
         ui->label_y->setText("0");
    }else
    {
        ui->label_y->setText(QString::number(y_pos));
    }


}

//link_lidar slot
void MainWindow::on_linkLidar_pushButton_clicked()
{
    qDebug()<<QString::fromLocal8Bit("link lidar has coming ");

   QString lidarIP = ui->lineEdit_8->text();
   QString lidarPort = ui->lineEdit_9->text();
//    ui->lineEdit_9->text();

    emit lidarStart_signal(lidarIP,lidarPort);
}

//link_server slot
void MainWindow::on_linkServer_pushButton_clicked()
{
    if((ui->lineEdit_11->text().isEmpty()) || (ui->lineEdit_12->text().isEmpty()))
    {
        QMessageBox::information(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("输入不能为空!"));
        return ;
    }


    QString ipAddress = ui->lineEdit_11->text();
    int port = ui->lineEdit_12->text().toInt();



    tcpClient.abort();


    tcpClient.connectToHost(ipAddress,port);

    if(tcpClient.waitForConnected(1000))
    {
        QMessageBox::information(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("连接成功^_^ !!"));
    }
    else
    {
        QMessageBox::information(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("连接失败v_v !!"));
    }


    QByteArray sendArray = "{\"@table\":5,\"@src\":\"qt\"}";
    sendMsg(sendArray);

//    if(false == udp_socket.bind(6667))
//    {
//         QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("UDP端口绑定失败"),NULL);
//         return;
//    }
//    connect(&udp_socket,SIGNAL(readyRead()),this,SLOT(receUdpMsg_slot()));
}

//update basic
void MainWindow::on_Select_pushButton_clicked()
{
    if(!click_flag)
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("请稍后再试..."),NULL);
        return;
    }
    click_flag = false;
    click_timer.start(3000);


//    tcpClient.write(sendMsg);

    QByteArray sendArray = "{\"@table\":5,\"@src\":\"qt\"}";
    sendMsg(sendArray);
}

//add basic
void MainWindow::on_add_pushButton_clicked()
{
    if(!click_flag)
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("请稍后再试..."),NULL);
        return;
    }
    click_flag = false;
    click_timer.start(3000);


    airplaneModel_Alertflag = false;
    addModel.setModal(true);
    addModel.show();
}

//发送添加机型命令
void MainWindow::addPlaneModel_slot(QString model, float wingLen, float eng_Inner, float eng_Outer, float plane_Len, float nose_Hei)
{
    qDebug()<<QString::fromLocal8Bit("主程序接收到数据")<< model<<"  "<<nose_Hei;

    //判断机型是否已经存在
    int len  = AllPlaneModel_list.length();
    for(int i=0;  i<len; i++)
    {
        if(AllPlaneModel_list[i] == model)
        {
            QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("机型已经存在，添加失败！"),NULL);
            return;
        }
    }

    // 向前端发送添加指令
    QJsonObject add_json;
    QJsonObject msg_json;
    msg_json.insert("CFTP",model);
    msg_json.insert("wing",QString::number(wingLen));
    msg_json.insert("engineInner",QString::number(eng_Inner));
    msg_json.insert("engineOuter",QString::number(eng_Outer));
    msg_json.insert("length",QString::number(plane_Len));
    msg_json.insert("noseheight",QString::number(nose_Hei));

    add_json.insert("@table",6);
    add_json.insert("@src","qt");
    add_json.insert("msg",msg_json);

    QJsonDocument document;
    document.setObject(add_json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    sendMsg(byteArray);
//    tcpClient.write(byteArray);



    //更新界面(增加一行)
    ui->tableWidget->showRow(len);
    planeModel_Item[len].setText(model);
    wingLength_Item[len].setText(QString::number(wingLen));
    engineInner_Item[len].setText(QString::number(eng_Inner));
    engineOuter_Item[len].setText(QString::number(eng_Outer));
    planeLength_Item[len].setText(QString::number(plane_Len));
    noseHeight_Item[len].setText(QString::number(nose_Hei));

    AllPlaneModel_list.append(model);    //机型存储链表中也要增加一行

    airplaneModel_Alertflag = true;
}


//alter basic
void MainWindow::on_alert_pushButton_clicked()
{
    if(!click_flag)
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("请稍后再试..."),NULL);
        return;
    }
    click_flag = false;
    click_timer.start(3000);


    QJsonObject alert_json;
    QJsonArray msgArray_json;
    for(int i=0; i<airPlaneModel_alterlist.length(); i+=6)
    {
        QJsonObject single_json;
        single_json.insert("CFTP",airPlaneModel_alterlist[i]);
        single_json.insert("wing",airPlaneModel_alterlist[i+1]);
        single_json.insert("engineInner",airPlaneModel_alterlist[i+2]);
        single_json.insert("engineOuter",airPlaneModel_alterlist[i+3]);
        single_json.insert("length",airPlaneModel_alterlist[i+4]);
        single_json.insert("noseheight",airPlaneModel_alterlist[i+5]);
        msgArray_json.append(single_json);
    }

    alert_json.insert("@table",7);
    alert_json.insert("@src","qt");
    alert_json.insert("msg",msgArray_json);

    QJsonDocument document;
    document.setObject(alert_json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    sendMsg(byteArray);

    airPlaneModel_alterlist.clear();
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
    if(airplaneModel_Alertflag == false  || row<0)
        return;

      qDebug()<<"the row  = "<<row<<"  the coloum = "<<column;

      //校验
      QRegExp reg("^[0-9 | .]*$");
      QRegExpValidator validator(reg,0);
      int pos = 0;
//      if(QValidator::Acceptable!=validator.validate(wingLength_Item[row].text(),pos) ||
//              QValidator::Acceptable!=validator.validate(engineInner_Item[row].text(),pos)  ||
//              QValidator::Acceptable!=validator.validate(engineOuter_Item[row].text(),pos)  ||
//              QValidator::Acceptable!=validator.validate(planeLength_Item[row].text(),pos)  ||
//              QValidator::Acceptable!=validator.validate(noseHeight_Item[row].text(),pos)
//              )
//         {
//              QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("机型基本参数中不允许有数字！"),NULL);
//              return;
//         }

    if(airPlaneModel_alterlist.empty())
    {

        airPlaneModel_alterlist.append(planeModel_Item[row].text());
        airPlaneModel_alterlist.append(wingLength_Item[row].text());
        airPlaneModel_alterlist.append(engineInner_Item[row].text());
        airPlaneModel_alterlist.append(engineOuter_Item[row].text());
        airPlaneModel_alterlist.append(planeLength_Item[row].text());
        airPlaneModel_alterlist.append(noseHeight_Item[row].text());

        qDebug()<<"the list is empty";
//        on_alert_pushButton_clicked();


    }else
    {
        QString planeModel = planeModel_Item[row].text();

        for(int i=0; i<airPlaneModel_alterlist.length(); i+=6)
        {
            if(planeModel == airPlaneModel_alterlist[i])
            {
                //如果已经存在 更新已有的参数
                airPlaneModel_alterlist[i+1] = wingLength_Item[row].text();
                airPlaneModel_alterlist[i+2] = engineInner_Item[row].text();
                airPlaneModel_alterlist[i+3] = engineOuter_Item[row].text();
                airPlaneModel_alterlist[i+4] = planeLength_Item[row].text();
                airPlaneModel_alterlist[i+5] = noseHeight_Item[row].text();

                qDebug()<<"the list is update "<<wingLength_Item[row].text()<<engineInner_Item[row].text()<<engineOuter_Item[row].text();
//                on_alert_pushButton_clicked();
                return;
            }
        }

        //添加机型的基本参数
        airPlaneModel_alterlist.append(planeModel_Item[row].text());
        airPlaneModel_alterlist.append(wingLength_Item[row].text());
        airPlaneModel_alterlist.append(engineInner_Item[row].text());
        airPlaneModel_alterlist.append(engineOuter_Item[row].text());
        airPlaneModel_alterlist.append(planeLength_Item[row].text());
        airPlaneModel_alterlist.append(noseHeight_Item[row].text());

    }
}



//update parameter
void MainWindow::on_Select2_pushButton_clicked()
{
    if(!click_flag)
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("请稍后再试..."),NULL);
        return;
    }
    click_flag = false;
    click_timer.start(3000);


    QByteArray byteArray = "{\"@table\":8,\"@src\":\"qt\"}" ;
    sendMsg(byteArray);
}

//add parameter
void MainWindow::on_add2_pushButton_clicked()
{
    if(!click_flag)
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("请稍后再试..."),NULL);
        return;
    }
    click_flag = false;
    click_timer.start(3000);


    Detection_Alertflag = false;
    addDetetion_diag.show();
}

//添加检测型号与参数
void MainWindow::addDetetion_slot(QString planeMod)
{

    //判断是否有重复
    for(int i=0; i<AllDetectionModel_list.length(); i++)
    {
        if(planeMod == AllDetectionModel_list[i])
        {
            QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("该机型已存在，添加失败!"));
            return ;
        }
    }

    //向前端发送添加信息
    QJsonObject addDetection_json;
    QJsonObject single_json;
    single_json.insert("CFTP",planeMod);
    single_json.insert("stop1x","");
    single_json.insert("stop1y","");
    single_json.insert("stop2x","");
    single_json.insert("stop2y","");
    single_json.insert("middle1x","");
    single_json.insert("middle1y","");
    single_json.insert("middle2x","");
    single_json.insert("middle2y","");

    addDetection_json.insert("@table",9);
    addDetection_json.insert("@src","qt");
    addDetection_json.insert("msg",single_json);

    QJsonDocument document;
    document.setObject(addDetection_json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    sendMsg(byteArray);


//    tcpClient.write(byteArray);
//    qDebug()<<QString::fromLocal8Bit("发送给前端的增加检测信息的命令为:")<<byteArray;


    //刷新界面显示
    int len  = AllDetectionModel_list.length();
    planeModel_Item2[len].setText(planeMod);
//    stop1_x_Item[len].setText(stop1x_);
//    stop1_y_Item[len].setText(stop1y_);
//    stop2_x_Item[len].setText(stop2x_);
//    stop2_y_Item[len].setText(stop2y_);
//    center1_x_Item[len].setText(mid1x_);
//    center1_y_Item[len].setText(mid1y_);
//    center2_x_Item[len].setText(mid2x_);
//    center2_y_Item[len].setText(mid2y_);
    ui->tableWidget_2->showRow(len);

    AllDetectionModel_list.append(planeMod);

    Detection_Alertflag = true;
}





//alter parameter
void MainWindow::on_alert2_pushButton_clicked()
{
    if(!click_flag)
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("请稍后再试..."),NULL);
        return;
    }
    click_flag = false;
    click_timer.start(3000);



    QJsonObject alert_json;
    QJsonArray msgArray;

    for(int i=0; i<Detection_alterlist.length(); i += 9)
    {
        QJsonObject single_json;
        single_json.insert("CFTP",Detection_alterlist[i]);
        single_json.insert("stop1x",Detection_alterlist[i+1]);
        single_json.insert("stop1y",Detection_alterlist[i+2]);
        single_json.insert("stop2x",Detection_alterlist[i+3]);
        single_json.insert("stop2y",Detection_alterlist[i+4]);
        single_json.insert("middle1x",Detection_alterlist[i+5]);
        single_json.insert("middle1y",Detection_alterlist[i+6]);
        single_json.insert("middle2x",Detection_alterlist[i+7]);
        single_json.insert("middle2y",Detection_alterlist[i+8]);

        msgArray.append(single_json);
    }
    alert_json.insert("@table",10);
    alert_json.insert("@src","qt");
    alert_json.insert("msg",msgArray);


    QJsonDocument document;
    document.setObject(alert_json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    sendMsg(byteArray);

//    tcpClient.write(byteArray);
//    qDebug()<<QString::fromLocal8Bit("发送给前端的修改检测信息的命令为:")<<byteArray;



    Detection_alterlist.clear();
}

void MainWindow::on_tableWidget_2_cellChanged(int row, int column)
{
    if(row<0 || Detection_Alertflag == false)
    {
        return ;
    }

    qDebug()<<"the row  = "<<row<<"  the coloum = "<<column;

    QRegExp reg("^-?[0-9 | .]*$");


    QRegExpValidator validator(reg,0);
    int pos = 0;

//    if(QValidator::Acceptable!=validator.validate(stop1_x_Item[row].text(),pos) ||
//            QValidator::Acceptable!=validator.validate(stop1_y_Item[row].text(),pos)  ||
//            QValidator::Acceptable!=validator.validate(stop2_x_Item[row].text(),pos)  ||
//            QValidator::Acceptable!=validator.validate(stop2_y_Item[row].text(),pos)  ||
//            QValidator::Acceptable!=validator.validate(center1_x_Item[row].text(),pos)   ||
//            QValidator::Acceptable!=validator.validate(center1_y_Item[row].text(),pos)   ||
//            QValidator::Acceptable!=validator.validate(center2_x_Item[row].text(),pos)   ||
//            QValidator::Acceptable!=validator.validate(center2_y_Item[row].text(),pos)

//            )
//       {
//            QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("机型检测参数中只允许有数字！"),NULL);
//            return;
//       }



    if(Detection_alterlist.empty())
    {

        Detection_alterlist.append(planeModel_Item2[row].text());
        Detection_alterlist.append(stop1_x_Item[row].text());
        Detection_alterlist.append(stop1_y_Item[row].text());
        Detection_alterlist.append(stop2_x_Item[row].text());
        Detection_alterlist.append(stop2_y_Item[row].text());
        Detection_alterlist.append(center1_x_Item[row].text());
        Detection_alterlist.append(center1_y_Item[row].text());
        Detection_alterlist.append(center2_x_Item[row].text());
        Detection_alterlist.append(center2_y_Item[row].text());

        qDebug()<<"the list is empty";
    }else
    {
        QString planeModel = planeModel_Item2[row].text();

        for(int i=0; i<Detection_alterlist.length(); i+=6)
        {
            if(planeModel == Detection_alterlist[i])
            {
                //如果已经存在 更新已有的参数
                Detection_alterlist[i+1] = stop1_x_Item[row].text();
                Detection_alterlist[i+2] = stop1_y_Item[row].text();
                Detection_alterlist[i+3] = stop2_x_Item[row].text();
                Detection_alterlist[i+4] = stop2_y_Item[row].text();
                Detection_alterlist[i+5] = center1_x_Item[row].text();
                Detection_alterlist[i+6] = center1_y_Item[row].text();
                Detection_alterlist[i+7] = center2_x_Item[row].text();
                Detection_alterlist[i+8] = center2_y_Item[row].text();

                qDebug()<<"the list is update "<<stop1_x_Item[row].text()<<stop1_y_Item[row].text()<<center2_y_Item[row].text();
                return;
            }
        }
        //添加机型的基本参数
        Detection_alterlist.append(planeModel_Item2[row].text());
        Detection_alterlist.append(stop1_x_Item[row].text());
        Detection_alterlist.append(stop1_y_Item[row].text());
        Detection_alterlist.append(stop2_x_Item[row].text());
        Detection_alterlist.append(stop2_y_Item[row].text());
        Detection_alterlist.append(center1_x_Item[row].text());
        Detection_alterlist.append(center1_y_Item[row].text());
        Detection_alterlist.append(center2_x_Item[row].text());
        Detection_alterlist.append(center2_y_Item[row].text());
    }
}


//person parameter
void MainWindow::on_Person_pushButton_clicked()
{
    if(!click_flag)
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("请稍后再试..."),NULL);
        return;
    }
    click_flag = false;
    click_timer.start(3000);


    QJsonObject alert_json;

    QString clu_min_ = ui->lineEdit->text();
    QString clu_max_ = ui->lineEdit_2->text();
    QString toleran_ = ui->lineEdit_3->text();
    QString clipmin_ = ui->lineEdit_4->text();
    QString clipmax_ = ui->lineEdit_5->text();
    QString clipleft_ = ui->lineEdit_6->text();
    QString clipright_ = ui->lineEdit_7->text();
    QString angular_ = ui->lineEdit_10->text();

    float temp = angular_.toFloat();

    if((temp<0) || (temp>15))
    {
        QMessageBox::information(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("angular的值超出范围，请重新输入！"));
        return ;
    }


    if(clu_min_.isEmpty() || clu_max_.isEmpty() || toleran_.isEmpty() || clipmin_.isEmpty() || clipmax_.isEmpty() || clipleft_.isEmpty() || clipright_.isEmpty()|| angular_.isEmpty())
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("不允许空值，请重新输入"),NULL);
        return ;
    }

    alert_json.insert("@table",12);
    alert_json.insert("@src","qt");
    alert_json.insert("clustermin",clu_min_);
    alert_json.insert("clustermax",clu_max_);
    alert_json.insert("tolerance",toleran_);
    alert_json.insert("clipmin",clipmin_);
    alert_json.insert("clipmmax",clipmax_);
    alert_json.insert("clipleft",clipleft_);
    alert_json.insert("clipright",clipright_);
    alert_json.insert("angular",angular_);

    QJsonDocument document;
    document.setObject(alert_json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);


    sendMsg(byteArray);

//    tcpClient.write(byteArray);
//    qDebug()<<QString::fromLocal8Bit("发送给前端的修改行人检测信息的命令为:")<<byteArray;
}







void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(0 == index)       //查询机型
    {
        QByteArray sendArray = "{\"@table\":5,\"@src\":\"qt\"}";
        sendMsg(sendArray);
        ui->selDetePoint_pushButton->setDisabled(true);
        DetectonPoints_List.clear();
        ui->showDetePoint_textEdit->clear();

    }else if(1 == index)  //查询检测参数
    {
        emit updateModel_signal(AllPlaneModel_list);
        QByteArray sendArray = "{\"@table\":8,\"@src\":\"qt\"}";
        ui->selDetePoint_pushButton->setDisabled(true);
        DetectonPoints_List.clear();
        ui->showDetePoint_textEdit->clear();
        sendMsg(sendArray);


    }else if(2 == index)  //查询行人检测参数
    {
        QByteArray snedArray = "{\"@table\":11,\"@src\":\"qt\"}";
        sendMsg(snedArray);
        DetectonPoints_List.clear();
        ui->showDetePoint_textEdit->clear();
        ui->selDetePoint_pushButton->setDisabled(true);
        qDebug()<<QString::fromLocal8Bit("查询行人检测参数已经发出！");

    }else if(3 == index)
    {
        QByteArray snedArray = "{\"@table\":13,\"@src\":\"qt\"}";
        sendMsg(snedArray);
        ui->selDetePoint_pushButton->setDisabled(true);
        DetectonPoints_List.clear();
        ui->showDetePoint_textEdit->clear();
        qDebug()<<QString::fromLocal8Bit("查询环境检测参数已经发出！");
    }else if(4 == index)  //检测区域界面
    {
        ui->selDetePoint_pushButton->setDisabled(false);
    }else if(5 == index)  // 查询场景参数
    {
        QByteArray snedArray = "{\"@table\":19,\"@src\":\"qt\"}";
        sendMsg(snedArray);
    }
}

void MainWindow::on_tableWidget_2_cellDoubleClicked(int row, int column)
{
    qDebug()<<QString::fromLocal8Bit("双击函数已经进来 column")<< column <<endl;

//    stop1_x_Item[row].setText("(56.1 , 12.02)");
    QMessageBox::information(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("左侧选定坐标后,点击修改检测参数,完成修改!"));
    ui->pushButton->setDisabled(false);
    Alter_row = row;
    Alter_col = column;

}

//修改某个单元格的检测参数
void MainWindow::on_pushButton_clicked()
{
    QString str ;
    QString planeModel_str,stop1x_str, stop1y_str, stop2x_str, stop2y_str, mid1x_str, mid1y_str, mid2x_str, mid2y_str;
    QString tmp_str;
    QStringList str_list;

    if(1 == Alter_col)  //stop1
    {
        str = "(" + QString::number(currentSlider_x)+" , " + QString::number(currentSlider_y)+")";
        stop1_x_Item[Alter_row].setText(str);
        //////////////////////////////////
        stop1x_str = QString::number(currentSlider_x);
        stop1y_str = QString::number(currentSlider_y);

    }else if(2 == Alter_col ) //stop2
    {
        str = "(" + QString::number(currentSlider_x)+" , " + QString::number(currentSlider_y)+")";
        stop2_x_Item[Alter_row].setText(str);
        //////////////////////////////////
        stop2x_str = QString::number(currentSlider_x);
        stop2y_str = QString::number(currentSlider_y);

    }else if(3 == Alter_col)  //mid1
    {
        str = "(" + QString::number(currentSlider_x)+" , " + QString::number(currentSlider_y)+")";
        center1_x_Item[Alter_row].setText(str);
        //////////////////////////////////
        mid1x_str = QString::number(currentSlider_x);
        mid1y_str = QString::number(currentSlider_y);

    }else if(4 == Alter_col)  //mid2
    {
        str = "(" + QString::number(currentSlider_x)+" , " + QString::number(currentSlider_y)+")";
        center2_x_Item[Alter_row].setText(str);
        //////////////////////////////////
        mid2x_str = QString::number(currentSlider_x);
        mid2y_str = QString::number(currentSlider_y);
    }

    planeModel_str = planeModel_Item2[Alter_row].text();

    tmp_str = stop1_x_Item[Alter_row].text();
    if(!tmp_str.isEmpty())
    {
        tmp_str = tmp_str.left(tmp_str.length()-1);
        tmp_str = tmp_str.right(tmp_str.length()-1);
        str_list = tmp_str.split(",");
        stop1x_str = str_list[0];
        stop1y_str = str_list[1];
    }

    tmp_str = stop2_x_Item[Alter_row].text();
    if(!tmp_str.isEmpty())
    {

        tmp_str = tmp_str.left(tmp_str.length()-1);
        tmp_str = tmp_str.right(tmp_str.length()-1);
        str_list = tmp_str.split(",");
        stop2x_str = str_list[0];
        stop2y_str = str_list[1];
    }
    tmp_str = center1_x_Item[Alter_row].text();
    if(!tmp_str.isEmpty())
    {

        tmp_str = tmp_str.left(tmp_str.length()-1);
        tmp_str = tmp_str.right(tmp_str.length()-1);
        str_list = tmp_str.split(",");
        mid1x_str = str_list[0];
        mid1y_str = str_list[1];
    }

    tmp_str = center2_x_Item[Alter_row].text();
    if(!tmp_str.isEmpty())
    {

        tmp_str = tmp_str.left(tmp_str.length()-1);
        tmp_str = tmp_str.right(tmp_str.length()-1);
        str_list = tmp_str.split(",");
        mid2x_str = str_list[0];
        mid2y_str = str_list[1];
    }

    QJsonObject alert_json;
    QJsonArray msgArray;

    QJsonObject single_json;
    single_json.insert("CFTP",planeModel_str);
    single_json.insert("stop1x",stop1x_str);
    single_json.insert("stop1y",stop1y_str);
    single_json.insert("stop2x",stop2x_str);
    single_json.insert("stop2y",stop2y_str);
    single_json.insert("middle1x",mid1x_str);
    single_json.insert("middle1y",mid1y_str);
    single_json.insert("middle2x",mid2x_str);
    single_json.insert("middle2y",mid2y_str);
    msgArray.append(single_json);

    alert_json.insert("@table",10);
    alert_json.insert("@src","qt");
    alert_json.insert("msg",msgArray);

    QJsonDocument document;
    document.setObject(alert_json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    sendMsg(byteArray);

//    qDebug()<<"stop1x_str = "<<stop1x_str<<"  stop1y_str"<<stop1y_str<<endl;

    ui->pushButton->setDisabled(true);
}

void MainWindow::on_Person_pushButton_3_clicked()
{
    if(!click_flag)
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("请稍后再试..."),NULL);
        return;
    }
    click_flag = false;
    click_timer.start(3000);

    QJsonObject alert_json;

    QString right_point0x = ui->lineEdit_13->text();
    QString right_point0y = ui->lineEdit_23->text();
    QString right_point1x = ui->lineEdit_24->text();
    QString right_point1y = ui->lineEdit_25->text();
    QString left_point0x = ui->lineEdit_26->text();
    QString left_point0y = ui->lineEdit_27->text();
    QString left_point1x = ui->lineEdit_28->text();
    QString left_point1y = ui->lineEdit_29->text();
    QString height_distance_threshold = ui->lineEdit_30->text();

    if(right_point0x.isEmpty() || right_point0y.isEmpty() || right_point1x.isEmpty() || right_point1y.isEmpty() || left_point0x.isEmpty() || left_point0y.isEmpty() || left_point1x.isEmpty()|| left_point1y.isEmpty()||height_distance_threshold.isEmpty())
    {
        QMessageBox::information(NULL,NULL,QString::fromLocal8Bit("不允许空值，请重新输入"),NULL);
        return ;
    }

    alert_json.insert("@table",14);
    alert_json.insert("@src","qt");
    alert_json.insert("right_point0x",right_point0x);
    alert_json.insert("right_point0y",right_point0y);
    alert_json.insert("right_point1x",right_point1x);
    alert_json.insert("right_point1y",right_point1y);
    alert_json.insert("left_point0x",left_point0x);
    alert_json.insert("left_point0y",left_point0y);
    alert_json.insert("left_point1x",left_point1x);
    alert_json.insert("left_point1y",left_point1y);
    alert_json.insert("height_distance_threshold",height_distance_threshold);

    QJsonDocument document;
    document.setObject(alert_json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    sendMsg(byteArray);
}

void MainWindow::receUdpMsg_slot()
{
    QByteArray dataArray;

    while(udp_socket.hasPendingDatagrams())
    {
        dataArray.resize(udp_socket.pendingDatagramSize());
        udp_socket.readDatagram(dataArray.data(), dataArray.size());

        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(dataArray, &jsonError);                     // 转化为 JSON 文档
        if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
        {                                                                                            // 解析未发生错误
            if (doucment.isObject()) {
                QJsonObject object = doucment.object();                                              // 转化为对象
                if (object.contains("@table"))
                {                                                                                    // 包含指定的 key
                    QJsonValue val_flag = object.value("@table");                                    // 获取指定 key 对应的 value
                    int flag = val_flag.toInt();
                    qDebug() << "@table " << flag;
                    if(16 == flag)              //机型基本参数返回表
                    {
                        if(!(object.contains("yaxis")&&object.contains("tempture")))
                        {
                            qDebug()<<QString::fromLocal8Bit("接收到的命令有误，请珂珂检查之")<<endl;
                            continue;
                        }

                      QJsonValue value_yaxis = object.value("yaxis");
                      QJsonValue value_tempture = object.value("tempture");

                      double yaxis_ = value_yaxis.toDouble();
                      double tempture_ = value_tempture.toDouble();

                      ui->yamis_label->setText(QString::number(yaxis_));
                      ui->tempture_label->setText(QString::number(tempture_));
                    }
                }

            }else     //不是Json对象
            {
                qDebug()<<QString::fromLocal8Bit("不是json对象");

                continue;
            }
        }else        //documnt is not null
        {
            qDebug()<<QString::fromLocal8Bit("数据解析失败");
            continue;
        }
    }
}


void MainWindow::heartBeat_slot()
{
    QByteArray snedArray = "{\"@table\":15,\"@src\":\"qt\"}";
    sendMsg(snedArray);

    qDebug()<<"heart beat signal has send ";
}




/*
 * @brief:  add the detec point
 * @date:  2019-4-10
 * @author:zwt
 */
void MainWindow::on_selDetePoint_pushButton_clicked()
{
    //在控件上显示添加的检测点
    QString alreadyText = ui->showDetePoint_textEdit->toPlainText();
    alreadyText.append("x:"+QString::number(currentSlider_x)+"   y:"+QString::number(currentSlider_y)+"\n");
    ui->showDetePoint_textEdit->setText(alreadyText);

    QString currentText = "("+QString::number(currentSlider_x) +" , "+ QString::number(currentSlider_y) +")";
    qDebug()<<"currentText ="<<currentText<<endl;
    DetecPoint_Item[currentDetePoints_index].setText(currentText);
    currentDetePoints_index++;

    //序列添加检测点
    DetectonPoints_List.append(QString::number(currentSlider_x));
    DetectonPoints_List.append(QString::number(currentSlider_y));

    QMessageBox::information(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("完成添加,请在右侧窗口查看已添加的检测点!"));
}


/*
 * @brief:  alter the detec point
 * @date:  2019-4-10
 * @author:zwt
 */
void MainWindow::on_DetePoint_tableWidget_cellDoubleClicked(int row, int column)
{

}


/*
 * @brief:  show the detec point on openGL
 * @date:  2019-4-10
 * @author:zwt
 */
void MainWindow::on_showDetecPoint_pushButton_clicked()
{

}


void MainWindow::on_addDetePoint_pushButton_clicked()
{
    QJsonObject alert_json;
    QJsonArray msgArray;
    if(DetectonPoints_List.length()<1)
    {
        QMessageBox::information(NULL,QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("不能为空！请先添加检测点！"));
        return ;
    }

    for(int i=0; i<DetectonPoints_List.length(); i += 2)
    {
        QJsonObject single_json;
        single_json.insert("x",DetectonPoints_List[i]);
        single_json.insert("y",DetectonPoints_List[i+1]);
        msgArray.append(single_json);
    }
    alert_json.insert("@table",10);
    alert_json.insert("@src","qt");
    alert_json.insert("coordinates",msgArray);

    QJsonDocument document;
    document.setObject(alert_json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    sendMsg(byteArray);
    qDebug()<<" add the detetion msg = "<<byteArray<<endl;
}






//submit the scence parameter slot
void MainWindow::on_submitPara_pushButton_clicked()
{

    QByteArray sendArray = "{\"@table\":18,\"@src\":\"qt\",";

    if(true == ui->radioButton->isChecked())
    {
        sendArray.append("\"fuselagedetect\":1,");

    }else
    {
        sendArray.append("\"fuselagedetect\":0,");
    }

    if(true == ui->radioButton_3->isChecked())
    {
        sendArray.append("\"apronsecne\":1}");
    }else
    {
        sendArray.append("\"apronsecne\":0}");
    }

    qDebug()<<"the msg ="<<sendArray<<endl;
    sendMsg(sendArray);

}




