#ifndef MYTHREAD_H
#define MYTHREAD_H
 
#include <QThread>
#include "pandar40p_sdk/pandar40p_sdk.h"
#include <pcl/io/pcd_io.h>
//#include <pcl/visualization/pcl_visualizer.h>

//QString lidar_IP;
//QString lidar_port;

typedef pcl::PointXYZI PointType;
typedef pcl::PointCloud<PointType> Cloud;
typedef Cloud::Ptr CloudPtr;

 
class MyThread : public QThread
{
public:
    MyThread();
    void closeThread();
    static void lidarCallback(boost::shared_ptr<PPointCloud> cld, double timestamp);
    static void gpsCallback(int timestamp);

     QString lidar_IP;
     QString lidar_port;
 
protected:
    virtual void run();
 
private:
    volatile bool isStop;       //isStop是易失性变量，需要用volatile进行申明
};
 
#endif // MYTHREAD_H

