//
// Created by be on 2019/8/27.
//
#include "evo_be_Common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgproc/types_c.h>
#include <cmath>

#include <evo_be_ImageProcess_Core.h>
#include <evo_be_ImageProcess_Core_GPU.h>
#include <evo_be_Utilities.h>
#include <evo_be_Common_Internal.h>

using namespace evo_be;

int main()
{

	// 连接设备
	CBionicEyes *device = device->create ( enumConnect_ImageControl, evo_be::enumDeviceServer_Only );


    CBE_Service *remote_connect;
    remote_connect = remote_connect->create();
    remote_connect->requestToTransFile(device->getBeDevice_Ip_str());

    char *filename_remote = "./stereo_handeye_01.json";
    remote_connect->recvFile(filename_remote,true);

    // 读取标定数据
    std::shared_ptr<CBE_Calibrator_Parallel> calib = CBE_Calibrator_Parallel::create("stereo_handeye_01.json");

    double scale =0.5;
    cv::Mat K1 = calib->K(0,scale);
    cv::Mat K2 = calib->K(1,scale);
    cv::Mat D1 = calib->D(0);
    cv::Mat D2 = calib->D(1);

	device->onoff_SV ( false );
	device->onoff_VOR ( false );
	device->goInitPosition ( enumAllMotor );
	device->setImageColor ( enumMono );

	// 第一组图获取图像大小信息
	while ( !device->isBeDataReady() )
	{
		usleep ( 10000 );
	}
	BE_GeneralData beData0 = device->getBeData();
	const int w = (int)( beData0.image[0].cols*scale) ;
	const int h = (int)( beData0.image[0].rows*scale) ;
	cv::Mat imgL_raw = beData0.image[0];
	cv::Mat imgR_raw = beData0.image[1];
	// stereo rectifed
	cv::Mat vimgL_rect = imgL_raw.clone(), vimgR_rect = imgR_raw.clone();
	// disparity and depth (float)
	cv::Mat disp ( h, w, CV_32F ), depth ( h, w, CV_32F );
	cv::Mat disp_color ( h, w, CV_8UC1 ), depth_color ( h, w, CV_8UC1 );
	// mask for pixels of invalid depth
	cv::Mat mask ( h, w, CV_8UC1 );

    std::shared_ptr<CBE_StereoRectify> rectify = CBE_StereoRectify::create();

	// STEREO MATCHING
	std::shared_ptr<CBE_StereoMatchingInterface> matchinger = CBE_StereoMatchingInterface::create("/usr/Evo_BionicEyes/dependencies/onnxruntime/models/model_float32_640.onnx");
	matchinger->init();



	while ( 1 )
	{
		if ( device->isBeDataReady() )
		{

			BE_GeneralData data= device->getBeData();

// 获取外参
            RtStereoSPM Rt = calib->getRt(data.imuData);
// 获取图像
			imgL_raw = data.image[0];
			imgR_raw = data.image[1];
			if ( imgL_raw.type() == CV_8UC3 )
				cv::cvtColor ( imgL_raw, imgL_raw, cv::COLOR_BGR2GRAY );
			if ( imgR_raw.type() == CV_8UC3 )
				cv::cvtColor ( imgR_raw, imgR_raw, cv::COLOR_BGR2GRAY );
            cv::resize(imgL_raw,imgL_raw,cv::Size(w,h));
			cv::resize(imgR_raw,imgR_raw,cv::Size(w,h));

            cv::medianBlur(imgL_raw,imgL_raw,9);
            cv::medianBlur(imgR_raw,imgR_raw,9);
            

            rectify->stereoRectify(imgL_raw,imgR_raw,K1,D1,K2,D2,Rt.R_rl, Rt.t_rl,vimgL_rect,vimgR_rect);
            cv::Mat H = rectify->getH();
            cv::Mat vQ = rectify->getQ();
//显示立体校正效果
        
			cv::Mat canvas;
			cv::hconcat ( vimgL_rect,vimgR_rect,canvas );
			cv::resize ( canvas,canvas,canvas.size() /2 );
			cv::cvtColor ( canvas, canvas,cv::COLOR_GRAY2BGR );
			for ( int j = 0; j < canvas.rows; j += 20 )
				cv::line ( canvas, cv::Point ( 0, j ), cv::Point ( canvas.cols, j ), ( j/20 ) %2==0?cv::Scalar ( 0, 255, 0 ) :cv::Scalar ( 0, 0, 255 ), 1, 8 );
			cv::namedWindow ( "rectify",0 );
			cv::imshow ( "rectify",canvas );

// 计算视差

			float minDepth = 200.f;
			float maxDepth = 2000.f;
            matchinger->inference(vimgL_rect,vimgR_rect,vQ,H,minDepth,maxDepth,disp,disp_color,mask);
		
			cv::namedWindow ( "disparity",cv::WINDOW_NORMAL |  cv::WINDOW_KEEPRATIO );
			cv::imshow ( "disparity",disp_color );

			char s =cv::waitKey ( 1 );
			if ( s=='q' )
				return 0;
		}

	}
	return 0;
}
