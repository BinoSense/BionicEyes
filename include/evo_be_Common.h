/*****************************************************************************
* Eyevolution BionicEye series products SDK.
* Copyright (C) 2018, Eyevolution, all rights reserved.
*
* @brief This is for general use of Eyevolution BionicEye series products.
* @author Openking <wang.kaifang@ilooktech.com>
* @date <2018/10/01>
* @note This file is very important! Please Do Not Modify it!!!
*****************************************************************************/

#pragma once
#ifndef _EVO_BE_COMMON_H_
#define _EVO_BE_COMMON_H_

#include <evo_be.h>
#include <stdlib.h>

#ifndef WIN32
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

/**************************************************
 * @brief Millisecond Delay Function For Linux USE
 **************************************************/
#define msleep(ms) usleep(1000 * (ms))

#else // for windows
#include <windows.h>
#include <time.h>
/**************************************************
 * Millisecond Delay Function For windows USE
 * @brief Millisecond Delay Function For windows USE
 **************************************************/
#define msleep(ms) Sleep(ms)

// detect keyboard press (virtual keyboard table: https://blog.csdn.net/limingmcu/article/details/25896315)
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1 : 0)
#endif

/**************************************************
 * @brief  Provide random number from 0..(num-1)
 **************************************************/
#define within(num) (int)((float)(num)*rand() / (RAND_MAX + 1.0))
//#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

using namespace std;

static const float RATE_ANGLE_PULSE = 360.0f / 55360.0f; /**<  */
static const float RATE_ANGLE_PULSE_5 = 360.0f / 64000.0f;
static const int NETWORK_TRANSFORM_MAXSIZE = 33554432;      //16777216

#if defined(_WIN32) && !defined(CYGWIN)
typedef __int64 int64_t;
#else
#if __WORDSIZE == 64
typedef long int int64_t;
#else
__extension__ typedef long long int int64_t;
#endif
#endif // _WIN32

/**************************************************
 * Get system time (100 Microseconds (UTC time)) since 0:0:0:000 on 1 January 1970
 * @brief  Get system time (100 Microseconds)
 * @return int64_t pass counted times since 0:0:0:000 on 1 January 1970(Every 100 Microseconds)
 **************************************************/
inline static int64_t GetSysTimeMicros()
{
#ifdef _WIN32
	// From 0:0:0:000 on January 1, 1601 to 0:0:0:000 on January 1, 1970 (in 100ns)
#define EPOCHFILETIME (116444736000000000UL)
	FILETIME ft;
	LARGE_INTEGER li;
	int64_t tt = 0;
	GetSystemTimeAsFileTime(&ft);
	li.LowPart = ft.dwLowDateTime;
	li.HighPart = ft.dwHighDateTime;
	// Microseconds (UTC time) since 0:0:0:000 on 1 January 1970
	tt = (li.QuadPart - EPOCHFILETIME) / 1000;
	return tt;
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return ((int64_t)tv.tv_sec * 1000000 + (int64_t)tv.tv_usec) / 100;
#endif // _WIN32
}

namespace evo_be
{
#ifdef _WIN32
#ifdef EVO_BIONICEYES_EXPORTS
#define EVO_BIONICEYES_API __declspec(dllexport)
#else
#define EVO_BIONICEYES_API __declspec(dllimport)
#endif // EVO_BIONICEYES_EXPORTS
#else
#define EVO_BIONICEYES_API
#endif // _WIN32
    /**************************************************
     * @brief Enumeration of BinoSense device connect type
     * @note Only for binosense device remote connect use
     **************************************************/
	enum BE_Connect_Type
	{
		enumdisConnect = 0,			        /**< No connect BE device(s) */
		enumConnect_Control = 1,	        /**< Only connect to control BE device */
		enumConnect_Image = 2,		        /**< Only connect to get image info */
		enumConnect_ImageControl = 3,       /**< Connect to control BE device and get image info */
	};

