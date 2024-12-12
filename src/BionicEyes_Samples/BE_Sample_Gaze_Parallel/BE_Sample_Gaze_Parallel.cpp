//
// Created by be on 2024/12/11.
//
#include "evo_be_Common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


#include <evo_be_ImageProcess_Core.h>

#include <evo_be_Common_Internal.h>

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


    while (1)
    {
        if (device->isBeDataReady())
        {
            BE_GeneralData data = device->getBeData();

            cv::Mat imgL_raw;
            resize(data.image[0], imgL_raw, cv::Size(w*scale, h*scale));
            if(gazePoint.x!=w*scale/2&&gazePoint.y!=h*scale/2)
            {
                RtStereoSPM Rt = calib->getRt(data.imuData);
                cv::Vec3d dstPixelCoordLeft = cv::Vec3d(gazePoint.x , gazePoint.y , 1);
                auto targetR_L = calib->invKin_Pix2q(Rt, srcPixelCoord, dstPixelCoordLeft, scale, evo_be::CameraIndex::enumLeft);
                auto invLeft = calib->invKin_Rot2q(targetR_L, evo_be::CameraIndex::enumLeft);
                std::vector<float> invMotor = {0, 0, 0, (float)invLeft(0), (float)invLeft(1), (float)invLeft(2)};

                device->setAbsolutePosition(evo_be::enumAllMotor, invMotor.data(), evo_be::BE_MovePatternType::enumMovePattern_Saccade);
                gazePoint=cv::Point(w*scale/2,h*scale/2);
            }


            cv::circle(imgL_raw, cv::Point(imgL_raw.cols/2, imgL_raw.rows/2), 3, cv::Scalar(0, 0, 255), 3);

            cv::imshow("image_left", imgL_raw);
            //cv::imshow("image_right", imgR_raw);
            cv::setMouseCallback("image_left", onMouse, NULL);

        }
        cv::waitKey(40);
    }
    return 0;

}
