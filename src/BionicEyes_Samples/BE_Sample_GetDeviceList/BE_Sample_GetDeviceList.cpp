//
// Created by be on 2019/8/27.
//

#include "evo_be_Common.h"
#include "evo_be_Communication.h"
#include "evo_be_Common_Internal.h"

using namespace evo_be;

int main()
{
    CBE_Communication_Ethernet_Special *device_ipAddr_Service;
    device_ipAddr_Service = device_ipAddr_Service->create(enumEth_Req);

    //*************************************************************************
    std::vector<BE_IpInfo> deviceList;

    while (!device_ipAddr_Service->requestBeIpAddress(deviceList)) {
        printf("No BionicEye device detected. Retrying.....\n");
        msleep(2000);
    }

    printf("BionicEye device detected(count == %d)!!!\n", deviceList.size());

    for (int i = 0; i < deviceList.size(); i++)
    {
        printf("%d: %s %s\n", i, deviceList[i].deviceName, deviceList[i].ipAddrStr);
    }

    return 0;
}