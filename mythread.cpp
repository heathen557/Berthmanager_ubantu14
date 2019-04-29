#include "mythread.h"
#include <QDebug>
#include <QMutex>


static int _id = 0;
char _fname[20];
CloudPtr _src_cloud(new Cloud);
int count = 1;

static int saveIndex = 0;
 
MyThread::MyThread()
{
    isStop = false;


    qDebug()<<"readPCAP_slot() has coming";


}
 
void MyThread::closeThread()
{
    isStop = true;
}
 
void MyThread::run()
{
    qDebug()<<"the thread begin and ip = "<<lidar_IP<<"    port = "<<lidar_port;


//    //播放本地点云文件
//    HesaiLidarSDK psdk(
//            //8080				/* lidar data port */,
//            //8308				/* gps data port */,
//            std::string("debugx.pcap"),
//            std::string("correction.csv")	/* calibration file of lidar */,
//            HESAI_LIDAR_RAW_DATA_STRCUT_SINGLE_RETURN/* Return Mode: Single Return data structure */,
//            40				/* laser counter */,
//            HESAI_LIDAR_PCL_DATA_TYPE_REDUCED/* pcl data alignment */,
//            lidarCallback 			/* point cloud data call back */
//           // gpsCallback 			/* gps data callback */,
//    );


//    HesaiLidarSDK psdk(
//    2368				/* lidar data port */,
//    10110				/* gps data port */,
//    std::string("correction.csv")	/* calibration file of lidar */,
//    lidarCallback 			/* point cloud data call back */,
//    gpsCallback 			/* gps data callback */,
//    HESAI_LIDAR_RAW_DATA_STRCUT_DUAL_RETURN/* Return Mode: Dual Return data structure */,
//    40				/* laser counter */,
//    HESAI_LIDAR_PCL_DATA_TYPE_REDUCED/* pcl data alignment */
//    );

//    Pandar40PSDK pandar40p(std::string("192.168.1.201"),
//                    2368, 10110, lidarCallback, gpsCallback,
//                    13500, 0, std::string("hesai40"));


    //play local .pcd file
    int i=0;
    while (1) {
        i++;
        QString fileName = "12-25/"+QString::number(i)+".pcd";
        pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_per(new pcl::PointCloud <pcl::PointXYZI>);
        if(-1 == pcl::io::loadPCDFile(fileName.toStdString(),*cloud_per))
        {
            std::cout<<"read pcd error!~~~~"<<std::endl;
            break;
        }
        _src_cloud->resize(cloud_per->points.size());
        _src_cloud->points[i].x = cloud_per->points[i].x;
        _src_cloud->points[i].y = cloud_per->points[i].y;
        _src_cloud->points[i].z = cloud_per->points[i].z;
        _src_cloud->points[i].intensity = cloud_per->points[i].intensity;

        usleep(1000);
    }



    Pandar40PSDK pandar40p(std::string("192.168.1.201"),
                    2368, 10110, lidarCallback, gpsCallback,
                    6000, 0, std::string("hesai40"));


    pandar40p.Start();

    while (1)
    {
        if(isStop)
            return;
        sleep(1);
    }




}

void MyThread::lidarCallback(boost::shared_ptr<PPointCloud> cld, double timestamp)
{
        printf("lidar: time %lf , points %d\n", timestamp , cld->points.size(), cld->size());
        _src_cloud->width = cld->width;
        _src_cloud->width = cld->height;
        _src_cloud->resize(cld->points.size());
        for (int i = 0; i < cld->points.size(); ++i)
        {
            _src_cloud->points[i].x = cld->points[i].x;
            _src_cloud->points[i].y = cld->points[i].y;
            _src_cloud->points[i].z = cld->points[i].z;
            _src_cloud->points[i].intensity = cld->points[i].intensity;
        }


        // save pcd file
        saveIndex++;
        QString index_str = "12-25/" + QString::number(saveIndex) + ".pcd";
        pcl::io::savePCDFileASCII(index_str.toStdString(),*_src_cloud);

}

void MyThread::gpsCallback(int timestamp)
{

}


