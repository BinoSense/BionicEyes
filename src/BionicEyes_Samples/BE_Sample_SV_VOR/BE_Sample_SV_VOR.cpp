//
// Created by be on 2019/8/27.
//

#include "evo_be_Common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <X11/Xlib.h>
#include <X11/keysym.h>

using namespace evo_be;

// detect keyboard press, keyValue_Linux can be found in "/usr/include/X11/keysymdef.h"
// for example: XK_Shift_L, XK_A, XK_a, XK_0
static Display *be_keypress_dpy = XOpenDisplay(NULL);
static bool KEY_DOWN(KeySym keyValue_Linux)
{
    char keys_return[32];
    XQueryKeymap(be_keypress_dpy, keys_return);
    KeyCode kc2 = XKeysymToKeycode(be_keypress_dpy, keyValue_Linux);
    return !!(keys_return[kc2 >> 3] & (1 << (kc2 & 7)));
}

int main()
{
    CBionicEyes *device = device->create(enumConnect_ImageControl);

    device->onoff_VOR(false);
    device->goInitPosition(enumAllMotor);
    device->onoff_SV(false);
    msleep(500);
    device->onoff_VOR(true);

    bool flag_SV = false,
    flag_VOR = true;

    device->setImageResolution_Transfer(cv::Size(1920,540));
    device->setImageResolution(cv::Size(1920,540));
    device->setImageColor_Transfer(enumColor);
    device->setImageCompressionQuality(95);
//    device->setImageColor(enumMono);
    //cout<< device->getMaxImageFrameRate()<<endl;
    device->setDataRate_Transfer(25);

    cout<< "Bionic Eyes SV_VOR Sample:" <<endl
    <<"    Press S to switch ON/OFF of SV"<<endl
    <<"    Press V to Switch ON/OFF of VOR"<<endl
    <<"    Press I to move to init position"<<endl;

    while (1)
    {
        if(KEY_DOWN(XK_S))
        {
            flag_SV = !flag_SV;
            device->onoff_SV(flag_SV);
        }
        if(KEY_DOWN(XK_V))
        {
            flag_VOR = !flag_VOR;
            device->onoff_VOR(flag_VOR);
        }
        if(KEY_DOWN(XK_I))
        {
            device->goInitPosition(enumAllMotor);
        }
        if (device->isBeDataReady())
        {
            BE_GeneralData data = device->getBeData();
            cv::Mat temp;
            //cv::resize(data.image, temp, cv::Size(1920, 540));
            cv::imshow("VOR_Sample", data.image[enumBoth]);
        }
        cv::waitKey(40);
    }
    return 0;
}