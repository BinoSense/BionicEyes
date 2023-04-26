//
// Created by ok_linux on 2021/10/22.
//

#include "evo_be.h"
#include <string>
#include <opencv2/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/imgproc/types_c.h>

using namespace std;

bool infraImageTrans(cv::Mat input, cv::Mat& infraImage, cv::Mat& tempImage)
{
    if(input.empty())
        return false;
    int width = input.cols / 2, height = input.rows;
    cv::Mat infraImage_ = cv::Mat(height, width, CV_8UC2);
    tempImage = cv::Mat(height, width, CV_16UC1);

    for(int i =0; i < height; i++)
    {
        memcpy(tempImage.data+i*width*2,input.data+i*width*4, width*2);
        memcpy(infraImage_.data+i*width*2, input.data+i*width*4+width*2, width*2);
    }
    cv::cvtColor(infraImage_, infraImage, cv::COLOR_YUV2RGB_UYVY, 3);
    return true;
}

uchar* readImage(string folderPath, int id)
{
    uchar* imcode = new uchar[1310728];//1310720
    FILE *picFile;
    std::string picName = folderPath + "Cam_3/" +std::to_string(id) + ".beInfra";
    if ((picFile = fopen(picName.c_str(), "r")) == NULL)
    {
        std::cout << "Warning: No_"<< id << " beInfra file not exists! Will pass it..." << std::endl;
        return NULL;
    }

    fseek(picFile, 0L, SEEK_END);
    int len = ftell(picFile);
    rewind(picFile);

    if(1310720 != len)
    {
        std::cout<< "Warning: Reading image doesn't fit to record! Will pass it..."<<std::endl;
        return NULL;
    }

    if (fread(imcode, 1, len, picFile) != len)
    {
        std::cout << "Warning: No_"<< id << " beInfra file read fail! Will pass it..." << std::endl;
        return NULL;
    }
    fclose(picFile);

    return imcode;
}

int main()
{
    cv::Mat infraImage, tempImage;

    string path = "/usr/Evo_BionicEyes/io_path/snapSave_T_T_T_T_T/";

    uchar* imageData = readImage(path, 745039);

    cv::Mat infCamTemp = cv::Mat(cvSize(1280, 512), CV_8UC2, imageData);

    infraImageTrans(infCamTemp, infCamTemp, tempImage);

    delete[] imageData;

    imshow("test", infCamTemp);
    cv::waitKey(0);
    return 0;

}