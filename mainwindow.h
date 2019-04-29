#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<add_model.h>
#include<add_detetion.h>
#include <QTableWidgetItem>
#include <QTimer>
#include<QtNetwork/QTcpSocket>
#include <QtNetwork/QUdpSocket>
//#include<QTcpSocket>

//extern QString lidar_IP;
//extern QString lidar_port;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    add_model addModel;
    add_detetion addDetetion_diag;
    void sendMsg(QByteArray array);
    void closeEvent(QCloseEvent *event);

private slots:
    void on_x_horizontalSlider_sliderMoved(int position);

    void on_y_horizontalSlider_sliderMoved(int position);

    void sliderRangechange_slot(float,float,float,float);

    void initSliderPos_slot(float x_pos,float y_pos);

    void on_linkLidar_pushButton_clicked();

    void on_Select_pushButton_clicked();

    void on_add_pushButton_clicked();

    void on_alert_pushButton_clicked();

    void on_Select2_pushButton_clicked();

    void on_add2_pushButton_clicked();

    void on_alert2_pushButton_clicked();

    void on_Person_pushButton_clicked();

    void readMessage();

    void on_linkServer_pushButton_clicked();

    void addPlaneModel_slot(QStringList paraList);

    void addDetetion_slot(QString);

    void on_tableWidget_cellChanged(int row, int column);

    void on_tableWidget_2_cellChanged(int row, int column);

    void on_tabWidget_currentChanged(int index);

    void click_timeSlot();

    void on_tableWidget_2_cellDoubleClicked(int row, int column);

    void on_pushButton_clicked();

    void on_Person_pushButton_3_clicked();

    void receUdpMsg_slot();

    void heartBeat_slot();

    void on_addDetePoint_pushButton_clicked();

    void on_selDetePoint_pushButton_clicked();

    void on_submitPara_pushButton_clicked();

    void on_DetePoint_tableWidget_cellDoubleClicked(int row, int column);

    void on_showDetecPoint_pushButton_clicked();

    void on_DetePoint_tableWidget_cellClicked(int row, int column);

private:
    /************************************************/
    QTableWidgetItem planeModel_Item[128];
    QTableWidgetItem wingLength1_Item[128];
    QTableWidgetItem wingLength2_Item[128];
    QTableWidgetItem wingLengthAdjust_Item[128];
    QTableWidgetItem wingLengthPosOffset_Item[128];
    QTableWidgetItem wingLengthNegOffset_Item[128];

    QTableWidgetItem engineInner_Item[128];
    QTableWidgetItem engineInnerAdjust_Item[128];
    QTableWidgetItem engineInnerPosOffset_Item[128];
    QTableWidgetItem engineInnerNegOffset_Item[128];

    QTableWidgetItem engineOuter_Item[128];
    QTableWidgetItem engineOuterAdjust_Item[128];
    QTableWidgetItem engineOuterPosOffset_Item[128];
    QTableWidgetItem engineOuterNegOffset_Item[128];

    QTableWidgetItem planeLength_Item[128];
    QTableWidgetItem planeLengthAdjust_Item[128];
    QTableWidgetItem planeLengthPosOffset_Item[128];
    QTableWidgetItem planeLengthNegOffset_Item[128];

    QTableWidgetItem planeHeight_Item[128];
    QTableWidgetItem planeHeightPosOffset_Item[128];
    QTableWidgetItem planeHeightNegOffset_Item[128];

    QTableWidgetItem planeWidth_Item[128];
    QTableWidgetItem planeWidthPosOffset_Item[128];
    QTableWidgetItem planeWidthNegOffset_Item[128];

    QTableWidgetItem noseHeight_Item[128];
    QTableWidgetItem noseHeightPosOffset_Item[128];
    QTableWidgetItem noseHeightNegOffset_Item[128];

    QTableWidgetItem noseDoor_Item[128];
    QTableWidgetItem noseDoorPosOffset_Item[128];
    QTableWidgetItem noseDoorNegOffset_Item[128];


    /*********************************************************/


    QTableWidgetItem planeModel_Item2[128];
    QTableWidgetItem stop1_x_Item[128];
    QTableWidgetItem stop1_y_Item[128];
    QTableWidgetItem stop2_x_Item[128];
    QTableWidgetItem stop2_y_Item[128];
    QTableWidgetItem center1_x_Item[128];
    QTableWidgetItem center1_y_Item[128];
    QTableWidgetItem center2_x_Item[128];
    QTableWidgetItem center2_y_Item[128];

    QTableWidgetItem index_Item[50];           //检测区域时 标识
    QTableWidgetItem DetecPoint_Item[50];      //检测区域时 检测点坐标


    QTcpSocket tcpClient;
    bool airplaneModel_Alertflag;
    bool Detection_Alertflag;
    QTimer click_timer;              //三秒钟内不允许重复点击
    bool click_flag;

    QStringList airPlaneModel_alterlist;               //要修改的 机型基本参数的列表
    QStringList Detection_alterlist;                   //要修改的检测参数的修改列表
    QStringList AllPlaneModel_list;                    //存储所有的机型
    QStringList AllDetectionModel_list;                //存储泊位检测的机型

    QByteArray m_buffer;

    int  Alter_row;                          //修改参数的行和列
    int  Alter_col;

    float currentSlider_x;
    float currentSlider_y;

    QUdpSocket udp_socket;

    QTimer heartBeat_timer;

    QStringList DetectonPoints_List;

    int currentDetePoints_index;

signals:
    void xchange_signal(float);
    void ychange_signal(float);
    void updateModel_signal(QStringList);
    void lidarStart_signal(QString,QString);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
