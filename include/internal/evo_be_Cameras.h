#pragma once
#ifndef __BE_CAMERAS_H__
#define __BE_CAMERAS_H__

#ifdef _WIN32
#ifdef EVO_BE_CAMERAS_EXPORTS
#define EVO_BE_CAMERAS_API __declspec(dllexport)
#else
#define EVO_BE_CAMERAS_API __declspec(dllimport)
#endif	// EVO_BE_CAMERAS_EXPORTS
#else
#define EVO_BE_CAMERAS_API
#endif	// _WIN32

#include <evo_be.h>

using namespace std;
using namespace evo_be;

namespace evo_be
{
    struct BECameraInfo
    {
        uint cameraType;
        string cameraSN;
        int trigger;
        cv::Size imageSize;
        int imageBpp;
        int frame_numerator;
        int frame_denominator;
    };

	/*
	* @brief : parameter uplimit & downlimit
	*/
	struct ParamLimit
	{
		double uplimit, downlimit;
		bool flag;                  //返回值是否有效
	};

	/*
	* @brief : 参数配置方式枚举
	*/
	enum PARAM_MODEL
	{
		AUTO,               //自动
		AUTO_ONCE,          //一次采样自动调整，适用于增益方式设置, 曝光方式设置
		AUTO_CONTINUOUS,    //多次采样自动调整，适用于增益方式设置, 曝光方式设置
		MANUAllY,           //手动设置， 适用于增益设置, 曝光方式设置
		AUTO_OFF,           //关闭自动曝光
	};

    /*
    * @brief : 曝光模式
    */
	enum ExposureMode_evo
    {
        enumExposureMode_Timed,
        enumExposureMode_TriggerWidth

    };

	enum ShutterMode
    {
        enumShutterMode_Global,           //!<The shutter opens and closes at the same time for all pixels. (It depends on camear, may not supported)
        enumShutterMode_Rolling,          //!<The shutter opens and closes sequentially for groups (typically lines) of pixels.
        enumShutterMode_GlobalReset       //!<The shutter opens at the same time for all pixels but ends in a sequential manner.
    };

	/* 设置触发参数
	 * @brief : 触发控制参数
	 */
	//触发源控制位 低三位（0， 1， 2位
	//#define NONE             0x00
	#define LINE1            0x01                              //00000001， 外部LINE1触发
	#define LINE2            0x02                              //00000010， 外部LINE2触发
	#define SOFT             0x03                              //00000011， 软件触发
    #define AUTO             0x04                              //00000100， 自动触发

	//触发信号控制位 3，4位
	#define RISE             0x08                              //00001000， 上升沿触发
	#define FALL             0x10                              //00010000， 下降沿触发
//	#define ENABLE_EXT       0x08
//	#define DISABLE_EXT      0x00

	enum LIGHT_SOURCE
	{
		OFF,              //外部光源为0
		TUNGSTEN,         //2800 Kelvin
		Daylight_5000,    //5000 Kelvin
		Daylight_6500     //6500 Kelvin
	};

	class EVO_BE_CAMERAS_API CBE_Cameras
	{
	public:
		/* 构造函数重载
		 * @brief : 由于该函数一次获得两个相机的SN, 之后函数会自动绑定相机，不需要再调用attachCamera
		 * @param[in] : 相机ID
		 * @param[in] : 触发源设置参数，其值可以通过定义的宏组合生成，比如选择外部LINE1上升沿触发，使能外部触发 trigger = LINE1 + RISE + ENABLE_EXT
		 * @param[in] : 左相机的SN编号
		 * @param[in] : 右相机的SN编号
		 */
		static CBE_Cameras *create(CameraIndex type,
									int _trigger, 
									cv::Size s, 
									std::string _leftSN = "", 
									std::string _rightSN = "", 
									int frame_numerator = 25,
									int frame_denominator = 1,
									void *logger_ptr = NULL);

		virtual ~CBE_Cameras();

		/* 抓取图像
		* @brief : Grab newest image
		*/
		virtual bool grabImage(cv::Mat &left, cv::Mat &right, uint32_t &timestamp) = 0;
		virtual bool grabImage(cv::Mat &fullImage, uint32_t& timestamp) = 0;

		/* 开始抓取相机设置
		* @brief : 开始抓取相机设置
		*/
		virtual void startGrabbing() = 0;

		/* 停止抓取相机设置
		* @brief : 停止抓取相机设置
		*/
		virtual void stopGrabbing() = 0;

