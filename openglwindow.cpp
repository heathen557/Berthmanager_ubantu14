#include "openglwindow.h"

#include<GL/gl.h>
#include<GL/glu.h>

#include <pcl/visualization/cloud_viewer.h>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>

#include <QtOpenGL/QtOpenGL>
#include<QDebug>

using namespace std;

extern CloudPtr _src_cloud;

openglwindow::openglwindow(QWidget *parent)
    :QOpenGLWidget(parent)
{

    showDetectionPoints_list.clear();
    isShowPolygon_flag = false;
//    rotationX = -21.0;
//    rotationY = -57.0;
//    rotationZ = -0.0;
    isFirst_show = true;

    rotationX = 0.0;
    rotationY = 10.0;
    rotationZ = -15.0;

    zoom = 0.04;


    faceColors[0] = 1;
    faceColors[1] = 0.8;
    faceColors[2] = 0.4;
    faceColors[3] = 0.2;


    min_x = 10000;
    min_y = 10000;
    max_x = -10000;
    max_y = -10000;
    floor_z = 10000;


    connect(&readPCAP_timer,SIGNAL(timeout()),this,SLOT(readPCAP_slot()));


    readPCAP_timer.start(300);
}

void openglwindow::lidarStart_slot(QString IP,QString port)
{
    readFile_thread.lidar_IP = IP;
    readFile_thread.lidar_port = port;
    readFile_thread.start();
}

openglwindow::~openglwindow()
{

}

void openglwindow::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0,0,0,1);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}

void openglwindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    draw();
}

void openglwindow::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat x = GLfloat(w) / h;
    glFrustum(-x,+x,-1.0,+0.1,3,20.0);

//    glFrustum(-5,+5,-10.0,+5.0,5,15.0);
    glMatrixMode(GL_MODELVIEW);
    cout<<"w="<<w<<"  h="<<h<<"  x="<<x<<endl;

}

void openglwindow::draw()
{
    static const GLfloat P1[3] = {0.0, -1.0, +2.0};
    static const GLfloat P2[3] = {+1.73205081, -1.0, -1.0};
    static const GLfloat P3[3] = {-1.73205081, -1.0, -1.0};
    static const GLfloat P4[3] = {0.0, 2.0, 0.0};
    static const GLfloat *const coords[4][3] = {
        {P1, P2, P3}, {P1, P3, P4}, {P1, P4, P2}, {P2, P4, P3}
    };
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
//    glTranslatef(0.0, 0.0, -10.0);

    glTranslatef(0.0, -1.0, -10.0);

    glRotatef(rotationX, 1.0, 0.0, 0.0);
    glRotatef(rotationY, 0.0, 1.0, 0.0);
    glRotatef(rotationZ, 0.0, 0.0, 1.0);

    glScaled(zoom, zoom, zoom);

//    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
//    pcl::io::loadPCDFile("0001.pcd", *cloud);


// draw the pcl file  points
    for(size_t i=0; i<_src_cloud->points.size(); i++)
    {
        glLoadName(i);
        glPointSize(1.0);
        glBegin(GL_POINTS);

        float cloud_x = _src_cloud->points[i].x;
        float cloud_y = _src_cloud->points[i].y;
        float cloud_z = _src_cloud->points[i].z;

        glColor3f(faceColors[1],faceColors[2],0);
        glVertex3f(cloud_x,cloud_y,cloud_z);

        if(cloud_x>max_x)
            max_x = cloud_x;
        if(cloud_x<min_x)
            min_x = cloud_x;
        if(cloud_y>max_y)
            max_y = cloud_y;
        if(cloud_y<min_y)
            min_y = cloud_y;
        if(cloud_z<floor_z)
            floor_z = cloud_z;
        glEnd();
    }
    emit sliderRangechange_signal(min_x,max_x,min_y,max_y);

////////////////////draw the x_slider////////////////////////
    glBegin(GL_LINES);
    glColor3f(1.0,0,0);
    if(true == isFirst_show)
    {
        glVertex3f(min_x,max_y,floor_z);
        glVertex3f(max_x,max_y,floor_z);
//        qDebug()<<"this is the first coming "<<endl;

    }else
    {
        glVertex3f(min_x,y_change,floor_z);
        glVertex3f(max_x,y_change,floor_z);

    }
    glEnd();

////////////////////draw the y_slider////////////////////////
    glBegin(GL_LINES);
    glColor3f(1.0,0,0);

    if(true == isFirst_show)
    {
//        float mid_x = (max_x + min_x)/2;

        glVertex3f(min_x,min_y,floor_z);
        glVertex3f(min_x,max_y,floor_z);

        //init the slider ; and init the x_change,y_change
        emit initSliderPos_signal(min_x,max_y);
        x_change = min_x;
        y_change = max_y;

    }else
    {
        glVertex3f(x_change,min_y,floor_z);
        glVertex3f(x_change,max_y,floor_z);
    }
    glEnd();



///////////////draw the Detection points//////////////////////////

    glPointSize(5.0);
    glBegin(GL_POINTS);
    glColor3f(0,1.0,0);

    for(int i=0; i<showDetectionPoints_list.size(); i=i+2)
    {
        QString detecPoint_x_str = showDetectionPoints_list[i];
        QString detecPoint_y_str = showDetectionPoints_list[i+1];
        float detecPoint_x = detecPoint_x_str.toFloat();
        float detecPoint_y = detecPoint_y_str.toFloat();

        glVertex3f(detecPoint_x,detecPoint_y,floor_z);
    }
    glEnd();


    /////////////draw the detection points polygon/////////////////////
    if(true == isShowPolygon_flag)
    {
       glBegin(GL_LINE_LOOP);
       glColor3f(0,1.0,0);
       for(int i=0; i<showDetectionPoints_list.size(); i+=2)
       {
           QString detecPoint_x_str = showDetectionPoints_list[i];
           QString detecPoint_y_str = showDetectionPoints_list[i+1];
           float detecPoint_x = detecPoint_x_str.toFloat();
           float detecPoint_y = detecPoint_y_str.toFloat();
           glVertex3f(detecPoint_x,detecPoint_y,floor_z);
       }
       glEnd();

    }




//    std::cerr << "max_x:"<< max_x<<"  min_x:"<<min_x<<"  max_y:"<<max_y<<"  min_y:"<<min_y<<"  floor_z:"<<floor_z<<std::endl;

}


