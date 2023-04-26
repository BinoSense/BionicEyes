#include "evo_be_Common.h"

using namespace evo_be;

struct BeCmdOptions
{
    bool openNetworkService;
    int dataDefaultFrameRate;
};

int main(int argc, char *argv[])
{
    BeCmdOptions opts;

    opts.dataDefaultFrameRate = 100;     // max 100fps
    opts.openNetworkService = false;    // whether use remote control function

    CBionicEyes *device = CBionicEyes::create(opts.openNetworkService, opts.dataDefaultFrameRate);
    msleep(4000);
    device->onoff_VOR(false);

    float ampli = 38;
    int MaxStep = 380;
    int countID = MaxStep / 2;
    bool moveFlag[6]={0,0,0,1,1,1};
    float moveAngle[6] = {0};


    while (1)
    {
        if(device->isBeDataReady())
        {
            BE_GeneralData dataT;
            device->getBeData(dataT);
            cout<<dataT.motorData[3]<<"  "<<dataT.motorData[4]<<"  "<<dataT.motorData[5]<<"  "<<dataT.imuData[enumQuat][0]<<"  "<<dataT.imuData[enumQuat][1]<<"  "<<dataT.imuData[enumQuat][2]<<"  "<<dataT.imuData[enumQuat][3]<<"  "<<dataT.imuData[enumMag][0]<<"  "<<dataT.imuData[enumMag][1]<<"  "<<dataT.imuData[enumMag][2]<<"  "<<dataT.imuData[enumMag][3]<<endl;

            float amp_now = ampli * std::sin(2.f * M_PI / (float)MaxStep * (float)(countID++ % MaxStep));
            float moveAngle[6] = {0, 0, 0, amp_now, amp_now, amp_now};
            device->setAbsolutePosition(moveFlag, moveAngle);

        }
        msleep(10);
    }
    return 0;
}