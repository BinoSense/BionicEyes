# -*- coding: utf-8 -*-
#!/usr/bin/python

from evo_be import *

def main():
    be = BionicEyes(BE_Connect_Type.enumConnect_ImageControl, BE_Connect_DataServerType.enumDeviceServer_Only)

    a=(1.0,0.0,0.0,0.0,0.0,0.0)

    while not be.isBeDataReady():
        print("Data is not ready...")
        time.sleep(1)

    print('Motor init position is')
    print(be.getInitPosition())
    print('Motor_left_pitch move range is')
    print(be.getUpDownLimit(type=MotorType.enumLeftPitch))
    be.goInitPosition()

    if be.haveNeckLinked():
        print("Have neck module linked")
    else:
        print("No neck module linked")

    #cv2.namedWindow('test', cv2.WINDOW_AUTOSIZE)

    while True:
        data = be.getBeData()
        cv2.imshow("test", data.image)
        cv2.waitKey(40)
        #be.setRelativePosition_0(type=MotorType.enumRightPitch, angle=a)

if __name__ == "__main__":
   main()
