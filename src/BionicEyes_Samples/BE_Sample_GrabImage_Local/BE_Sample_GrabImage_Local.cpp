//
// Created by be on 2019/8/27.
//

#include "evo_be_Common.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cmath>
#include <X11/Xlib.h>
#include <X11/keysym.h>

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

using namespace evo_be;

int main()
{
    CBionicEyes *device = device->create();

    // on/off SV function
    device->onoff_SV(false);
    device->onoff_VOR(true);

    float InitAngle[6]={0};

    while (1)
    {
        if (!(device->isBeDataReady()))
        {
            msleep(100);
            cout<<"waiting..."<<endl;
            continue;
        }
        BE_GeneralData data = device->getBeData();
        //cv::Mat temp;
        //cv::resize(data.image, temp, cv::Size(960, 480));

        if(KEY_DOWN(XK_i))
        {
            device->getInitPosition(enumAllMotor, InitAngle);
            cout<<"init pos: ";
            for(int i = 0; i< 6;i++)
                cout<<InitAngle[i]<<" ";
            cout<<endl;
            cout<<"imu value: ";
            for(int i = 0; i< 3;i++)
                cout<<data.imuData[enumEuler][i]<<" ";
            cout<<endl;
        }
        else if(KEY_DOWN(XK_o))
        {
            cout<<"Press o"<<endl;
            cout<<"current pos: ";
            for(int i = 0; i< 6;i++)
                cout<<data.motorData[i]<<" ";
            cout<<endl;
            cout<<"imu value: ";
            for(int i = 0; i< 3;i++)
                cout<<data.imuData[enumEuler][i]<<" ";
            cout<<endl;
            device->goInitPosition(enumAllMotor);
        }
        else if(KEY_DOWN(XK_w))
        {
            cout<<"Press w"<<endl;
            cout<<"current pos: ";
            for(int i = 0; i< 6;i++)
                cout<<data.motorData[i]<<" ";
            cout<<endl;
            cout<<"imu value: ";
            for(int i = 0; i< 3;i++)
                cout<<data.imuData[enumEuler][i]<<" ";
            cout<<endl;
            float moveAngle[6]={10,0,0,10,0,0};
            device->setSpaceAbsPosition(data.motorData, moveAngle, data.imuData[enumEuler]);
        }
        else if(KEY_DOWN(XK_s))
        {
            cout<<"Press s"<<endl;
            cout<<"current pos: ";
            for(int i = 0; i< 6;i++)
                cout<<data.motorData[i]<<" ";
            cout<<endl;
            cout<<"imu value: ";
            for(int i = 0; i< 3;i++)
                cout<<data.imuData[enumEuler][i]<<" ";
            cout<<endl;
            float moveAngle[6]={-10,0,0,-10,0,0};
            device->setSpaceAbsPosition(data.motorData, moveAngle, data.imuData[enumEuler]);
        }
        else if(KEY_DOWN(XK_a))
        {
            cout<<"Press a"<<endl;
            cout<<"current pos: ";
            for(int i = 0; i< 6;i++)
                cout<<data.motorData[i]<<" ";
            cout<<endl;
            cout<<"imu value: ";
            for(int i = 0; i< 3;i++)
                cout<<data.imuData[enumEuler][i]<<" ";
            cout<<endl;
            float moveAngle[6]={0,0,10,0,0,10};
            device->setSpaceAbsPosition(data.motorData, moveAngle, data.imuData[enumEuler]);
        }
        else if(KEY_DOWN(XK_d))
        {
            cout<<"Press d"<<endl;
            cout<<"current pos: ";
            for(int i = 0; i< 6;i++)
                cout<<data.motorData[i]<<" ";
            cout<<endl;
            cout<<"imu value: ";
            for(int i = 0; i< 3;i++)
                cout<<data.imuData[enumEuler][i]<<" ";
            cout<<endl;
            float moveAngle[6]={0,0,-10,0,0,-10};
            device->setSpaceAbsPosition(data.motorData, moveAngle, data.imuData[enumEuler]);
        }
        else if(KEY_DOWN(XK_q))
        {
            cout<<"Press q"<<endl;
            cout<<"current pos: ";
            for(int i = 0; i< 6;i++)
                cout<<data.motorData[i]<<" ";
            cout<<endl;
            cout<<"imu value: ";
            for(int i = 0; i< 3;i++)
                cout<<data.imuData[enumEuler][i]<<" ";
            cout<<endl;
            float moveAngle[6]={0,10,0,0,10,0};
            device->setSpaceAbsPosition(data.motorData, moveAngle, data.imuData[enumEuler]);
        }
        else if(KEY_DOWN(XK_e))
        {
            cout<<"Press e"<<endl;
            cout<<"current pos: ";
            for(int i = 0; i< 6;i++)
                cout<<data.motorData[i]<<" ";
            cout<<endl;
            cout<<"imu value: ";
            for(int i = 0; i< 3;i++)
                cout<<data.imuData[enumEuler][i]<<" ";
            cout<<endl;
            float moveAngle[6]={0,-10,0,0,-10,0};
            device->setSpaceAbsPosition(data.motorData, moveAngle, data.imuData[enumEuler]);
        }
        else if(KEY_DOWN(XK_z))
        {
            cout<<"Press z"<<endl;
            cout<<"current pos: ";
            for(int i = 0; i< 6;i++)
                cout<<data.motorData[i]<<" ";
            cout<<endl;
            cout<<"imu value: ";
            for(int i = 0; i< 3;i++)
                cout<<data.imuData[enumEuler][i]<<" ";
            cout<<endl;
            float moveAngle[6]={0,0,0,0,0,0};
            device->setSpaceAbsPosition(data.motorData, moveAngle, data.imuData[enumEuler]);
        }
        msleep(200);


//        cv::imshow("test", temp);
//        cv::waitKey(40);
    }
    return 0;
}