    /**************************************************
     * If you want to connect binosense directly by network,
     * you can choose enumDeviceServer_First or enumDeviceServer_Only.
     * This will occupy N*bandwidth when you run N independent processes
     * in one PC.
     * When you choose enumLocalServer_First or enumLocalServer_Only,
     * only a bandwidth will be occupied in one PC.
     *
     * @brief Enumeration of BinoSense device data server type
     * @note Only for binosense device remote connect use. When you choose
     * enumLocalServer_First or enumLocalServer_Only, a local server must be started. You can find an example in BionicEye_Samples.
     **************************************************/
	enum BE_Connect_DataServerType
	{
		enumLocalServer_First = 0,	        /**< For remote use, first connect Local server */
		enumDeviceServer_First = 1,         /**< For remote use, first connect BinoSense device server */
		enumDeviceServer_Only = 2,	        /**< For remote use, only connect BinoSense device server */
		enumLocalServer_Only = 3,	        /**< For remote use, only connect Local server */
	};

    /**************************************************
     * Choose enumDataTransmission_ASPAP will get newest data
     * as soon as possible, but may lost some data.
     * Choose enumDataTransmission_OneByOne will not lost data,
     * but may delay newest data receiving time. If recviving
     * too slow compared to device pub data, Some data will
     * still be lost.
     * @brief Used to set data transmission mode type
     * @note Only for binosense device remote connect use. You can find an example in
     * BionicEye_Samples.
     **************************************************/
	enum BE_Data_TransmissionType
    {
	    enumDataTransmission_ASPAP = 0,        /**< As soon as possible. But frames may be lost. */
	    enumDataTransmission_OneByOne = 1,     /**< One by one. */
	};

    /**************************************************
     * @brief Enumeration of binosense device move pattern type
     **************************************************/
	enum BE_MovePatternType
	{
		enumMovePattern_Saccade = 0,	        /**< Saccade move (Fast mode) */
		enumMovePattern_SmoothPursuit = 1,      /**< Smooth pursuit move (Smooth mode) */
	};

    /**************************************************
     * @brief Enumeration of binosense device move base type.
     **************************************************/
	enum BE_MoveBaseType
	{
	    /**************************************************
         * Binosense device moves independently
         * (left roll/pitch/yaw moves have no relationship
         * with right roll/pitch/yaw moves).
         * In this mode, SV don't work.
         **************************************************/
        enumMoveBase_Independent = 0,
        /**************************************************
         * Binosense moves based on left eye(only left
         * roll/pitch/yaw moves is base,
         * right eye move is same to left eye).
         **************************************************/
        enumMoveBase_LeftEye = 1,
        /**************************************************
         * Binosense moves based on right eye(only right
         * roll/pitch/yaw moves is base,
         * left eye move is same to right eye).
         **************************************************/
		enumMoveBase_RightEye = 2,
	};

    /**************************************************
     * @brief Enumeration of motor pid parameter type
     **************************************************/
	enum MotorPIDType
	{
		enumInnerLoopP_VOR = 0,
		enumInnerLoopI_VOR = 1,
		enumInnerLoopD_VOR = 2,
		enumOuterLoopP_VOR = 3,
		enumOuterLoopI_VOR = 4,
	};

    /**************************************************
     * This class contains all methods of BinoSense
     * device use.
     * For example, you can use this class to control
     * BinoSense device move, to get device data, to set
     * device running parameters and so on.
     * @brief BinoSense Device API
     **************************************************/
	class EVO_BIONICEYES_API CBionicEyes
	{
	public:
        /**************************************************
         * Creating BinoSense device instances for direct
         * connection use.
         * @brief Creating BinoSense device instances
         * @param openNetworkService:           Set #True# to use network service and support remote connect, or set false to
         * decrease CPU usage.
         * @param dataDefaultFrameRate:          Set default frame(including image， motor coder， IMU data, and so on..)
         * acquisition rate. You can still change it by calling function setDataRate. Max accepted value is 200. In some device
         *  and some image resolution setting, this value may be less than 200.\n
         * For example CBionicEyes *device = device->create(false, 50);
         * @note When setting openNetworkService true, you still need to start evo_be_Device_Service_**** program to support
         * remote user.
         **************************************************/
		static CBionicEyes *create(bool openNetworkService = true, int dataDefaultFrameRate = 25, void *logger_ptr = NULL);

