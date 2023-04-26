//
// Created by be on 2019/8/27.
//

#include "evo_be_Common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace evo_be;

int main()
{
    CBionicEyes *device = device->create(enumConnect_Image, enumLocalServer_First, enumDataTransmission_OneByOne);

    int lastFrameId = -1;
    while (1)
    {
        if (device->isBeDataReady())
        {
            BE_GeneralData data = device->getBeData();
            if(data.id != lastFrameId)
            {
                device->triggerDataTransmission();

                //////////////////////////////////////////////////////////
                //////////////////////process!!!//////////////////////////
                cout<<"Receive new image(ID is: "<<data.id<<")"<<endl;
                if(!data.image[enumBoth].empty())
                {
                    cv::Mat temp;
                    cv::resize(data.image[enumBoth], temp, cv::Size(960, 540));
                    cv::imshow("BE_Image", temp);
                    cv::waitKey(1);
                }
                //////////////////////////////////////////////////////////
                //////////////////////////////////////////////////////////
                lastFrameId = data.id;
                continue;
            } else
            {
                msleep(10);
            }
        }
    }
    return 0;
}