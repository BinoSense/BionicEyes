# -*- coding: utf-8 -*-
#!/usr/bin/python
# Written by openking (2019/09/01)

import sys
sys.path.append('/opt/ros/kinetic/lib/python2.7/dist-packages')
sys.path.remove('/opt/ros/kinetic/lib/python2.7/dist-packages')

import os
import time
import string
import cv2
import threading
import struct
import platform

from ctypes import *
from enum import Enum, unique
import numpy as np

# Motor type
try:
    @unique
    class MotorType(Enum):
        enumRightPitch = 0
        enumRightRoll = 1
        enumRightYaw = 2
        enumLeftPitch = 3
        enumLeftRoll = 4
        enumLeftYaw = 5
        enumAllMotor = 6
        enumNoMotor = 7
except ValueError as e:
    print(e)

# Neck Motor type
try:
    @unique
    class MotorType_Neck(Enum):
        enumNeckPitch = 0
        enumNeckRoll = 1
        enumNeckYaw = 2
        enumNeckAllMotor = 3
        enumNeckNoMotor = 4
except ValueError as e:
    print(e)

# IMU Data Type
try:
    @unique
    class IMUDataType(Enum):
        enumAcc = 0
        enumGyo = 1
        enumMag = 2
        enumEuler = 3
        enumQuat = 4
except ValueError as e:
    print(e)

# Axis type
try:
    class AxisType(Enum):
        enumPitch = 0
        enumRoll = 1
        enumYaw = 2
        enumX = 0
        enumY = 1
        enumZ = 2
        enumA = 0
        enumB = 1
        enumC = 2
        enumD = 3
except ValueError as e:
    print(e)

try:
    @unique
    class CameraIndex(Enum):
        enumLeft = 0
        enumRight = 1
        enumBoth = 2
except ValueError as e:
    print(e)

try:
    @unique
    class ImageColorType(Enum):
        enumMono = 0
        enumColor = 1
except ValueError as e:
    print(e)

try:
    @unique
    class BE_Connect_Type(Enum):
        enumNoConnect_Local = 0
        enumConnect_Control = 1
        enumConnect_Image = 2
        enumConnect_ImageControl = 3
except ValueError as e:
    print(e)

try:
    @unique
    class BE_Connect_DataServerType(Enum):
        enumLocalServer_First = 0
        enumDeviceServer_First = 1
        enumDeviceServer_Only = 2
        enumLocalServer_Only = 3
except ValueError as e:
    print(e)

try:
    @unique
    class BE_MovePatternType(Enum):
        enumMovePattern_Saccade = 0
        enumMovePattern_SmoothPursuit = 1
except ValueError as e:
    print(e)

try:
    @unique
    class BE_MoveBaseType(Enum):
        enumMoveBase_Independent = 0
        enumMoveBase_LeftEye = 1
        enumMoveBase_RightEye = 2
except ValueError as e:
    print(e)

try:
    @unique
    class MotorPIDType(Enum):
        enumInnerLoopP_VOR = 0
        enumInnerLoopI_VOR = 1
        enumInnerLoopD_VOR = 2
        enumOuterLoopP_VOR = 3
        enumOuterLoopI_VOR = 4
except ValueError as e:
    print(e)

try:
    @unique
    class SVType(Enum):
        enumGlobalSV = 0
        enumCentreSV = 1
        enumAreaSV = 2
except ValueError as e:
    print(e)

# Bionic Eye ip information structure
class BE_IpInfo(Structure):
    _fields_ = [("deviceName",c_char * 30),
            ("ipAddrInt", c_uint),
            ("ipAddrStr", c_char * 20)
            ]