        /**************************************************
         * Creating BinoSense device instances for remote connection use. When use this to create instance, all
         * devices in local network will be detected and listed, you can choose which device you want to use.
         * @brief Creating BinoSense device instances.
         * @note One instance can only connect one BinoSense device. If you have more BinoSense to connect, more
         * create functions should be called.
         * @param type:             Device connect type, for example, enumConnect_ImageControl means you can
         * control device and get its data including image info.
         * @param dataServerType:   You can choose connect local server to get device data or connect device to
         * get data directly. When local server is chosen, a local server must be started, or you can't get any data.
         * @param dataTransmissionType:     You can decide to get newest frame as soon as possible, or get every frame
         * with any frame lost.\n
         * For example CBionicEyes *device = device->create(enumConnect_ImageControl);
         **************************************************/
		static CBionicEyes *create(BE_Connect_Type type,
                                    BE_Connect_DataServerType dataServerType = enumDeviceServer_Only,
                                    BE_Data_TransmissionType dataTransmissionType = enumDataTransmission_ASPAP,
                                    void *logger_ptr = NULL);

        /**************************************************
         * Creating BinoSense device instances for remote connection use. This function will connect device which IP
         * address is same to given IP address.
         * @brief Creating BinoSense device instances.
         * @note One instance can only connect one BinoSense device. If you have more BinoSense to connect, more
         * create functions should be called.
         * @param type:             Device connect type, for example, enumConnect_ImageControl means you can
         * control device and get its data including image info.
         * @param dataServerType:   You can choose connect local server to get device data or connect device to
         * get data directly. When local server is chosen, a local server must be started, or you can't get any data.
         * @param dataTransmissionType:     You can decide to get newest frame as soon as possible, or get every frame
         * with any frame lost.\n
         * For example CBionicEyes *device = device->create("192.168.1.20", enumConnect_ImageControl);
         **************************************************/
		static CBionicEyes *create(string ipAddr,
                                    BE_Connect_Type type = enumdisConnect,
                                    BE_Connect_DataServerType dataServerType = enumDeviceServer_Only,
                                    BE_Data_TransmissionType dataTransmissionType = enumDataTransmission_ASPAP,
                                    void *logger_ptr = NULL);

        static CBionicEyes *create(string SrvipAddr,
                                   string devName,
                                   void *logger_ptr = NULL, int mediaPort=6674, int controlPort=6676);

//        static CBionicEyes *create(string SrvipAddr,
//                                   string devName,
//                                   void *logger_ptr = NULL);
        /**************************************************
         * @brief Destructor
         **************************************************/
		virtual ~CBionicEyes();

        /**************************************************
         * @brief Get connected BinoSense device IP address(32 bits integer).
         **************************************************/
		virtual uint32_t getBeDevice_Ip() = 0;

        /**************************************************
         * @brief Get connected BinoSense device type.
         **************************************************/
        virtual BE_Device_Type getBeDeviceType() = 0;

        /**************************************************
         * @brief Get connected BinoSense device IP address(string type, like 192.168.1.1).
         **************************************************/
		virtual string getBeDevice_Ip_str() = 0;

        /**************************************************
         * Set image resolution(SideBySide size!)(BE_ImageGeneralInfo.image, only for remote use)
         * @brief Creating Set image resolution.
         * @note This will affect function "getBeData" returning image size, but don't affect network transform image size.
         * @param size:     set image size, default (3840, 1080)
         * @param cameraId:     set specified camera, see
         **************************************************/
		virtual void setImageResolution(cv::Size size = cv::Size(1920, 1080)) = 0;

        /**************************************************
         * Set network transfer image resolution(SideBySide size!), if network isn't ideal, you could down sample the transfer image
         * @brief Creating Set network transfer image resolution.
         * @note This won't affect function "getBeData" returning image size.
         * @param size:     set image size, default (1920, 540)
         **************************************************/
		virtual void setImageResolution_Transfer(cv::Size size = cv::Size(960, 540)) = 0;

        /**************************************************
         * Set image color (BE_ImageGeneralInfo.colorType)
         * @brief Set image color
         * @note This will affect function "getBeData" returning image color type.
         * @param type:     set image color type, default enumColor
         **************************************************/
		virtual void setImageColor(ImageColorType type = enumColor) = 0;

        /**************************************************
         * @brief Set network transfer image color
         * @note This won't affect function "getBeData" returning image color type.
         * @param type:     set image color type, default enumColor
         **************************************************/
		virtual void setImageColor_Transfer(ImageColorType type = enumColor) = 0;