int openglwindow::faceAtPosition(const QPoint &pos)
{
    const int MaxSize = 512;
    GLuint buffer[MaxSize];
    GLint viewport[4];
    makeCurrent();
    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(MaxSize, buffer);
    glRenderMode(GL_SELECT);

    glInitNames();
    glPushName(0);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(GLdouble(pos.x()), GLdouble(viewport[3] - pos.y()), 5.0, 5.0, viewport);
    GLfloat x = GLfloat(width()) / height();
    glFrustum(-x, x, -1.0, 0.0, 4.0, 15.0);
    draw();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    if(!glRenderMode(GL_RENDER))
        return -1;
    return buffer[3];
}


void openglwindow::mousePressEvent(QMouseEvent *event)
{
//     std::cerr << "  this is the first  "<< event->pos()<<std::endl;
//     qDebug()<<"this is the first"<<event->pos();
    printf("the point is = %d",event->pos().x());
     lastPos = event->pos();
}

void openglwindow::mouseMoveEvent(QMouseEvent *event)
{
    GLfloat dx = GLfloat(event->x() - lastPos.x()) / width();
    GLfloat dy = GLfloat(event->y() - lastPos.y()) / height();
    if(event->buttons() & Qt::LeftButton){
        rotationX -= 180 * dy;
        rotationY -= 180 * dx;
        update();
    }
    else if(event->buttons() & Qt::RightButton){
        rotationX -= 180 * dy;
        rotationZ -= 180 * dx;
        update();
    }
    lastPos = event->pos();
}

void openglwindow::wheelEvent(QWheelEvent *event)
{
     std::cerr << "  this is the first  "<< event->delta()<<std::endl;
     if (event->delta() > 0)
     {
         zoom += 0.03;
     }
     else
     {
         if(zoom > 0.03)
            zoom -= 0.03;
     }

     std::cerr << " suofang zoom =  "<<zoom<<std::endl;
     update();

}


void openglwindow::mouseDoubleClickEvent(QMouseEvent *event)
{

}

void openglwindow::xchange_slot(float pos)
{
    std::cerr << "the slider of x =  "<< pos<<std::endl;
    isFirst_show =false;
    x_change = pos;
    update();
}

void openglwindow::ychange_slot(float pos)
{
    std::cerr << "the slider of y =  "<< pos<<std::endl;
    isFirst_show =false;
    y_change = pos;
    update();
}

void openglwindow::readPCAP_slot()
{
//    qDebug()<<" the count of pcd = "<<_src_cloud->size()<<endl;
    update();
}