# Bionic Eye basic data structure, include all synchronized data
class BE_ImageGeneralInfo_Data(Structure):
    _pack_ = 1
    _fields_ = [("id",c_uint),
            ("timeStamp", c_uint),
            ("deviceName", c_char * 30),
            ("ipAddrInt", c_uint),
            ("ipAddrStr", c_char * 20),
            ("colorType", c_int),
            ("cameraIndex", c_int),
            ("imageLen", c_longlong),
            ("imageTranQualityLevel", c_int),
            ("imageSize", c_int * 2),       # width, height
            ("imageSize_Trans", c_int * 2),       # width, height
            ("motorData", c_float * 6),
            ("imuData", c_float * 20),
            ("gpsData", c_double * 20),
            ("isMovingFastly", c_bool)
            ]

class BE_ImageGeneralInfo():
    """This is BionicEyes basic data structure in python"""
    def __init__(self):
        self.image = 0

        self.id = 0
        self.timeStamp = 0
        self.deviceName = np.char.array(1, 30)
        self.ipAddrInt = 0
        self.ipAddrStr = np.char.array(1, 20)

        self.colorType = ImageColorType(0)
        self.cameraIndex = CameraIndex(0)
        self.imageLen = 0
        self.imageTranQualityLevel = 0
        self.imageSize = (0,0)
        self.imageSize_Trans = (0,0)

        self.motorData = np.arange(6).reshape(1, 6)
        self.imuData = np.arange(20).reshape(5, 4)
        self.gpsData = np.arange(4).reshape(1, 4)

        self.isMovingFastly = 0