		/* 返回相机是否处于抓图状态
		* @brief : 返回相机是否处于抓图状态
		*/
		virtual bool isGrabbing() = 0;

        virtual void setTriggerFrameRate(int rate) = 0;

		///* 外部应用获取数据接口
		//* @brief : 外部应用获取数据接口
		//*/
		//void getImage(ImageColorType colorType, BE_ImageGeneralInfo &generalImage);
		//void getImage(CameraIndex type, ImageColorType colorType, cv::Mat &image0, cv::Mat &image1 = cv::Mat());

        virtual void setHighLightReduction(CameraIndex type, bool onoff) = 0;

		/* 触发源设置
		 * @brief : 触发源设置，包括触发源选择，触发信号类型，触发源使能, 设置触发之前相机必须要打开，并且相机关闭后设置失效
		 */
		virtual void setTrigger(CameraIndex type, int trigger) = 0;

        virtual void setShutterMode(CameraIndex type, ShutterMode mode) = 0;

		/* 相机设置增益
		* @brief : 设置相机增益
		* @param[in] : 相机选择
		* @param[in] : 相机增益的设置方式, mode = PARAM_MODEL::AUTO_ONCE 或 PARAM_MODEL::AUTO_CONTINUOUS 时自动调节增益，mode = PARAM_MODEL::MANUALLY是相机增益由第二个参数决定
		* @param[in] : 当相机增益的调节方式为MANUALLY时，设置的增益值
		*/
		virtual void setGain(CameraIndex type, PARAM_MODEL model, double p = 0.0) = 0;

		/*
		 * @brief : 获得增益设置范围，一次只能获取一只相机的增益
		 */
		virtual ParamLimit getGainRange(CameraIndex type) = 0;

		/* 设置自动增益的变化范围
		* @brief : 设置自动增益的变化范围
		* @param[in] : 相机选择
		* @param[in] : 自动增益变化的上限
		* @param[in] : 自动增益变化的下限
		*/
		virtual void setAutoGainLimit(CameraIndex type, double uplimit, double downlimit) = 0;

        virtual void setExposureAutoReference(CameraIndex type, int value = 70) = 0;

        virtual void setGainAutoLowerLimit(CameraIndex type, int value = 0) = 0;
        virtual void setGainAutoUpperLimit(CameraIndex type, int value = 480) = 0;

        virtual void setExposureAutoUpperLimitAuto(CameraIndex type, bool onoff) = 0;
        virtual void setExposureAutoLowerLimit(CameraIndex type, int value = 24) = 0;
        virtual void setExposureAutoUpperLimit(CameraIndex type, int value = 999999) = 0;

		/* 设置模拟信号的偏置
		* @brief : 模拟信号的偏置量，在SDK中称为black level
		* @param[in] : 相机选择
		* @param[in] : 偏移量
		*/
		virtual void blackLevel(CameraIndex type, double level = 0.0) = 0;

		/*
		* @brief : 获得模拟信号的偏置设置范围，一次只能获取一只相机的偏置
		*/
		virtual ParamLimit getBlackLevelRange(CameraIndex type) = 0;

		/* 设置图像尺寸, 必须在开始抓取前设置
		* @brief : 图像的采集尺寸
		* @param[in] : 相机选择
		* @param[in] : _height 图像高度
		* @param[in] : _width 图像宽度
		*/
		virtual void setSize(int _height, int _width) = 0;

		/*
		 * @brief : 获得相机图像高的设置范围，一次只能获取一只相机的相机尺寸
		 */
		virtual ParamLimit getHeightRange(CameraIndex type) = 0;

		/*
		 * @brief : 获得相机图像宽的设置范围，一次只能获取一只相机的相机尺寸
		 */
		virtual ParamLimit getWidthRange(CameraIndex type) = 0;

		/* 设置相机曝光
		* @brief : 设置相机曝光
		* @param[in] : 相机选择
		* @param[in] : 相机曝光的设置方式, mode = PARAM_MODEL::AUTO_ONCE 或 PARAM_MODEL::AUTO_CONTINUOUS 时自动调节曝光，mode = PARAM_MODEL::MANUALLY是相机曝光由第二个参数决定
		* @param[in] : 当相机曝光的调节方式为MANUALLY时，设置的曝光值
		*/
		virtual void setExposure(CameraIndex type, PARAM_MODEL model, double p = 15000.0) = 0;

        /* 获取相机当前曝光时间
        * @brief : 获取相机曝光时间
        * @param[in] : 相机选择（仅支持单个相机）
        */
        virtual double getExposureTime(CameraIndex type) = 0;