        /**************************************************
         * @brief Set network transfer frame rate
         * @param rate:     set transfer frame rate, default 25.0
         **************************************************/
		virtual void setDataRate_Transfer(float rate = 25.0f) = 0;

        /**************************************************
         * @brief Get origin camera image resolution
         **************************************************/
		virtual cv::Size getOriginImageResolution() = 0;

        /**************************************************
         * @brief Get max image frame rate
         **************************************************/
		virtual float getMaxImageFrameRate() = 0;

        /**************************************************
         * @brief Set image loseless compression or lossy compression (only for network transfer)
         * @note loseless compression: imgQual = 100; lossy compression: imgQual (0 = worst, 99 = best)
         **************************************************/
		virtual void setImageCompressionQuality(int imgQual = 95) = 0;

        /**************************************************
         * @brief Determine if the data is ready.
         * @note Before calling "getBeData", you should ensure that this function return True.
         **************************************************/
		virtual bool isBeDataReady() = 0;

        /**************************************************
         * @brief Get bedata.
         * @note Thread safe and returned data has be copied.(Deep copy)
         * @note Before calling this function, you should call
         * isBeDataReady first and ensure function isBeDataReady return True.
         **************************************************/
        virtual void getBeData(BE_GeneralData &data) = 0;

        /**************************************************
         * @brief Get bedata.
         * @note Thread safe and returned data has be copied.(Deep copy)
         * @note Before calling this function, you should call
         * isBeDataReady first and ensure function isBeDataReady return True.
         **************************************************/
        virtual BE_GeneralData getBeData() = 0;

        /**************************************************
         * @brief Get bedata.
         * @note Thread safe and returned data has be copied.(Deep copy)
         * @note Before calling this function, you should call
         * isBeDataReady first and ensure function isBeDataReady return True.
         **************************************************/
        virtual bool getBeData(BE_GeneralData &data, uint id) = 0;

        /**************************************************
         * @brief Get Bionic Eye Calibration Data
         * @note Caller is responsible for freeing the returned buffer.
         **************************************************/
		virtual uchar *getCalibrationInfo() = 0;

        /**************************************************
         * @brief Get motor up and down limit position(For eye).
         * @note Type(enumAllMotor) is not supported.
         * @param type:     Choose which motor you want to get up and down limit.
         * @param up:       Float data reference, return up limit value
         * @param down:     Float data reference, return down limit value
         **************************************************/
		virtual bool getUpDownLimit(MotorType type, float &up, float &down) = 0;

        /**************************************************
         * @brief Get motor up and down limit position(For neck)
         * @note Type(enumNeckAllMotor) is not supported.
         * @param type:     Choose which motor you want to get up and down limit.
         * @param up:       Float data reference,
         * @param down:     Float data reference,
         **************************************************/
		virtual bool getUpDownLimit_Neck(MotorType_Neck type, float &up, float &down) = 0;

        /**************************************************
         * @brief Set eye absolute speed(deg/s).
         * @param activeFlag:     Bool data array(size = 6). Choose which motor or motors you want to set speed. If you set
         * activeFlag[id] = false; the no.id motor will auto set speed to 0.
         * @param speed:    Float data array(size = 6). Only for chosen motor, the value will work.
         **************************************************/
        virtual void setAbsoluteSpeed(bool activeFlag[], float speed[]) = 0;

        /**************************************************
         * @brief Set eye absolute position
         * @param type:     Choose which motor or all motors you want to send move cmd
         * @param angle:    Float data array(size = 6). For chosen motor, the value will work.
         * @param moveType:     Set move type
         * @param movebaseType: Set move base Type
         **************************************************/
        virtual void setAbsolutePosition(MotorType type,
                                         float angle[],
                                         BE_MovePatternType moveType = enumMovePattern_Saccade,
                                         BE_MoveBaseType movebaseType = enumMoveBase_Independent) = 0;