class BionicEyes(threading.Thread):
    """This is BionicEyes core function in python"""
    def __init__(self,  type:BE_Connect_Type = BE_Connect_Type.enumNoConnect_Local, serverType:BE_Connect_DataServerType = BE_Connect_DataServerType.enumLocalServer_First, ipAddr:string='0.0.0.0'):
        sysstr = platform.system()
        sysversion = platform.version()
        if(sysstr =="Windows"):
            os.chdir(os.getcwd()+'/../../lib')
            self.BElib = cdll.LoadLibrary(os.getcwd()+'/../bin/BionicEyes.dll')
        elif(sysstr == "Linux"):
            os.chdir('/usr/Evo_BionicEyes/lib')
            if(sysversion.find("18.04") >= 0):
                self.BElib = cdll.LoadLibrary('/usr/Evo_BionicEyes/lib/libevo_be_BionicEyes_shared_1804.so')
            elif(sysversion.find("16.04")):
                self.BElib = cdll.LoadLibrary('/usr/Evo_BionicEyes/lib/libevo_be_BionicEyes_shared_1604.so')
            else:
                print("Unsupported Linux Version!!!")
                exit()
        else:
            print("Unsupported system!!!")
            exit()

        if type == BE_Connect_Type.enumNoConnect_Local:
            self.BElib.create_py_local()
        elif ipAddr == '0.0.0.0':
            self.BElib.create_py_remote_0(type.value, serverType.value)
        else:
            self.BElib.create_py_remote_1(ipAddr, type.value, serverType.value)

        # self.BElib.getBeDevice_Ip_py.restype = c_uint32
        # self.ipAddr = IP(self.BElib.getBeDevice_Ip_py())
        # self.port = 5120
        #
        # self.context = zmq.Context()
        # self.socket = self.context.socket(zmq.SUB)
        # self.socket.connect("tcp://%s:%d" % (self.ipAddr, self.port))
        # self.socket.setsockopt(zmq.SUBSCRIBE,b'1000005')

        # self.jpeg = TurboJPEG()

        # threading.Thread.__init__(self)
        return

    def run(self):
        print("Start receive be data...\n")
        databuf = BE_ImageGeneralInfo()
        while True:
            recvData = self.socket.recv(copy = False)
            data = struct.unpack("8cIIIIq2I6f20f1250000B", recvData.bytes)
            databuf.id = data[8]
            databuf.timeStamp = data[9]
            databuf.colorType = ImageColorType(data[10])
            databuf.cameraIndex = CameraIndex(data[11])
            databuf.imageLen = data[12]
            databuf.imageSize = [data[13],data[14]]
            databuf.motorData = data[15:21]
            databuf.imuData = data[21:41]

            databuf.image = self.jpeg.decode(jpeg_buf=bytes(data[41:41+databuf.imageLen]))
            cv2.imshow('image',databuf.image)
            cv2.waitKey(1)
        return

    def isBeDataReady(self):
        return self.BElib.isBeDataReady_py()

    def getBeData(self):

        bedata_c = BE_ImageGeneralInfo_Data()
        bedata = BE_ImageGeneralInfo()
        self.BElib.getBeData_py.restype = POINTER(c_ubyte)
        imgData = self.BElib.getBeData_py(byref(bedata_c))

        bedata.id = bedata_c.id
        bedata.timeStamp = bedata_c.timeStamp
        bedata.deviceName = bedata_c.deviceName
        bedata.ipAddrInt = bedata_c.ipAddrInt
        bedata.ipAddrStr = bedata_c.ipAddrStr

        bedata.colorType = ImageColorType(bedata_c.colorType)
        bedata.cameraIndex = CameraIndex(bedata_c.cameraIndex)
        bedata.imageLen = bedata_c.imageLen
        bedata.imageTranQualityLevel = bedata_c.imageTranQualityLevel
        bedata.imageSize = (bedata_c.imageSize[0], bedata_c.imageSize[1])
        bedata.imageSize_Trans = (bedata_c.imageSize_Trans[0], bedata_c.imageSize_Trans[1])

        bedata.motorData = bedata_c.motorData
        bedata.imuData = bedata_c.imuData
        bedata.gpsData = bedata_c.gpsData

        bedata.isMovingFastly = bedata_c.isMovingFastly
        # time_start=time.time()
        # print(bedata.imageSize[1], bedata.imageSize[0])
        bedata.image = np.array(np.fromiter(imgData, dtype=np.uint8, count=bedata.imageSize[1] * bedata.imageSize[0] * 3))
        bedata.image = bedata.image.reshape((bedata.imageSize[1], bedata.imageSize[0], 3))

        # time_end=time.time()
        # print('time cost',time_end-time_start,'s')
        # bedata.image = np.ndarray(buffer=(c_ubyte * bedata_c.imageSize[0] * bedata_c.imageSize[1] * 3).from_address(imgData),
        #                           dtype=np.uint8,
        #                           shape=(bedata_c.imageSize[1], bedata_c.imageSize[0], 3))

        # bedata.image = np.ndarray(buffer=(c_ubyte * bedata_c.imageSize[0] * bedata_c.imageSize[1] * 3).from_address(imgData),
        #                           dtype=np.uint8,
        #                           shape=(bedata_c.imageSize[1], bedata_c.imageSize[0], 3))


        # cv2.imshow('image', bedata.image)
        # cv2.waitKey(1)
        return bedata

    ## get eye motor uplimit and downlimit, return (downLimit, upLimit)
    def getUpDownLimit(self, type:MotorType):
        down = c_float(0)
        up = c_float(0)
        self.BElib.getUpDownLimit_py(type.value, byref(up), byref(down))
        return (down.value, up.value)

    ## get neck motor uplimit and downlimit, return (downLimit, upLimit)
    def getUpDownLimit_Neck(self, type:MotorType_Neck):
        down = c_float(0)
        up = c_float(0)
        self.BElib.getUpDownLimit_Neck_py(type.value, byref(up), byref(down))
        return (down.value, up.value)

    ## Set Eye Position (angle:float.Size = 6)
    def setAbsolutePosition_0(self,
                              type:MotorType,
                              angle,
                              moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade,
                              movebaseType:BE_MoveBaseType=BE_MoveBaseType.enumMoveBase_Independent):
        float_arr = (c_float*6)()
        for i in range(0,6):
            float_arr[i] = angle[i]
        self.BElib.setAbsolutePosition_py_0(type.value, float_arr, moveType.value, movebaseType.value)
        return

    ## Set Eye Position (angle:float.Size = 6   activeFlag:bool.Size = 6)
    def setAbsolutePosition_1(self,
                              activeFlag,
                              angle,
                              moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade,
                              movebaseType:BE_MoveBaseType=BE_MoveBaseType.enumMoveBase_Independent):
        float_arr = (c_float*6)()
        bool_arr = (c_bool*6)()
        for i in range(0,6):
            float_arr[i] = angle[i]
            bool_arr[i]=activeFlag[i]
        self.BElib.setAbsolutePosition_py_1(activeFlag, float_arr, moveType.value, movebaseType.value)
        return

    ## Set Eye Position (angle:float.Size = 6)
    def setRelativePosition_0(self,
                              type:MotorType,
                              angle,
                              moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade,
                              movebaseType:BE_MoveBaseType=BE_MoveBaseType.enumMoveBase_Independent):
        float_arr = (c_float*6)()
        for i in range(0,6):
            float_arr[i] = angle[i]
        self.BElib.setRelativePosition_py_0(type.value, float_arr, moveType.value, movebaseType.value)
        return

    ## Set Eye Position (angle:float.Size = 6   activeFlag:bool.Size = 6)
    def setRelativePosition_1(self,
                              activeFlag,
                              angle,
                              moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade,
                              movebaseType:BE_MoveBaseType=BE_MoveBaseType.enumMoveBase_Independent):
        float_arr = (c_float*6)()
        bool_arr = (c_bool*6)()
        for i in range(0,6):
            float_arr[i] = angle[i]
            bool_arr[i]=activeFlag[i]
        self.BElib.setRelativePosition_py_1(activeFlag, float_arr, moveType.value, movebaseType.value)
        return

    ## Set Eye space absolute Position (angle:float.Size = 6   imuAngle:float.Size = 6)
    def setSpaceAbsPosition_0(self,
                              type:MotorType,
                              angle,
                              imuAngle,
                              moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade,
                              movebaseType:BE_MoveBaseType=BE_MoveBaseType.enumMoveBase_Independent):
        float_arr = (c_float*6)()
        float_imu_arr = (c_float*3)()
        for i in range(0, 3):
            float_arr[i] = angle[i]
            float_arr[i+3] = angle[i+3]
            float_imu_arr[i] = imuAngle[i]
        self.BElib.setSpaceAbsPosition_py_0(type.value, float_arr, float_imu_arr, moveType.value, movebaseType.value)
        return

    ## Set Eye space absolute Position (angle:float.Size = 6   imuAngle:float.Size = 6   activeFlag:bool.Size = 6)
    def setSpaceAbsPosition_1(self,
                              activeFlag,
                              angle,
                              imuAngle,
                              moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade,
                              movebaseType:BE_MoveBaseType=BE_MoveBaseType.enumMoveBase_Independent):
        float_arr = (c_float*6)()
        bool_arr = (c_bool*6)()
        float_imu_arr = (c_float*3)()
        for i in range(0,6):
            float_arr[i] = angle[i]
            bool_arr[i]=activeFlag[i]
            float_imu_arr[i%3]=imuAngle[i%3]
        self.BElib.setSpaceAbsPosition_py_1(activeFlag, float_arr, float_imu_arr, moveType.value, movebaseType.value)
        return

    ## Set Neck Position ( if have ) (angle:float.Size = 3   activeFlag:bool.Size = 3)
    def setAbsolutePosition_Neck(self,
                                 activeFlag,
                                 angle,
                                 moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade):
        float_arr = (c_float*3)()
        bool_arr = (c_bool*3)()
        for i in range(0,3):
            float_arr[i] = angle[i]
            bool_arr[i] = activeFlag[i]

        self.BElib.setAbsolutePosition_Neck_py(activeFlag, float_arr, moveType.value)
        return

    ## Set Neck Position ( if have ) (angle:float.Size = 3   activeFlag:bool.Size = 3)
    def setRelativePosition_Neck(self,
                                 activeFlag,
                                 angle,
                                 moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade):
        float_arr = (c_float*3)()
        bool_arr = (c_bool*3)()
        for i in range(0,3):
            float_arr[i] = angle[i]
            bool_arr[i] = activeFlag[i]
        self.BElib.setRelativePosition_Neck_py(activeFlag, float_arr, moveType.value)
        return

    ## Set Neck & Eye Union Position （Absolute space） (eyeAngle:float.Size = 3   neckAngle:float.Size = 3    activeFlag:bool.Size = 3)
    def setUnionAbsolutePosition_NeckEye(self,
                                         activeFlag,
                                         eyeAngle,
                                         neckAngle,
                                         moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade):
        float_arr = (c_float*3)()
        float_neck_arr = (c_float*3)()
        bool_arr = (c_bool*3)()
        for i in range(0,3):
            float_arr[i] = eyeAngle[i]
            float_neck_arr[i] = neckAngle[i]
            bool_arr[i] = activeFlag[i]
        self.BElib.setUnionAbsolutePosition_NeckEye_py(activeFlag, float_arr, float_neck_arr, moveType.value)
        return

    ## Set Neck & Eye Union Position （Absolute space） (eyeAngle:float.Size = 3   neckAngle:float.Size = 3    activeFlag:bool.Size = 3)
    def setUnionRelativePosition_NeckEye(self,
                                         activeFlag,
                                         eyeAngle,
                                         neckAngle,
                                         moveType:BE_MovePatternType=BE_MovePatternType.enumMovePattern_Saccade):
        float_arr = (c_float*3)()
        float_neck_arr = (c_float*3)()
        bool_arr = (c_bool*3)()
        for i in range(0,3):
            float_arr[i] = eyeAngle[i]
            float_neck_arr[i] = neckAngle[i]
            bool_arr[i] = activeFlag[i]
        self.BElib.setUnionRelativePosition_NeckEye_py(activeFlag, float_arr, float_neck_arr, moveType.value)
        return

    ## Init position command( For eye)
    def goInitPosition(self, type:MotorType = MotorType.enumAllMotor):
        self.BElib.goInitPosition_py(type.value)
        return

    def getInitPosition(self):
        float_arr = (c_float*6)()
        result_arr = [0.0]*6
        self.BElib.getInitPosition_py(MotorType.enumAllMotor.value, float_arr)
        for i in range(0,6):
            result_arr[i] = float_arr[i]
        return result_arr

        ## Init position command( For neck)
    def goInitPosition_Neck(self, type:MotorType_Neck = MotorType_Neck.enumNeckAllMotor):
        self.BElib.goInitPosition_Neck_py(type.value)
        return

    def getInitPosition_Neck(self):
        float_arr = (c_float*3)()
        result_arr = [0.0]*3
        self.BElib.getInitPosition_Neck_py(MotorType_Neck.enumNeckAllMotor.value, float_arr)
        for i in range(0,3):
            result_arr[i] = float_arr[i]
        return result_arr

    ## return Has bionic eye system linked neck
    def haveNeckLinked(self):
        return self.BElib.haveNeckLinked_py()

    ## Open Close VOR function (vor_eye:bool.Size = 1   vor_neck:bool.Size = 1)
    def onOff_VOR(self, vor_eye, vor_neck):
        return self.BElib.onOff_VOR_py(vor_eye, vor_neck)

    ## Open Close sv function
    def onoff_SV(self, onoff):
        return self.BElib.onoff_SV_py(onoff)

    ## Set sv work model (rect:int.Size = 4   size:int.Size = 2)
    def setSvModel(self,rect, size, type:SVType = SVType.enumCentreSV):
        rect_arr = (c_int*4)()
        size_arr = (c_int*2)()
        for i in range(0,2):
            rect_arr[i] = rect[i]
            rect_arr[i+2] = rect[i+2]
            size_arr[i] = size[i]
        return self.BElib.setSvModel_py(type.value, rect_arr, size_arr)

    ## set be-device synchronizing signal source (1: send signal, 0: only recv signal) */
    def setSyncSignalSource(self, flag):
        return self.BElib.setSyncSignalSource_py(flag)