        /* 配置重叠曝光和图像读出
        * @brief : 配置重叠曝光和图像读出
        * @param[in] : 相机选择
        * @param[in] : 开启true和禁用false
        */
        virtual void setOverlapMode(CameraIndex type, bool flag) = 0;

        /* 启用“立即触发”模式
        * @brief : 启用“立即触发”模式
        * @param[in] : 相机选择
        * @param[in] : 开启true和禁用false
        */
        virtual void setImmediateTriggerMode(CameraIndex type, bool flag) = 0;

        virtual void setExposureMode(CameraIndex type, ExposureMode_evo mode) = 0;

		/*
		 * @brief : 获得相机曝光设置范围，一次只能获取一只相机的曝光
		 */
		virtual ParamLimit getExposureRange(CameraIndex type) = 0;

		/* 设置自动曝光的变化范围
		* @brief ：设置自动曝光的变化范围
		* @param[in] : 相机选择
		* @param[in] : 自动曝光变化的上限
		* @param[in] : 自动曝光变化的下限
		*/
		virtual void setAutoExposureLimit(CameraIndex type, double uplimit, double downlimit) = 0;

		/* 设置曝光图像帧是否重叠
		* @brief : 设置曝光图像帧是否重叠
		* @param[in] : 相机选择
		* @param[in] : 设置标准，flag == true 重叠, flag == false 不重叠
		*/
		virtual void setExposureOverlap(CameraIndex type, bool flag) = 0;

		/* 设置相机白平衡
		* @brief : 设置相机白平衡
		* @param[in] : 相机选择
		* @param[in] : 相机白平衡的设置方式, mode = PARAM_MODEL::AUTO_ONCE 或 PARAM_MODEL::AUTO_CONTINUOUS 时自动调节白平衡，mode = PARAM_MODEL::MANUALLY是相机白平衡由第二个参数决定
		* @param[in] : 当相机白平衡的调节方式为MANUALLY时，设置的白平衡值
		*/
		virtual void setWhiteBalance(CameraIndex type, PARAM_MODEL model, double p = 1.0) = 0;

		/*
		 * @brief : 获得相机白平衡设置范围，一次只能获取一只相机的白平衡
		 */
		virtual ParamLimit getWhiteBalanceRange(CameraIndex type) = 0;

		/* 设置水平方向图像翻转
		* @brief ： 设置水平方向图像翻转
		* @param[in] : 相机选择
		* @param[in] : 启用标志
		*/
		virtual void reverseX(CameraIndex type, bool flag) = 0;

		/* 设置垂直方向图像翻转
		* @brief ： 设置垂直方向图像翻转
		* @param[in] : 相机选择
		* @param[in] : 启用标志
		*/
		virtual void reverseY(CameraIndex type, bool flag) = 0;

		/* 水平偏置
		* @brief : 设置水平偏置
		* @param[in] : 相机选择
		* param[in] : 水平偏置量
		*/
		virtual void setOffX(CameraIndex type, int p) = 0;

		/*
		* @brief : 获得相机水平偏置设置范围，一次只能获取一只相机的水平偏置
		*/
		virtual ParamLimit getOffXRange(CameraIndex type) = 0;

		/* 垂直偏置
		* @brief : 设置垂直偏置
		* @param[in] : 相机选择
		* param[in] : 垂直偏置量
		*/
		virtual void setOffY(CameraIndex type, int p) = 0;

		/*
		* @brief : 获得相机垂直偏置设置范围，一次只能获取一只相机的垂直偏置
		*/
		virtual ParamLimit getOffYRange(CameraIndex type) = 0;

		/* 设置帧率
		* @brief : 设置帧率, 该函数设置帧率的允许值上限，具体帧率由实际情况决定
		* @param[in] : 相机选择
		* @param[in] : r 帧率值
		*/
		virtual void setAcquisitionRate(CameraIndex type, double r = 30.0) = 0;

		/*
		* @brief : 获得相机帧率设置范围，一次只能获取一只相机的帧率
		*/
		virtual ParamLimit getAcquisitionRateRange(CameraIndex type) = 0;

		/* 设置gamma校正值
		* @brief : 设置gamma校正值
		* @param[in] : 相机选择
		* param[in] : gamma值
		*/
		virtual void setGamma(CameraIndex type, double p = 1.0) = 0;

		/*
		* @brief : 获得相机Gamma校正设置范围，一次只能获取一只相机的Gamma校正范围
		*/
		virtual ParamLimit getGammaRange(CameraIndex type) = 0;

