#include <iostream>
#include <fstream>
#include <vector>

#include <opencv2/opencv.hpp>

#include <evo_be_Common.h>
#include <evo_be_Console.h>
#include <evo_be_Communication.h>
#include <evo_be_ImageProcess_Core.h>
#include <evo_be_ImageProcess_Core_GPU.h>
#include <evo_be_Utilities.h>

struct BE_FSMState // Finite state machine
{
    evo_be::BE_Connect_Type connect_type;
    evo_be::BE_Connect_DataServerType connect_dataServerType;
    evo_be::BE_Data_TransmissionType data_transmissionType;
    std::string receiver_ipAddr = "";
    std::string server_ipAddr = "";
    evo_be::CBionicEyes *device = nullptr;
    evo_be::CBE_Communication_Ethernet *communication = nullptr;
    evo_be::BE_GeneralData beData;
    int imgMaxHeight = 1536;
    int imgMaxWidth = 2048;
    //int imgMaxHeight = 1080;
    //int imgMaxWidth = 1440;
    double scale = 0.5;
};
void printHelpMessage();
bool cmdOptionParser(int argc, char *argv[], BE_FSMState &be_fsm);
bool initDevice(BE_FSMState &be_fsm, cv::Mat &canvas);

int main(int argc, char *argv[])
{

    if (evo_be::utility::ProgramOptionExists(argc, argv, "--help") || evo_be::utility::ProgramOptionExists(argc, argv, "-h"))
    {
        printHelpMessage();
        std::exit(EXIT_FAILURE);
    }

    cv::Mat canvas;
    BE_FSMState be_fsm;

    if (cmdOptionParser(argc, argv, be_fsm))
        be_fsm.device = be_fsm.device->create(be_fsm.server_ipAddr, be_fsm.connect_type, be_fsm.connect_dataServerType, be_fsm.data_transmissionType);
    else
        be_fsm.device = be_fsm.device->create(be_fsm.connect_type, be_fsm.connect_dataServerType, be_fsm.data_transmissionType);
    if (be_fsm.receiver_ipAddr != "")
    {
        be_fsm.communication = be_fsm.communication->create();
        //be_fsm.communication->connect(be_fsm.receiver_ipAddr, 1919); //communication->bind("*", 1919);
        be_fsm.communication->connect(be_fsm.receiver_ipAddr, 5310); //communication->bind("*", 1919);
        std::cout << "Established communication with be@" << be_fsm.receiver_ipAddr << "\n";
    }
    cv::Mat canvas0;
    if (!initDevice(be_fsm, canvas0))
        std::exit(EXIT_FAILURE);

    // ########### actual computation starts here

    auto device = be_fsm.device;
    //device->onoff_SV(false);
   // device->onoff_VOR(false);
    evo_be::BE_GeneralData &beData = be_fsm.beData;
    const double scale = be_fsm.scale;
    const int w = beData.image[enumBoth].cols / 2;
    const int h = beData.image[enumBoth].rows;

    // dynamic calibration pointer
    std::shared_ptr<evo_be::CBE_Calibrator> calib;
    uchar *encoder = device->getCalibrationInfo();
    calib = calib->create(encoder);
    delete[] encoder;
      // opencv camera and distort matrices. See opencv documentation.
    cv::Matx33d K1 = calib->K(calib->eyeId("Left"), scale);
    auto D1 = calib->D(calib->eyeId("Left"));
    cv::Matx33d K2 = calib->K(calib->eyeId("Right"), scale);
    auto D2 = calib->D(calib->eyeId("Right"));

    // cv::Mat allocation
    // raw
    cv::Mat imgL_raw = beData.image[enumBoth].colRange(0, w), imgR_raw = beData.image[enumBoth].colRange(w, 2 * w);
    // remove distortion
    cv::Mat imgL_undist = imgL_raw.clone(), imgR_blend = imgR_raw.clone();
    // stereo rectifed
    cv::Mat imgL_rect = imgL_raw.clone(), imgR_rect = imgR_raw.clone();
    // disparity and depth (float)
    cv::Mat disp(h, w, CV_32F), depth(h, w, CV_32F);
    // disparity and depth (uchar)
    cv::Mat disp_8u(h, w, CV_8U), depth_8u(h, w, CV_8U);
    // disparity and depth (uchar3) for visualization purpose
    cv::Mat disp_color(h, w, CV_8UC3), depth_color(h, w, CV_8UC3);
    // mask for pixels of invalid depth
    cv::Mat mask(h, w, CV_8UC1);

    // stereomatching pointer
    std::shared_ptr<evo_be::CBE_StereoMatching> sgm = sgm->create(cv::Size(w, h));
    sgm->setWidth(w);
    sgm->setHeight(h);
    // P1 and P2 see Semi-global matching.
    sgm->setUniqueness(0.97);
    sgm->setP1(10);
    sgm->setP2(200);


    int beDataId_last = -1;
    
    while (cv::waitKey(1) != 27)
    {
        auto t0 = GetSysTimeMicros();

        device->getBeData(beData);
        if (beData.isMovingFastly)
        {
            continue;
        }
        if (beData.id != beDataId_last)
        {
            // get relative pose between two eyes
            evo_be::RtStereo Rt = calib->getRt(beData.motorData);


            // stereo rectify
            cv::Mat vR1, vR2, vP1, vP2, vQ;
            cv::Rect vvalidRoi[2];
            cv::Mat vrmap[2][2];
            evo_be::BE_Utilities::virtualStereoRectify(cv::Mat(K1), cv::Mat(K2), cv::Mat(D1), cv::Mat(D2),cv::Size(w, h),cv::Mat(Rt.R_rl), cv::Mat(Rt.t_rl), vR1,vR2,vP1, vP2, vQ,cv::CALIB_ZERO_DISPARITY,1,cv::Size(w, h), vvalidRoi[0], vvalidRoi[1],0,0);
            cv::initUndistortRectifyMap(cv::Mat(K1), cv::Mat(D1), cv::Mat(vR1), cv::Mat(vP1),cv::Size(w, h), CV_16SC2, vrmap[0][0], vrmap[0][1]);
            cv::initUndistortRectifyMap(cv::Mat(K2), cv::Mat(D2), cv::Mat(vR2), cv::Mat(vP2), cv::Size(w, h), CV_16SC2, vrmap[1][0], vrmap[1][1]);
		
            cv::Mat vimgL_rect,vimgR_rect;
            cv::remap(imgL_raw, vimgL_rect, vrmap[0][0], vrmap[0][1], cv::INTER_LINEAR);
            cv::remap(imgR_raw, vimgR_rect, vrmap[1][0], vrmap[1][1], cv::INTER_LINEAR);
		    cv::imshow("stereo rectify left image",vimgL_rect);
	        cv::imshow("stereo rectify right image",vimgR_rect);

            //rectify result virsualization
            cv::Mat canvas;
            canvas.create(h, w * 2, CV_8UC3);
            cv::Mat canvasPart1 = canvas(cv::Rect(0, 0, w, h));
            cv::Mat canvasPart2 = canvas(cv::Rect(w, 0, w, h));
            cv::resize(vimgL_rect, canvasPart1, canvasPart1.size(), 0, 0, cv::INTER_AREA);
            cv::resize(vimgR_rect, canvasPart2, canvasPart2.size(), 0, 0, cv::INTER_AREA);
            for (int j = 0; j < canvas.rows; j += 20)
            	cv::line(canvas, cv::Point(0, j), cv::Point(canvas.cols, j),(j/20)%2==0?cv::Scalar(0, 255, 0):cv::Scalar(0, 0, 255), 1, 8);
            cv::imshow("stereo rectify",canvas);
            canvas.create(h, w * 2, CV_8UC3);

		    // do stereo matching 
            sgm->virtualCalc(vimgL_rect,vimgR_rect,vQ,disp,depth,mask);
            sgm->disp32fTo8u(disp,disp_8u);
            disp_8u.convertTo(disp_8u, CV_8U, 1.0);
            cv::applyColorMap(disp_8u, disp_color, cv::COLORMAP_JET);
		    cv::imshow("disp", disp_color);


            beDataId_last = beData.id;
            continue;
        }
        else
            msleep(10);

        //std::cout << "Loop  time  = " << (GetSysTimeMicros() - t0) * 0.1 << " ms" << std::endl;
    }

    return 0;
}

