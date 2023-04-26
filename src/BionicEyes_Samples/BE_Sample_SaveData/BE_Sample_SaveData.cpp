//
// Created by be on 2019/8/27.
//

#include "evo_be_Common.h"
#include <opencv2/core.hpp>

using namespace evo_be;

int main()
{
    CBionicEyes *device = device->create(enumConnect_ImageControl, enumDeviceServer_First);

    // set network transfer image size
//    device->setImageResolution_Transfer(cv::Size(2880,1080));
//    device->setImageCompressionQuality(95);
    device->setDataRate_Transfer(5);

    // Waiting to take effect
    msleep(2000);

    //// start save be_data stream, using default folder
    device->saveBeData();

    ///// snap data example
    int i = 0;
    while(i < 50)
    {
        // Snap be_data every 0.2 second, using default folder
        device->snapBeData("This is snap Sample", Be_Io_Path + "snapSave");
        msleep(200);
        i++;
    }
    ///// snap data example in new folder
    device->stopSnapBeData();
    msleep(50);
    while(i < 100)
    {
        // Snap be_data every 0.2 second, using default folder
        device->snapBeData("This is snap Sample", Be_Io_Path + "newSnapSave");
        msleep(200);
        i++;
    }

    //// record data example
    while(i < 150)
    {
        if (device->isBeDataReady()) {
            BE_GeneralData data = device->getBeData();

            if(i % 5 == 0)
                device->recordBeData(data, "This is record Sample", Be_Io_Path + "recordSave");
        }
        msleep(100);
        i++;
    }
    ///// record data example in new folder
    device->stopRecordBeData();
    msleep(50);
    while(i < 200)
    {
        if (device->isBeDataReady()) {
            BE_GeneralData data = device->getBeData();

            if(i % 5 == 0)
                device->recordBeData(data, "This is record Sample", Be_Io_Path + "NewRecordSave");
        }
        msleep(100);
        i++;
    }

    //// stop saving be_data stream
    device->saveBeData(0);
    return 0;
}