		/* 设置锐化增强
		* @brief : 设置锐化增强值
		* @param[in] : 相机选择
		* @param[in] : 锐化设置值
		*/
		virtual void setSharpnessEnhancement(CameraIndex type, double p = 0.0) = 0;

		/*
		* @brief : 获得相机锐化设置范围，一次只能获取一只相机的Gamma锐化范围
		*/
		virtual void setSharpnessEnhancementRange(CameraIndex type, double &uplimit, double &downlimit) = 0;

		/* 预设照明光源
		* @brief : 预设照明光源
		* @param[in] : 相机选择
		* @param[in] : 预设光源模型
		*/
		virtual void presetLightSource(CameraIndex type, LIGHT_SOURCE model) = 0;

		/* 目标亮度设置
		* @brief : 设置目标亮度
		* @param[in] : 相机选择
		* @param[in] : 目标亮度值
		*/
		virtual void setTargetLightness(CameraIndex type, double p = 0.3) = 0;

		/*
		* @brief : 获得目标亮度设置范围，一次只能获取一只相机的目标亮度设置范围
		*/
		virtual ParamLimit getTargetLightnessRange(CameraIndex type) = 0;

		/* 保存配置参数
		* @brief : 保存配置参数
		* @param[in] : 相机选择
		* @param[in] : 配置参数文件名, 只存在一只相机时，保存的文件名与filename相同
		*/
		virtual void saveConfig(CameraIndex type, std::string left = "left_conf.pfs", std::string right = "right_conf.pfs") = 0;

		/* 载入配置参数
		* @brief : 载入配置参数
		* @param[in] : 相机选择
		* @param[in] : 配置参数文件名, 只存在一只相机时，保存的文件名与filename相同
		*/
		virtual void loadConfig(CameraIndex type, std::string left = "left_conf.pfs", std::string right = "right_conf.pfs") = 0;

		virtual double getCurrentFrameRate() const = 0;

		virtual void focusTool(CameraIndex type) = 0;
	};

	class EVO_BE_CAMERAS_API CBE_Cameras2
    {
    public:
        /* 构造函数重载
        * @brief : 由于该函数一次获得两个相机的SN, 之后函数会自动绑定相机，不需要再调用attachCamera
        * @param[in] : 相机ID
        * @param[in] : 触发源设置参数，其值可以通过定义的宏组合生成，比如选择外部LINE1上升沿触发，使能外部触发 trigger = LINE1 + RISE + ENABLE_EXT
        * @param[in] : 左相机的SN编号
        * @param[in] : 右相机的SN编号
        */
        static CBE_Cameras2 *create(
                std::vector<BECameraInfo> &cam,
                void *logger_ptr = NULL);

        virtual ~CBE_Cameras2();

        /* 抓取图像
        * @brief : Grab newest image
        */
        virtual bool grabImage(cv::Mat image[], uint32_t &timestamp) = 0;

        /* 开始抓取相机设置
        * @brief : 开始抓取相机设置
        */
        virtual void startGrabbing(int type) = 0;

        /* 停止抓取相机设置
        * @brief : 停止抓取相机设置
        */
        virtual void stopGrabbing(int type) = 0;

        /* 返回相机是否处于抓图状态
        * @brief : 返回相机是否处于抓图状态
        */
        virtual bool isGrabbing(int type) = 0;

        virtual void setTriggerFrameRate(int rate) = 0;

        ///* 外部应用获取数据接口
        //* @brief : 外部应用获取数据接口
        //*/
        //void getImage(ImageColorType colorType, BE_ImageGeneralInfo &generalImage);
        //void getImage(CameraIndex type, ImageColorType colorType, cv::Mat &image0, cv::Mat &image1 = cv::Mat());

        /*
        * @brief :
        */
        virtual void setHighLightReduction(int type, bool onoff) = 0;

        virtual void setImxLowLatency(int type, bool onoff) = 0;
        /* 触发源设置
        * @brief : 触发源设置，包括触发源选择，触发信号类型，触发源使能, 设置触发之前相机必须要打开，并且相机关闭后设置失效
        */
        virtual void setTrigger(int type, int trigger) = 0;


        virtual void setShutterMode(int type, ShutterMode mode) = 0;