bool cmdOptionParser(int argc, char *argv[], BE_FSMState &be_fsm)
{
    be_fsm.connect_type = evo_be::enumConnect_ImageControl;
    be_fsm.connect_dataServerType = evo_be::enumLocalServer_First;
    be_fsm.data_transmissionType = evo_be::enumDataTransmission_ASPAP;
    if (evo_be::utility::ProgramOptionExists(argc, argv, "-t"))
        be_fsm.data_transmissionType = evo_be::enumDataTransmission_OneByOne;

    std::string option;

    if (evo_be::utility::ProgramOptionExists(argc, argv, "--connect"))
        option = evo_be::utility::GetProgramOptionAsString(argc, argv, "--connect");
    else if (evo_be::utility::ProgramOptionExists(argc, argv, "-c"))
        option = evo_be::utility::GetProgramOptionAsString(argc, argv, "-c");

    if (option == "c")
        be_fsm.connect_type = evo_be::enumConnect_Control;
    else if (option == "ic" || option == "ci")
        be_fsm.connect_type = evo_be::enumConnect_ImageControl;
    else if (option == "i")
        be_fsm.connect_type = evo_be::enumConnect_Image;

    if (evo_be::utility::ProgramOptionExists(argc, argv, "--server"))
        option = evo_be::utility::GetProgramOptionAsString(argc, argv, "--server");
    else if (evo_be::utility::ProgramOptionExists(argc, argv, "-s"))
        option = evo_be::utility::GetProgramOptionAsString(argc, argv, "-s");

    if (option == "lo")
        be_fsm.connect_dataServerType = evo_be::enumLocalServer_Only;
    else if (option == "lf")
        be_fsm.connect_dataServerType = evo_be::enumLocalServer_First;
    else if (option == "do")
        be_fsm.connect_dataServerType = evo_be::enumDeviceServer_Only;
    else if (option == "df")
        be_fsm.connect_dataServerType = evo_be::enumDeviceServer_First;

    if (evo_be::utility::ProgramOptionExists(argc, argv, "--receiverIP"))
        be_fsm.receiver_ipAddr = evo_be::utility::GetProgramOptionAsString(argc, argv, "--receiverIP");
    else if (evo_be::utility::ProgramOptionExists(argc, argv, "-rip"))
        be_fsm.receiver_ipAddr = evo_be::utility::GetProgramOptionAsString(argc, argv, "-rip");

    if (evo_be::utility::ProgramOptionExists(argc, argv, "--receiverIP"))
        be_fsm.receiver_ipAddr = evo_be::utility::GetProgramOptionAsString(argc, argv, "--receiverIP");
    else if (evo_be::utility::ProgramOptionExists(argc, argv, "-rip"))
        be_fsm.receiver_ipAddr = evo_be::utility::GetProgramOptionAsString(argc, argv, "-rip");

    if (evo_be::utility::ProgramOptionExists(argc, argv, "--serverIP"))
        be_fsm.server_ipAddr = evo_be::utility::GetProgramOptionAsString(argc, argv, "--serverIP");
    else if (evo_be::utility::ProgramOptionExists(argc, argv, "-sip"))
        be_fsm.server_ipAddr = evo_be::utility::GetProgramOptionAsString(argc, argv, "-sip");
    else
        return false;

    std::string pingCMD = "ping -c1 -s1 ";
    pingCMD = pingCMD + be_fsm.server_ipAddr;
    pingCMD = pingCMD + " > /dev/null 2>&1";
    return !(bool)system(pingCMD.c_str());
}

