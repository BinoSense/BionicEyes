//
// Created by be on 2019/8/27.
//

#include "evo_be_Common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>

using namespace evo_be;

int main()
{
    CBionicEyes *device = device->create(enumConnect_ImageControl, evo_be::enumDeviceServer_Only);

    // on/off SV function
    device->onoff_SV(false);

//    device->setImageResolution_Transfer(cv::Size(1920,540));
//    device->setImageResolution(cv::Size(3840,1080));
//    device->setImageColor_Transfer(enumColor);
//    device->setImageCompressionQuality(95);
////    device->setImageColor(enumMono);
//    //cout<< device->getMaxImageFrameRate()<<endl;
//    device->setDataRate_Transfer(25);
    while (1)
    {
        if (device->isBeDataReady())
        {
            BE_GeneralData data = device->getBeData();
            for(int i = 0; i< MAXCAMERASInDEVICE; i++)
            {
                if(data.imageFlag[i])
                {
                    if(i == 3)
                    {
                        cv::Mat infraImage, tempImage;

                        if(device->infraImageTrans(data, infraImage, tempImage))
                        {
                            cv::imshow("BE_Image_" + to_string(i), infraImage);
                        }
                    } else
                    {
                        cv::Mat temp;
                        cv::resize(data.image[i], temp, cv::Size(data.image[i].cols >> 1, data.image[i].rows >> 1));
                        cv::imshow("BE_Image_" + to_string(i), temp);
                    }
                }
            }
        }
        cv::waitKey(40);
    }
    return 0;
}