        /**************************************************
         * @brief Set Eye relative Position
         * @param type:     Choose which motor or all motors you want to send move cmd
         * @param angle:    Float data array(size = 6). For chosen motor, the value will work.
         * @param moveType:     Set move type
         * @param movebaseType: Set move base Type
         **************************************************/
        virtual void setRelativePosition(MotorType type,
                                         float angle[],
                                         BE_MovePatternType moveType = enumMovePattern_Saccade,
                                         BE_MoveBaseType movebaseType = enumMoveBase_Independent) = 0;

        /**************************************************
         * @brief Set eye absolute position
         * @param activeFlag:     Bool data array(size = 6). Choose which motor or motors you want to send move cmd
         * @param angle:    Float data array(size = 6). Only for chosen motor, the value will work.
         * @param moveType:     Set move type
         * @param movebaseType: Set move base Type
         **************************************************/
		virtual void setAbsolutePosition(bool activeFlag[],
                                   float angle[],
                                   BE_MovePatternType moveType = enumMovePattern_Saccade,
                                   BE_MoveBaseType movebaseType = enumMoveBase_Independent) = 0;

        /**************************************************
         * @brief Set eye relative position.
         * @param activeFlag:     Bool data array(size = 6). Choose which motor or motors you want to send move cmd
         * @param angle:    Float data array(size = 6). Only for chosen motor, the value will work.
         * @param moveType:     Set move type
         * @param movebaseType: Set move base Type
         **************************************************/
		virtual void setRelativePosition(bool activeFlag[],
                                   float angle[],
                                   BE_MovePatternType moveType = enumMovePattern_Saccade,
                                   BE_MoveBaseType movebaseType = enumMoveBase_Independent) = 0;

        /**************************************************
         * Set space Eye absolute Position （Absolute space）
         * @brief Set space Eye absolute Position
         * @note Only when device is on VOR mode, this function can work!
         * @param motorAngle:     Float data array(size = 6). Input current all motor value.
         * @param motorMoveAngle:    Float data array(size = 6). Set motor move angle value.
         * @param imuAngle:    Float data array(size = 3). Input current all IMU value.
         * @param moveType:     Set move type
         * @param movebaseType: Set move base Type
         **************************************************/
		virtual void setSpaceAbsPosition(float motorAngle[],
                                   float motorMoveAngle[],
                                   float imuAngle[],
                                   BE_MovePatternType moveType = enumMovePattern_Saccade,
                                   BE_MoveBaseType movebaseType = enumMoveBase_Independent) = 0;
		//virtual void setSpaceAbsPosition(bool activeFlag[], float angle[], float imuAngle[], BE_MovePatternType moveType = enumMovePattern_Saccade, BE_MoveBaseType movebaseType = enumMoveBase_Independent) = 0;

        /**************************************************
         * @brief Set neck absolute position (If neck exists)
         * @note Type(enumNeckAllMotor) is not supported.
         * @param activeFlag:   Bool data array(size = 3). Choose which motor or motors you want to send move cmd
         * @param angle:        Float data array(size = 3). Only for chosen motor, the value will work.
         * @param moveType:     Set move type
         **************************************************/
		virtual void setAbsolutePosition_Neck(bool activeFlag[],
                                        float angle[],
                                        BE_MovePatternType moveType = enumMovePattern_Saccade) = 0;

        /**************************************************
         * Set neck relative position (If neck exists) (Array Size = 3)
         * @brief Set neck space eye relative position
         * @param activeFlag:   Bool data array(size = 3). Choose which motor or motors you want to send move cmd
         * @param angle:        Float data array(size = 3). Only for chosen motor, the value will work.
         * @param moveType:     Set move type
         **************************************************/
		virtual void setRelativePosition_Neck(bool activeFlag[],
                                        float angle[],
                                        BE_MovePatternType moveType = enumMovePattern_Saccade) = 0;

		//void setSpaceAbsPosition(bool activeFlag[], float angle[], float imuAngle[], BE_MovePatternType moveType = enumMovePattern_Saccade);

        /**************************************************
         * @brief Set neck & eye union position （Absolute space）
         * @param activeFlag:   Bool data array(size = 3). Choose which motor or motors you want to send move cmd
         * @param angle:        Float data array(size = 3). Only for chosen motor, the value will work.
         * @param moveType:     Set move type
         **************************************************/
		virtual void setUnionAbsolutePosition_NeckEye(bool activeFlag[], float eyeAngle[], float neckAngle[], BE_MovePatternType moveType = enumMovePattern_Saccade) = 0;

