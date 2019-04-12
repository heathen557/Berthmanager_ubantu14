#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QWidget>
#include <QEvent>
#include <QMouseEvent>
#include "mythread.h"
#include<QTimer>



class openglwindow : public QOpenGLWidget,
                     protected QOpenGLFunctions
{
    Q_OBJECT

public:
    openglwindow(QWidget *parent = 0);
    ~openglwindow();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void draw();
    int faceAtPosition(const QPoint &pos);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);



    GLfloat rotationX;
    GLfloat rotationY;
    GLfloat rotationZ;
    GLfloat zoom;
    GLfloat zoom_amount;
    GLfloat faceColors[4];
    QPoint lastPos;
    QTimer readPCAP_timer;

    float min_x,max_x,min_y,max_y,floor_z;

    float x_change,y_change;

    bool isFirst_show;

    MyThread readFile_thread;

    QStringList showDetectionPoints_list;

    bool isShowPolygon_flag;

public slots:
    void xchange_slot(float );
    void ychange_slot(float );
    void lidarStart_slot(QString IP,QString port);
    void readPCAP_slot();

signals:
    void sliderRangechange_signal(float,float,float,float);

    void initSliderPos_signal(float,float);
};


#endif // OPENGLWINDOW_H