        /* 相机设置增益
        * @brief : 设置相机增益
        * @param[in] : 相机选择
        * @param[in] : 相机增益的设置方式, mode = PARAM_MODEL::AUTO_ONCE 或 PARAM_MODEL::AUTO_CONTINUOUS 时自动调节增益，mode = PARAM_MODEL::MANUALLY是相机增益由第二个参数决定
        * @param[in] : 当相机增益的调节方式为MANUALLY时，设置的增益值
        */
        virtual void setGain(int type, PARAM_MODEL model, double p = 0.0) = 0;

        /*
        * @brief : 获得增益设置范围，一次只能获取一只相机的增益
        */
        virtual ParamLimit getGainRange(int type) = 0;

        /* 设置自动增益的变化范围
        * @brief : 设置自动增益的变化范围
        * @param[in] : 相机选择
        * @param[in] : 自动增益变化的上限
        * @param[in] : 自动增益变化的下限
        */
        virtual void setAutoGainLimit(int type, double uplimit, double downlimit) = 0;

        virtual void setExposureAutoReference(int type, int value = 70) = 0;

        virtual void setGainAutoLowerLimit(int type, int value = 0) = 0;
        virtual void setGainAutoUpperLimit(int type, int value = 480) = 0;

        virtual void setExposureAutoUpperLimitAuto(int type, bool onoff) = 0;
        virtual void setExposureAutoLowerLimit(int type, int value = 24) = 0;
        virtual void setExposureAutoUpperLimit(int type, int value = 999999) = 0;

        /* 设置模拟信号的偏置
        * @brief : 模拟信号的偏置量，在SDK中称为black level
        * @param[in] : 相机选择
        * @param[in] : 偏移量
        */
        virtual void blackLevel(int type, double level = 0.0) = 0;

        /*
        * @brief : 获得模拟信号的偏置设置范围，一次只能获取一只相机的偏置
        */
        virtual ParamLimit getBlackLevelRange(int type) = 0;

        /* 设置图像尺寸, 必须在开始抓取前设置
        * @brief : 图像的采集尺寸
        * @param[in] : 相机选择
        * @param[in] : _height 图像高度
        * @param[in] : _width 图像宽度
        */
        virtual void setSize(int _height, int _width) = 0;

        /*
        * @brief : 获得相机图像高的设置范围，一次只能获取一只相机的相机尺寸
        */
        virtual ParamLimit getHeightRange(int type) = 0;

        /*
        * @brief : 获得相机图像宽的设置范围，一次只能获取一只相机的相机尺寸
        */
        virtual ParamLimit getWidthRange(int type) = 0;

        /* 设置相机曝光
        * @brief : 设置相机曝光
        * @param[in] : 相机选择
        * @param[in] : 相机曝光的设置方式, mode = PARAM_MODEL::AUTO_ONCE 或 PARAM_MODEL::AUTO_CONTINUOUS 时自动调节曝光，mode = PARAM_MODEL::MANUALLY是相机曝光由第二个参数决定
        * @param[in] : 当相机曝光的调节方式为MANUALLY时，设置的曝光值
        */
        virtual void setExposure(int type, PARAM_MODEL model, double p = 3000.0) = 0;

        /* 获取相机当前曝光时间
        * @brief : 获取相机曝光时间
        * @param[in] : 相机选择（仅支持单个相机）
        */
        virtual double getExposureTime(int type) = 0;

        virtual void setOverlapMode(int type, bool flag) = 0;

        virtual void setImmediateTriggerMode(int type, bool flag) = 0;

        virtual void setExposureMode(int type, ExposureMode_evo mode) = 0;

        /*
        * @brief : 获得相机曝光设置范围，一次只能获取一只相机的曝光
        */
        virtual ParamLimit getExposureRange(int type) = 0;

        /* 设置自动曝光的变化范围
        * @brief ：设置自动曝光的变化范围
        * @param[in] : 相机选择
        * @param[in] : 自动曝光变化的上限
        * @param[in] : 自动曝光变化的下限
        */
        virtual void setAutoExposureLimit(int type, double uplimit, double downlimit) = 0;


        /* 设置曝光图像帧是否重叠
        * @brief : 设置曝光图像帧是否重叠
        * @param[in] : 相机选择
        * @param[in] : 设置标准，flag == true 重叠, flag == false 不重叠
        */
        virtual void setExposureOverlap(int type, bool flag) = 0;

        /* 设置相机白平衡
        * @brief : 设置相机白平衡
        * @param[in] : 相机选择
        * @param[in] : 相机白平衡的设置方式, mode = PARAM_MODEL::AUTO_ONCE 或 PARAM_MODEL::AUTO_CONTINUOUS 时自动调节白平衡，mode = PARAM_MODEL::MANUALLY是相机白平衡由第二个参数决定
        * @param[in] : 当相机白平衡的调节方式为MANUALLY时，设置的白平衡值(2500~10000)
        */
        virtual void setWhiteBalance(int type, PARAM_MODEL model, double p = 1.0) = 0;