        /**************************************************
         * Set neck relative position (If neck exists) (Array Size = 3)
         * @brief Set neck space eye relative position
         * @param activeFlag:   Bool data array(size = 3). Choose which motor or motors you want to send move cmd
         * @param angle:        Float data array(size = 3). Only for chosen motor, the value will work.
         * @param moveType:     Set move type
         **************************************************/
		virtual void setUnionRelativePosition_NeckEye(bool activeFlag[], float eyeAngle[], float neckAngle[], BE_MovePatternType moveType = enumMovePattern_Saccade) = 0; // For future

        /**************************************************
         * Set current position as init position command. Current motor value will turn 0 after restart.
         * @brief Set current position as init position command
         * @note Current postion will be initial position. Effective after restart.
         **************************************************/
        virtual void setNowPositionAsInitPos() = 0;

        /**************************************************
         * @brief Chosen motor or motor will go to init position.
         * @param type: Set chosen eye motor.
         **************************************************/
		virtual void goInitPosition(MotorType type) = 0;

        /**************************************************
         * @brief Get chosen motor or motors init position value;
         * @param type: Set chosen eye motor.
         * @param angle: return motor initial position value
         **************************************************/
		virtual void getInitPosition(MotorType type, float angle[]) = 0;

        /**************************************************
         * @brief Chosen neck motor or motor will go to init position.
         * @param type: Set chosen neck motor.
         **************************************************/
		virtual void goInitPosition_Neck(MotorType_Neck type) = 0;

        /**************************************************
         * @brief Get chosen neck motor or motors init position value.
         * @param type: Set chosen neck motor.
         * @param angle: return neck motor initial position value
         **************************************************/
		virtual void getInitPosition_Neck(MotorType_Neck type, float angle[]) = 0;

        /**************************************************
         * @brief return true if neck existed
         **************************************************/
		virtual bool haveNeckLinked() = 0;

        /**************************************************
         * @brief Open or close VOR function
         * @param vor_eye on/off eye vor
         * @param vor_neck on/off neck vor
         **************************************************/
		virtual void onoff_VOR(bool vor_eye = true, bool vor_neck = false) = 0;

        /**************************************************
         * @brief Open or close SV function
         * @param onoff on/off SV function
         **************************************************/
		virtual void onoff_SV(bool onoff) = 0;

        /**************************************************
         * @brief Set sv work mode
         * @param type: set sv work mode type (see SVType)
         * @param rect: set work area
         * @param size: set origin image size
         **************************************************/
		virtual void setSvModel(SVType type = enumGlobalSV, cv::Rect rect = cv::Rect(0, 0, 1920, 1080), cv::Size size = cv::Size(1920, 1080)) = 0;

        /**************************************************
         * Set be-device synchronizing signal source
         * @brief Set be-device synchronizing signal source
         * @param flag: True: send signal, False: only recv signal
         **************************************************/
		virtual void setSyncSignalSource(bool flag = true) = 0;

        /**************************************************
         * @brief Get camera exposure time set range
         * @param up: return exposure time up_limit
         * @param down: return exposure time down_limit
         **************************************************/
		virtual void getCameraExposureTimeRange(float &up, float &down) = 0;

        /**************************************************
         * @brief Get camera exposure time
         * @param type: Specify camera to change parameter, see BE_ImageSensorType and CameraIndex  (-1 means all camera!!!)
         **************************************************/
		virtual float getCameraExposureTime(int type) = 0;

        /**************************************************
         * @brief Set camera exposure time
         * @param type: Specify camera to change parameter, see BE_ImageSensorType and CameraIndex  (-1 means all camera!!!)
         * @param autoExposure: True: set auto exposure mode, False: set manual exposure mode
         * @param exposureTime: set exposure time. Only works when autoExposure = False;
         **************************************************/
		virtual void setCameraExposure(int type, bool autoExposure, float exposureTime = 0) = 0;

        /**************************************************
         * @brief Get Camera WhiteBalance Temperature set range
         * @param up: return WhiteBalance Temperature up_limit
         * @param down: return WhiteBalance Temperature down_limit
         **************************************************/
        virtual void getCameraWhiteBalanceTemperatureRange(int &up, int &down) = 0;

