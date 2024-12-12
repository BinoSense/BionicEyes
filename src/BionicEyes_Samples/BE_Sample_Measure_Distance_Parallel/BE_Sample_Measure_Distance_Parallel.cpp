//
// Created by be on 2024/12/11.
//
#include "evo_be_Common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <evo_be_ImageProcess_Core.h>

#include <evo_be_Common_Internal.h>
#include <iomanip>
using namespace evo_be;
using namespace cv;
cv::Point gazePoint;
void onMouse(int event, int x, int y, int flags, void* param)
{
    if (event == EVENT_LBUTTONDOWN) // 鼠标左键点击事件
    {
        gazePoint.x=x;
        gazePoint.y=y;
    }

}

int main()
{

	// 连接设备
	CBionicEyes *device = device->create ( enumConnect_ImageControl, evo_be::enumDeviceServer_Only );


    CBE_Service *remote_connect;
    remote_connect = remote_connect->create();
    remote_connect->requestToTransFile(device->getBeDevice_Ip_str());

    char *filename_remote = "./stereo_handeye_01.json";
    remote_connect->recvFile(filename_remote,true);
    delete remote_connect;
    // 读取标定数据
    std::shared_ptr<CBE_Calibrator_Parallel> calib = CBE_Calibrator_Parallel::create("stereo_handeye_01.json");

    double scale =0.5;

    BE_GeneralData data;
    device->getBeData(data);
    const int w = data.image[0].cols;
    const int h = data.image[0].rows;

    gazePoint=cv::Point(w*scale/2,h*scale/2);//鼠标点击图像中返回的坐标点
    cv::Vec3d srcPixelCoord = cv::Vec3d(w*scale/2,h*scale/2, 1);
	device->onoff_SV ( false );
	device->onoff_VOR ( false );
	device->goInitPosition ( enumAllMotor );
    gazePoint=cv::Point(w*scale/2,h*scale/2);
    cv::Point lastpoint;
    cv::Rect roi_right;
    double distance;
    while (1)
    {
        if (device->isBeDataReady())
        {
            BE_GeneralData data = device->getBeData();

            cv::Mat imgL_raw,imgR_raw,fullImage;
            resize(data.image[0], imgL_raw, cv::Size(w*scale, h*scale));
            resize(data.image[1], imgR_raw, cv::Size(w*scale, h*scale));
            hconcat(imgL_raw,imgR_raw,fullImage);


            if(gazePoint.x!=lastpoint.x||gazePoint.y!=lastpoint.y)
            {
                RtStereoSPM Rt = calib->getRt(data.imuData);
                cv::Rect roi_left=cv::Rect(gazePoint.x-25,gazePoint.y-25,50,50);
                distance=0;
                if(calib->Calculate_Point_Distance(imgL_raw,imgR_raw,roi_left,roi_right,scale,Rt,distance))
                {

                }
                lastpoint=gazePoint;
            }
            if(distance!=0)
            {
                cv::circle(fullImage, gazePoint, 4, cv::Scalar(0, 0, 255), 3);
                cv::circle(fullImage, cv::Point(roi_right.x + roi_right.width * 0.5+w*scale, roi_right.y + roi_right.height * 0.5), 4, cv::Scalar(0, 0, 255), 3);

                std::stringstream stream;
                stream << std::fixed << std::setprecision(2) << distance;
                cv::putText(fullImage,                                     //target image
                            std::string(stream.str() + " (m)"), //text
                            cv::Point(w*scale/2+20, h*scale/2 - 20),                                                                  //top-left position
                            cv::FONT_HERSHEY_DUPLEX,
                            1.0,
                            CV_RGB(255, 0, 0), //font color
                            3);
            }


            cv::imshow("image", fullImage);
            cv::setMouseCallback("image", onMouse, NULL);

        }
        cv::waitKey(40);
    }
    return 0;

}