        /*
        * @brief : 获得相机白平衡设置范围，一次只能获取一只相机的白平衡
        */
        virtual ParamLimit getWhiteBalanceRange(int type) = 0;

        /* 设置水平方向图像翻转
        * @brief ： 设置水平方向图像翻转
        * @param[in] : 相机选择
        * @param[in] : 启用标志
        */
        virtual void reverseX(int type, bool flag) = 0;

        /* 设置垂直方向图像翻转
        * @brief ： 设置垂直方向图像翻转
        * @param[in] : 相机选择
        * @param[in] : 启用标志
        */
        virtual void reverseY(int type, bool flag) = 0;

        /* 水平偏置
        * @brief : 设置水平偏置
        * @param[in] : 相机选择
        * param[in] : 水平偏置量
        */
        virtual void setOffX(int type, int p) = 0;

        /*
        * @brief : 获得相机水平偏置设置范围，一次只能获取一只相机的水平偏置
        */
        virtual ParamLimit getOffXRange(int type) = 0;

        /* 垂直偏置
        * @brief : 设置垂直偏置
        * @param[in] : 相机选择
        * param[in] : 垂直偏置量
        */
        virtual void setOffY(int type, int p) = 0;

        /*
        * @brief : 获得相机垂直偏置设置范围，一次只能获取一只相机的垂直偏置
        */
        virtual ParamLimit getOffYRange(int type) = 0;

        /* 设置帧率
        * @brief : 设置帧率, 该函数设置帧率的允许值上限，具体帧率由实际情况决定
        * @param[in] : 相机选择
        * @param[in] : r 帧率值
        */
        virtual void setAcquisitionRate(int type, double r = 30.0) = 0;

        /*
        * @brief : 获得相机帧率设置范围，一次只能获取一只相机的帧率
        */
        virtual ParamLimit getAcquisitionRateRange(int type) = 0;

        /* 设置gamma校正值
        * @brief : 设置gamma校正值
        * @param[in] : 相机选择
        * param[in] : gamma值
        */
        virtual void setGamma(int type, double p = 1.0) = 0;

        /*
        * @brief : 获得相机Gamma校正设置范围，一次只能获取一只相机的Gamma校正范围
        */
        virtual ParamLimit getGammaRange(int type) = 0;

        /* 设置锐化增强
        * @brief : 设置锐化增强值
        * @param[in] : 相机选择
        * @param[in] : 锐化设置值
        */
        virtual void setSharpnessEnhancement(int type, double p = 0.0) = 0;

        /*
        * @brief : 获得相机锐化设置范围，一次只能获取一只相机的Gamma锐化范围
        */
        virtual void setSharpnessEnhancementRange(int type, double &uplimit, double &downlimit) = 0;

        /* 预设照明光源
        * @brief : 预设照明光源
        * @param[in] : 相机选择
        * @param[in] : 预设光源模型
        */
        virtual void presetLightSource(int type, LIGHT_SOURCE model) = 0;

        /* 目标亮度设置
        * @brief : 设置目标亮度
        * @param[in] : 相机选择
        * @param[in] : 目标亮度值
        */
        virtual void setTargetLightness(int type, double p = 0.3) = 0;

        /*
        * @brief : 获得目标亮度设置范围，一次只能获取一只相机的目标亮度设置范围
        */
        virtual ParamLimit getTargetLightnessRange(int type) = 0;

        /* 保存配置参数
        * @brief : 保存配置参数
        * @param[in] : 相机选择
        * @param[in] : 配置参数文件名, 只存在一只相机时，保存的文件名与filename相同
        */
        virtual void saveConfig(int type, std::string left = "left_conf.pfs", std::string right = "right_conf.pfs") = 0;

        /* 载入配置参数
        * @brief : 载入配置参数
        * @param[in] : 相机选择
        * @param[in] : 配置参数文件名, 只存在一只相机时，保存的文件名与filename相同
        */
        virtual void loadConfig(int type, std::string left = "left_conf.pfs", std::string right = "right_conf.pfs") = 0;

        virtual double getCurrentFrameRate() const = 0;

        virtual void focusTool(int type) = 0;
    };

}
#endif // ! __BE_CAMERAS_HPP__