        /**************************************************
         * @brief Get Camera WhiteBalance Temperature
         * @param type: Specify camera to change parameter, see BE_ImageSensorType and CameraIndex  (-1 means all camera!!!)
         **************************************************/
        virtual float getCameraWhiteBalanceTemperature(int type) = 0;

        /**************************************************
         * @brief Set Camera WhiteBalance Temperature
         * @param type: Specify camera to change parameter, see BE_ImageSensorType and CameraIndex  (-1 means all camera!!!)
         * @param autoWhitebalance: True: set whitebalance temperature mode, False: set manual whitebalance temperature mode
         * @param WhiteBalanceTemperature: set whitebalance temperature. Only works when autoExposure = False;
         **************************************************/
        virtual void setCameraWhiteBalanceTemperature(int type, bool autoWhitebalance, float WhiteBalanceTemperature = 6600) = 0;

        /**************************************************
         * @brief Get camera exposure auto reference range
         * @param up: Return up limit
         * @param down: Return down limit
         **************************************************/
        virtual void getCameraExposureAutoReferenceRange(float &up, float &down) = 0;

        /**************************************************
         * @brief Set camera exposure auto reference
         * @note This function only supports some specified BinoSense Device. For example, BinoSense V3 is not
         * supported.
         * @param type: Specify camera to change parameter, see BE_ImageSensorType and CameraIndex  (-1 means all camera!!!)
         * @param value: set exposure exposure auto reference value
         **************************************************/
        virtual void setCameraExposureAutoReference(int type, int value = 70) = 0;

        /**************************************************
         * @brief Get camera gain range
         * @param up: Return up limit
         * @param down: Return down limit
         **************************************************/
        virtual void getCameraGainRange(float &up, float &down) = 0;

        /**************************************************
         * @brief Set camera Gain work mode
         * @note This function only supports some specified BinoSense Device. For example, BinoSense V3 is not
         * supported.
         * @param type: Specify camera to change parameter, see BE_ImageSensorType and CameraIndex  (-1 means all camera!!!)
         * @param autoGain: Set camera gain set work in auto mode or manually mode.
         * @param value: set gain value(Only works when autoGain = False)
         **************************************************/
        virtual void setCameraGain(int type, bool autoGain = true, int value = 370) = 0;

        /**************************************************
         * Start to save all data when calling this function.
         * @brief Save all be_data
         * @note If you have chosen high image resolution, note the disk capacity.
         * You can change saving image size by calling setImageResolution_Transfer
         * @param startStop: 0, stop; 1, start;
         * @param folderPath: set data_saving path. If path exists, this function will auto generate a new path
         * named path_T.
         **************************************************/
		virtual void saveBeData(bool startStop = 1, string folderPath = Be_Io_Path + "fastSave") = 0;

        /**************************************************
         * Snap be_data when calling this function. May have some time delay(ms).
         * @brief Snap be_data when calling this function.
         * @note You can change saving image size by calling setImageResolution_Transfer
         * @note After calling snapBeData in the first time, the param folderPath will not work until
         * function stopSnapBeData is called.
         * @param description: You can give this snaped data a description. It will be written
         * in CSV file.
         * @param folderPath: set data saving path. If path exists, this function will auto
         * generate a new path named path_T.
         **************************************************/
		virtual bool snapBeData(string description="", string folderPath = Be_Io_Path + "snapSave") = 0;

        /**************************************************
         * Stop snapping be_data when calling this function. May have some time delay(ms).
         * @brief Stop snapping be_data when calling this function.
         * @note If you want to change a folder to save snaped data, call this function first
         * and then recall snapBeData.
         **************************************************/
        virtual void stopSnapBeData() = 0;

        /**************************************************
         * Record be_data when calling this function. May have some time delay(ms).
         * @brief Record be_data when calling this function.
         * @note You can change saving image size by calling setImageResolution_Transfer
         * @note After calling recordBeData in the first time, the param folderPath will not work until
         * function stopRecordBeData is called.
         * @param data: The data will be recorded.
         * @param description: You can give this recorded data a description. It will be written
         * in CSV file.
         * @param folderPath: set data saving path. If path exists, this function will auto
         * generate a new path named path_T.
         **************************************************/
        virtual bool recordBeData(BE_GeneralData &data, string description="", string folderPath = Be_Io_Path + "recordSave") = 0;