void printHelpMessage()
{
    printf("\
usage: evo_be_App_Visualizer_1*04 \
[--connect <mode>] [--server <mode>] [--receiverIP <ip>] [--serverIP <ip>]\n\
options:\n\
  -h --help      \tShow this help message\n\
  -c --connect   \tConnect mode: i, c, ic\n\
  -s --server    \tServer mode: lo, lf, do, df\n\
  -rip --receiverIP\tIP of receiving device: xxx.xxx.xxx.xxx\n\
  -sip --serverIP  \tIP of server device: xxx.xxx.xxx.xxx\n");
};

bool initDevice(BE_FSMState &be_fsm, cv::Mat &canvas)
{
   // be_fsm.device->onoff_SV(true);
  //  be_fsm.device->onoff_VOR(false);
    be_fsm.device->setImageColor_Transfer(evo_be::enumColor);
    be_fsm.device->setImageColor(evo_be::enumColor);
    auto originalSize = be_fsm.device->getOriginImageResolution();
    be_fsm.imgMaxWidth = originalSize.width;   //2048
    be_fsm.imgMaxHeight = originalSize.height; //1536
    be_fsm.scale = 0.5;                        // set whatever image resolution that you need
    cv::Size be_res(be_fsm.imgMaxWidth * 2 * be_fsm.scale, be_fsm.imgMaxHeight * be_fsm.scale);
    be_fsm.device->setImageResolution_Transfer(be_res);
    be_fsm.device->setImageResolution(be_res);
    be_fsm.device->setImageCompressionQuality(95);
    be_fsm.device->setDataRate_Transfer(25);
    msleep(250);
    while (!(be_fsm.device->isBeDataReady()))
    {
        msleep(40);
        std::cout << "Waiting for data..." << std::endl;
        continue;
    }
   // be_fsm.device->setAbsolutePosition(evo_be::enumAllMotor, std::vector<float>(6, 0.0f).data());
    if (be_fsm.data_transmissionType == evo_be::BE_Data_TransmissionType::enumDataTransmission_OneByOne)
        for (int k = 0; k < 5; k++)
        {
            be_fsm.device->triggerDataTransmission();
            be_fsm.device->getBeData();
            msleep(20);
        }

    be_fsm.device->getBeData(be_fsm.beData);
    canvas = cv::Mat(1080, 1920, CV_8UC3);
    canvas.setTo(cv::Scalar(0, 0, 0));
    return true;
}