        /**************************************************
         * Stop recording be_data when calling this function.
         * @brief Stop recording be_data when calling this function.
         * @note If you want to change a folder to save recorded data, call this function
         * first and then recall recordBeData. It's similar to snapBeData & stopSnapBeData.
         **************************************************/
        virtual void stopRecordBeData() = 0;

        /**************************************************
         * Give a trigger signal to data server to send next data(Only for remote connect use,
         * and parameter dataTransmissionType must be enumDataTransmission_OneByOne)
         * @brief Give a trigger signal to data server to send a next data
         **************************************************/
        virtual void triggerDataTransmission() = 0;

        /*! @brief Set grab data rate, Max 200 fps.*/
        virtual void setBeDataRate(int rate = 25) = 0;

        /*! @brief Set sync camera delay time: max 30000 us.*/
        virtual void setSyncCameraDelayTime(int timeDelay = 0) = 0;

        /*! @brief Transform infrared image. Only for bionic device which has infrared camera.*/
        virtual bool infraImageTrans(BE_GeneralData &data, cv::Mat& infraImage, cv::Mat& tempImage) = 0;

        /*! @brief Calculate temp for point value of infrared image.*/
        virtual float infraTempCalc(uint value) = 0;

        /**************************************************
         * On/Off camera image grab or network transform. It will help to save system resource or bandwidth resources
         * @brief On/Off camera image grab or network transform
         * @param flag: Bool data array(size = MAXCAMERASInDEVICE). Set 1 means ON and set 0 means OFF
         * @param NetworkType: 1 means only affect network transform, 0 means affect both network and local image grab
         **************************************************/
        virtual void onOffCameraImage(bool flag[], bool onlyNetworkType = true) = 0;

        /**************************************************
         * @brief On/Off Laser ranging
        **************************************************/
        virtual void onOffLaserRanging(bool onOff = true) = 0;

        /**************************************************
         * @brief On/Off Infrared Light
        **************************************************/
        virtual void onOffInfraredFillingLamp(bool onOff = false, uchar focalLength = 0) = 0;

        /**************************************************
         * @brief Set zoom camera module focal length, range: 1~30
        **************************************************/
        virtual void setZoomFocalLength(uchar focalLength = 1) = 0;

        /**************************************************
         * @brief Set wipers module(Only for eagle eye device),0: close ,1~255 :open, different speed
        **************************************************/
        virtual void setWipers(uchar value = 0) = 0;

        // for wiper: para0, para1 work.
        //      para0: id, 暂时无用
        //      para1: 0 停止运动， 1~255 速度
        // for zoom camera focal length: para0 works.
        //      para0: 1~30
        // for laserranging: para0, para1 work.
        //      para0: id, 暂时无用
        //      para1: 1/0，开/关
        // for infrared filling lamp: para0, para1, para2 work.
        //      para0: id, 暂时无用
        //      para1: 1/0，开/关  
        //      para2: 焦距， 0~255  
        // for mouth: para0 works.
        //      para0: 0x00 闭合，0x01 微张，0x02 半张，0x03 全张，0x0A 慢速随机，0x0B 快速随机，0x0C 跟随喇叭动
        // for lighting: para0 and para1 work.
        //      para0: 0x00 额头灯，0x01 胸部灯，0x02 右耳灯，0x03 左耳灯，0x04 底盘灯
        //      para1: 0xFE 呼吸，0xFF 关闭，0x00 常亮， 其余为闪烁的频率
        // for arm: para0, para1, and para2 work.
        //      para0: 0xC8 右手臂， 0xC9 左手臂
        //      para1: 0x00 执行动作， 0x01 学习动作
        //      para2: 0x00~0x0F 16个动作， 0x10 解锁关节,  0x11 锁定关节, 0x12 设置为初始位置
        virtual void setExtraModuleFunction(BE_ExtraModuleType type, int para0 = 0, int para1 = 0, int para2 = 0) = 0;
        
        virtual void setMotorPositionSpeed(int saccadeSpeed,int pursuitSpeed) = 0;
	};

} // namespace evo_be

#endif //_EVO_BE_COMMON_H_
