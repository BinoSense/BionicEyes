/*****************************************************************************
*  Eyevolution BionicEye series products SDK.\n
*  Copyright (C) 2018, Eyevolution, all rights reserved.
*
* @brief This is for internal use of bionic eye device, including configure file
 * system， log file system, debug info system, internal common variate declaration and so on...
* @author Openking <wang.kaifang@ilooktech.com>
* @date <2017/11/20>
* @note This is only for Eyevolution Bionic Eye generation III.
*****************************************************************************/

#ifndef _EVO_BE_COMMON_INTERNAL_H_
#define _EVO_BE_COMMON_INTERNAL_H_

#include <sstream>
#include <deque>
#include <vector>
#include <typeinfo>
#include <opencv2/core/core.hpp>

#include <evo_be.h>

using namespace std;

namespace evo_be
{
	static const int MAX_PATH_STRLENS = 128;

	enum BeFrameType
	{
		enumBE_ControlFrame = 0,
		enumBE_RequestFrame = 1,
		enumBE_ReplyFrame = 2,
	};

	enum BeFrameCommandType
	{
		enumBE_SetMotorAbsolutePosition = 0x20,
		enumBE_SetMotorRelativePosition = 0x21,
		enumBE_SetMotorSpaceAbsPosition = 0x22,
		enumBE_SetVorOnOff = 0x23,
		enumBE_SetMotorMoveToInitPos = 0x24,
		enumBE_SetSvOnOff = 0x25,
		enumBE_SetSvModel = 0x26,
		enumBE_SetSyncSignalSource = 0x27,
		enumBE_SetNeckMotorAbsolutePosition = 0x28,
		enumBE_SetNeckMotorRelativePosition = 0x29,
		enumBE_SetEyeNeckUnionAbsPos = 0x2A,  // set eye and neck union absolute position
		enumBE_SetEyeNeckUnionRelaPos = 0x2B, // set eye and neck union relative position
		enumBE_SetImageTransferResolution = 0x2C,
		enumBE_SetImageTransferColorType = 0x2D,
		enumBE_SetImageTransferRate = 0x2E,
		enumBE_SetImageCompressionQuality = 0x2F,
		enumBE_SetCameraExposure = 0x30,
		enumBE_SetNowPosAsInitPos = 0x31,
		enumBE_SetCameraExposureAutoReference = 0x32,
		enumBE_SetCameraGain = 0x33,
		enumBE_SetMotorAbsoluteSpeed = 0x34,
		enumBE_SetBeDataRate = 0x35,
        enumBE_SetCameraWhiteBalance = 0x36,
        enumBE_SetSyncCameraDelayTime = 0x37,
        enumBE_SetCameraImageOnOff = 0x38,
        enumBE_SetInfraredFillingLamp = 0x39,
        enumBE_SetLaserRangefinder = 0x40,
		enumBE_SetWipers = 0x41,
		enumBE_SetZoomFocalLength = 0x42,
		enumBE_SetExtraModule = 0x43,

		enumBE_GetMotorInitPos = 0x60,
		enumBE_GetMotorUpDownLimit = 0x61,
		enumBE_GetHaveNeck = 0x62,
		enumBE_GetMotorInitPos_Neck = 0x63,
		enumBE_GetMotorUpDownLimit_Neck = 0x64,
		enumBE_GetOrignalImageResolution = 0x65,
		enumBE_GetMaxImageFrameRate = 0x66,
		enumBE_GetCameraPara = 0x67,
	};

	// local data server data sources
	enum BeLocalServerType
	{
		enumBE_FakeServer = 0,
		enumBE_RealDataServer = 1,
	};

#pragma pack(1)

    struct MotorDataStru
    {
        float moveUplimit;	 // degree
        float moveDownlimit; // degree
        float initPosition;	 // motor initial position (in degree)
        float pid_VOR[5];
    };

	/**
    * @brief Bionic Eye calibration data info
    */
	struct BE_CalibData
	{
		cv::Matx33d K[2];
		cv::Matx<double, 5, 1> D[2];
		cv::Matx33d R[7];
		cv::Matx31d t[7];
		std::string motors[3];
		std::string eyes[2];

		// Caller is responsible for freeing the returned buffer.
		uchar *encoder()
		{
			uchar *data = new uchar[896];
			memcpy(data, this, 896);
			return data;
		};
		void decoder(uchar *data)
		{
			assert(data);
			memcpy(this, data, 896);
			for (int g = 0; g < 2; g++)
				this->eyes[g] = evo_be::EyeTags[g];
			for (int g = 0; g < 3; g++)
				this->motors[g] = evo_be::MotorTags[g];
		};
	};

	struct Be_Frame
	{
		Be_Frame(){};
		Be_Frame(BeFrameType fType, BeFrameCommandType cType) : frameType(fType), timeStamp(0), CommandType(cType){};
		Be_Frame(BeFrameType fType, uint32_t timestamp, BeFrameCommandType cType) : frameType(fType), timeStamp(timestamp), CommandType(cType){};
		BeFrameType frameType;
		uint32_t timeStamp; // timestamp (100us)
		BeFrameCommandType CommandType;
		uchar data[96];
	};

	struct Be_Data_Control
	{
		bool motor_flag[6];
		float motor_val[6];
		float motor_move_val[6];
		float imu_val[3];
		int _moveType;
		int _movebaseType;
		uchar flag;
	};

	struct Be_ExtraModuleData_Control
	{
		BE_ExtraModuleType type;
		int para0;
		int para1;
		int para2;
	};

	struct Be_data_NeckEye_Control
	{
		bool motor_flag[3];
		float motor_val[3];
		float neck_val[3];
	};

    struct Be_SensorControl
    {
        bool flag_onoff;
        uchar value;
    };

	struct Be_SV_TransStru
	{
		uint8_t flag;
		uint8_t type;
		cv::Rect rect;
		cv::Size size;
	};

	struct Be_Data_Limit
	{
		float UpLimit[6];
		float DownLimit[6];
	};

	struct Be_ImageTransferSet
	{
		cv::Size size;
		ImageColorType type;
		float transRate;
		int transImageQuality;
		int beDataRate;
		int timeDelay;
        bool camera_flag[MAXCAMERASInDEVICE];
        bool onlyNetworkType;
	};
	struct Be_CameraParaSet
	{
		bool isAuto;
		int cameraType;
		float exposureTime;
		int exposureAutoReference;
		float upLimit;
		float downLimit;
		int gainValue;
	};

	struct Be_LocalServer_info
	{
		BeLocalServerType serverDataType;
		BE_IpInfo dataSourceInfo;
		char configFilePath[MAX_PATH_STRLENS];
	};
#pragma pack()

#ifdef _WIN32
#ifdef EVO_BE_SERVICE_EXPORTS
#define EVO_BE_SERVICE_API __declspec(dllexport)
#else
#define EVO_BE_SERVICE_API __declspec(dllimport)
#endif // EVO_BE_SERVICE_EXPORTS
#else
#define EVO_BE_SERVICE_API
#endif // _WIN32

	/**  Bionic eye device special services
	* @brief including: Device address acquisition  &&&  remote file transfer
	*/
	class EVO_BE_SERVICE_API CBE_Service
	{
	public:
		static CBE_Service *create();
		virtual ~CBE_Service();

		/**  start request to trans file (use this before **sendFile** and **recvFile**)
		*/
		virtual void requestToTransFile(string ip) = 0;

		/**  start request to get device ip address (use this before **GetIpAddr**)
		*/
		virtual void requestToGetDeviceIpAddress() = 0;

		/**  send local file to device and save to specified path
		* @param[in] fileName Local file path
		*/
		virtual bool sendFile(char *fileName,bool x=false) = 0;

		/**  recv remote file(specified path) and save to local path
		* @param[in] fileName Local file path
		*/
		virtual bool recvFile(char *fileName,bool x=false) = 0;

		/**  Get remote bionic eye device ip address
		*/
		virtual bool GetIpAddr(vector<BE_IpInfo> &ipInfo) = 0;

		virtual string ipAddressTran(unsigned int ipaddr) = 0;

		virtual uint32_t ipAddressTran(string ipaddr) = 0;
	};

	/**  XML数据类型枚举
	* @brief XML数据类型枚举， EVO_NON标记非数据结点
	*/
	enum XMLDataType
	{
		enumEvoBeFloat,	  // 单精度浮点数
		enumEvoBeDouble,  // 双精度浮点数
		enumEvoBeInt,	  // int型整数
		enumEvoBeString,  // 字符串
		enumEvoBeMat,	  // cv::Mat
		enumEvoBePoint,	  // cv::Ponit
		enumEvoBePoint3,  // cv::Point3
		enumEvoBeSize,	  // cv::Size
		enumEvoBeComplex, // cv::Complex
		enumEvoBeRect,	  // cv::Rect
		enumEvoBeVec,	  // cv::Vec
		enumEvoBeScalar,  // cv::Scalar
		enumEvoBeRange,	  // cv::Range
		enumEvoBeNon,	  // 非数据节点
	};

	/** 节点读写等级枚举
	* @brief 节点读写等级枚举
	*/
	enum UserLevelType
	{
		enumUserL_Admin,
		enumUserL_DevelopIn,
		enumUserL_DevelopEx,
		enumUserL_USER,
		enumUserL_GUEST,
	};

#ifdef _WIN32
#ifdef EVO_BE_CONFIGFILE_EXPORTS
#define EVO_BE_CONFIGFILE_API __declspec(dllexport)
#else
#define EVO_BE_CONFIGFILE_API __declspec(dllimport)
#endif // EVO_BE_CONFIGFILE_EXPORTS
#else
#define EVO_BE_CONFIGFILE_API
#endif // _WIN32

	class CBE_ConfigFileVars;
	class EVO_BE_CONFIGFILE_API CBE_ConfigFile
	{
	public:
		static CBE_ConfigFile *getInstance(void *logger_ptr = NULL);

        void delete_object();

		void checkLevel(void *ele, UserLevelType level);

		bool xmlLoad(string xmlname, bool useEncryption = true);

		/**  读取结点数据
		* @brief 读取结点数据
		* @param[in] ele 待读入的结点
		* @param[in] value 读入数值的存储的位置，因此该输入应该是个左值
		* @param[in] defalut_value 读入失败时默认的值
		*/
		void read(void *ele, int &value, UserLevelType level, int defalut_value = 0);

		/**  读取结点数据
		* @brief read的重载
		*/
		void read(void *ele, float &value, UserLevelType level, float default_value = 0.0f);

		/**  读取结点数据
		* @brief read的重载
		*/
		void read(void *ele, double &value, UserLevelType level, double default_value = 0.0);

		/**  读取结点数据
		* @brief read的重载
		*/
		void read(void *ele, string &value, UserLevelType level, const string &default_value = "");

		/**  读取结点数据
		* @brief read的重载
		*/
		void read(void *ele, cv::Mat &mat, UserLevelType level, const cv::Mat &defalut_mat = cv::Mat());

		/**  读取结点数据
		* @brief read的重载
		*/
		template <typename _Tp>
		void read(void *ele, cv::Point_<_Tp> &value, UserLevelType level, const cv::Point_<_Tp> &default_value = cv::Point_<_Tp>());

		/**  读取结点数据
		* @brief read的重载
		*/
		template <typename _Tp>
		void read(void *ele, cv::Point3_<_Tp> &value, UserLevelType level, const cv::Point3_<_Tp> &default_value = cv::Point3_<_Tp>());

		/**  读取结点数据
		* @brief read的重载
		*/
		template <typename _Tp>
		void read(void *ele, cv::Size_<_Tp> &value, UserLevelType level, const cv::Size_<_Tp> &default_value = cv::Size_<_Tp>());

		/**  读取结点数据
		* @brief read的重载
		*/
		template <typename _Tp>
		void read(void *ele, cv::Complex<_Tp> &value, UserLevelType level, const cv::Complex<_Tp> &default_value = cv::Complex<_Tp>());

		/**  读取结点数据
		* @brief read的重载
		*/
		template <typename _Tp>
		void read(void *ele, cv::Rect_<_Tp> &value, UserLevelType level, const cv::Rect_<_Tp> &default_value = cv::Rect_<_Tp>());

		/**  读取结点数据
		* @brief read的重载
		*/

		template <typename _Tp, int cn>
		void read(void *ele, cv::Vec<_Tp, cn> &value, UserLevelType level, const cv::Vec<_Tp, cn> &default_value = cv::Vec<_Tp, cn>());

		/**  读取结点数据
		* @brief read的重载
		*/
		template <typename _Tp>
		void read(void *ele, cv::Scalar_<_Tp> &value, UserLevelType level, const cv::Scalar_<_Tp> &default_value = cv::Scalar_<_Tp>());

		/**  读取结点数据
		* @brief read的重载
		*/
		void read(void *ele, cv::Range &value, UserLevelType level, const cv::Range &default_value = cv::Range());

		std::string getAttribute(void *ele, std::string attr_name);

		std::string rawRead(void *ele);

		std::string getNodeName(void *ele);

		void *getRoot();

		void setAttribute(void *ele, std::string attr_name, std::string attr_value);

		void *getFirstChild(void *ele);

		void *getNextBrother(void *ele);

		void *getParent(void *ele);

		void *searchNode(vector<string> node_list); //结点查找

		/** 修改数据结点, 保证对应结点存储的是相应类型的数据是调用者的责任
		* @brief 修改节点数据
		* @param[in] ele : 待修改的结点，这个结点必须在xml树中已经出现，如果需要添加结点，请使用函数addNode()
		* @param[in] value : 修改后结点的值
		*/
		bool modifyNode(void *ele, int &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		bool modifyNode(void *ele, float &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		bool modifyNode(void *ele, double &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		bool modifyNode(void *ele, string &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		bool modifyNode(void *ele, cv::Mat &mat, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		template <typename _Tp>
		bool modifyNode(void *ele, cv::Point_<_Tp> &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		template <typename _Tp>
		bool modifyNode(void *ele, cv::Point3_<_Tp> &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		template <typename _Tp>
		bool modifyNode(void *ele, cv::Size_<_Tp> &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		template <typename _Tp>
		bool modifyNode(void *ele, cv::Complex<_Tp> &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		template <typename _Tp>
		bool modifyNode(void *ele, cv::Rect_<_Tp> &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		template <typename _Tp, int cn>
		bool modifyNode(void *ele, cv::Vec<_Tp, cn> &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		template <typename _Tp>
		bool modifyNode(void *ele, cv::Scalar_<_Tp> &value, UserLevelType level);

		/**  读取结点数据
		* @brief modifyNode的重载
		*/
		bool modifyNode(void *ele, cv::Range &value, UserLevelType level);

		/** 删除节点
		* @param[in] ele : 待删除的节点
		*/
		bool deleteNode(void *ele);

		/** 保存文件
		* @brief 保持内存中的xml文件结构
		*/
		int save(bool useEncryption = true);

		int save(string filename1, bool useEncryption = true);

		/** 打印树
		* @brief 打印XML树的结构，从根结点开始打印
		*/
		void printTree();

		/** printTree重载
		* @brief printTree重载，从输入节点开始打印
		* @param[in] ele : 打印的起始节点
		*/
		void printTree(void *ele);

		/** printTree 重载
		*
		*/

		/** 添加结点
		* @brief 添加节点到已有的XML树中
		* @param[in] path : 添加节点的完整路径
		*/
		void *addNode(vector<string> path);

		bool addNodeWithFlag(std::vector<std::string> path, void *&ele);

		/*
		* @param[out] : 0 成功读取数据， 1 节点不存在 2 节点和数据类型不匹配
		*/
		template <class _Tp>
		int readFromNodeName(std::string node, _Tp &value, UserLevelType level);

		/** 查询节点的完整路径
		* @brief 查询节点完整路径
		* @param[in] node : 待查询节点的指针
		*/
		vector<string> findFullPath(void *node);

		/** 为节点设置读写等级
		* @brief 为节点设置读写等级
		* @param[in] node : 待设置的节点
		* @param[in] level : 将要设置成的等级，数值越小等级越低
		*/
		void setLevel(void *node, UserLevelType level);

		/** 修改孩子节点位置
		* @brief 修改孩子节点位置
		*
		*/
		void reorderNode(void *parent, void *child, size_t pos);

		/** 修改节点名字
		* @brief 修改节点名字
		* @param[in] : 待修改节点指针
		* @param[in] : 修改后节点名字
		*/
		void renameNode(void *node, string name);

		/** 节点移动
		* @brief
		*
		*/
		void nodeMove(void *ele, int dir);

	private:
		//隐藏默认构造，拷贝构造，拷贝赋值运算符及析构
        CBE_ConfigFile(void *logger_ptr = NULL);
        CBE_ConfigFile();
		CBE_ConfigFile(CBE_ConfigFile const &){};
		CBE_ConfigFile &operator=(CBE_ConfigFile const &);
		~CBE_ConfigFile();

		CBE_ConfigFileVars *vars;

        bool logger_ptr_released;
		void * logger;

		string getText(void *ele);
		void setText(void *ele, string value_str);
		void setAttr(void *ele, XMLDataType type);

		template <typename _Tp>
		bool checkType(XMLDataType node_type, _Tp &val);
	};

	template <typename _Tp>
	void CBE_ConfigFile::read(void *ele, cv::Point_<_Tp> &value, UserLevelType level, const cv::Point_<_Tp> &default_value)
	{
		checkLevel(ele, level);

		if (!ele)
		{
			value = default_value;
			return;
		}

		std::string data(getText(ele));
		std::string num;
		std::istringstream data_stream(data);
		size_t idx = 0;
		_Tp data_value[2] = {0};
		while (data_stream >> num && idx < 2)
		{
			data_value[idx] = (_Tp)std::stod(num);
			++idx;
		}

		value.x = data_value[0];
		value.y = data_value[1];
	}

	template <typename _Tp>
	void CBE_ConfigFile::read(void *ele, cv::Point3_<_Tp> &value, UserLevelType level, const cv::Point3_<_Tp> &default_value)
	{
		checkLevel(ele, level);

		if (!ele)
		{
			value = default_value;
			return;
		}

		std::string data(getText(ele));
		std::string num;
		std::istringstream data_stream(data);
		size_t idx = 0;
		_Tp data_value[3] = {0};
		while (data_stream >> num && idx < 3)
		{
			data_value[idx] = (_Tp)std::stod(num);
			++idx;
		}

		value.x = data_value[0];
		value.y = data_value[1];
		value.z = data_value[2];
	}

	template <typename _Tp>
	void CBE_ConfigFile::read(void *ele, cv::Size_<_Tp> &value, UserLevelType level, const cv::Size_<_Tp> &default_value)
	{
		checkLevel(ele, level);

		if (!ele)
		{
			value = default_value;
			return;
		}

		std::string data(getText(ele));
		std::string num;
		std::istringstream data_stream(data);
		size_t idx = 0;
		_Tp data_value[2] = {0};
		while (data_stream >> num && idx < 2)
		{
			data_value[idx] = (_Tp)std::stod(num);
			++idx;
		}

		//先存width, 后存height
		value.width = data_value[0];
		value.height = data_value[1];
	}

	template <typename _Tp>
	void CBE_ConfigFile::read(void *ele, cv::Complex<_Tp> &value, UserLevelType level, const cv::Complex<_Tp> &default_value)
	{
		checkLevel(ele, level);

		if (!ele)
		{
			value = default_value;
			return;
		}

		std::string data(getText(ele));
		std::string num;
		std::istringstream data_stream(data);
		size_t idx = 0;
		_Tp data_value[2] = {0};
		while (data_stream >> num && idx < 2)
		{
			data_value[idx] = (_Tp)std::stod(num);
			++idx;
		}

		//先存re, 后存im
		value.re = data_value[0];
		value.im = data_value[1];
	}

	template <typename _Tp>
	void CBE_ConfigFile::read(void *ele, cv::Rect_<_Tp> &value, UserLevelType level, const cv::Rect_<_Tp> &default_value)
	{
		checkLevel(ele, level);

		if (!ele)
		{
			value = default_value;
			return;
		}

		std::string data(getText(ele));
		std::string num;
		std::istringstream data_stream(data);
		size_t idx = 0;
		_Tp data_value[4] = {0};
		while (data_stream >> num && idx < 4)
		{
			data_value[idx] = (_Tp)std::stod(num);
			++idx;
		}

		//x, y, width, height
		value.x = data_value[0];
		value.y = data_value[1];
		value.width = data_value[2];
		value.height = data_value[3];
	}

	template <typename _Tp, int cn>
	void CBE_ConfigFile::read(void *ele, cv::Vec<_Tp, cn> &value, UserLevelType level, const cv::Vec<_Tp, cn> &default_value)
	{
		checkLevel(ele, level);

		if (!ele)
		{
			value = default_value;
			return;
		}

		std::string data(getText(ele));
		std::string num;
		std::istringstream data_stream(data);
		size_t idx = 0; //_Tp data[2] = { 0 };
		while (data_stream >> num && idx < cn)
		{
			value[idx] = (_Tp)std::stod(num);
			++idx;
		}
	}

	template <typename _Tp>
	void CBE_ConfigFile::read(void *ele, cv::Scalar_<_Tp> &value, UserLevelType level, const cv::Scalar_<_Tp> &default_value)
	{
		checkLevel(ele, level);

		if (!ele)
		{
			value = default_value;
			return;
		}

		std::string data(getText(ele));
		std::string num;
		std::istringstream data_stream(data);
		size_t idx = 0; //_Tp data[2] = { 0 };
		while (data_stream >> num && idx < 4)
		{
			value[idx] = std::stod(num);
			++idx;
		}
	}

	template <typename _Tp>
	bool CBE_ConfigFile::modifyNode(void *ele, cv::Point_<_Tp> &value, UserLevelType level)
	{
		checkLevel(ele, level);

		if (ele == NULL)
			return false;

		setAttr(ele, enumEvoBePoint);

		std::string value_str;
		double d_x = value.x, d_y = value.y;

		std::stringstream ss;
		ss << d_x << " " << d_y;
		value_str = ss.str();

		setText(ele, value_str);
		return true;
	}

	template <typename _Tp>
	bool CBE_ConfigFile::modifyNode(void *ele, cv::Point3_<_Tp> &value, UserLevelType level)
	{
		checkLevel(ele, level);

		if (ele == NULL)
			return false;

		setAttr(ele, enumEvoBePoint3);

		std::string value_str;
		double d_x = value.x, d_y = value.y, d_z = value.z;

		std::stringstream ss;
		ss << d_x << " " << d_y << " " << d_z;
		value_str = ss.str();

		setText(ele, value_str);
		return true;
	}

	template <typename _Tp>
	bool CBE_ConfigFile::modifyNode(void *ele, cv::Size_<_Tp> &value, UserLevelType level)
	{
		checkLevel(ele, level);

		if (ele == NULL)
			return false;

		setAttr(ele, enumEvoBeSize);

		std::string value_str;
		char aux[30];
		double d_w = value.width, d_h = value.height;

		std::stringstream ss;
		ss << d_w << " " << d_h;
		value_str = ss.str();

		setText(ele, value_str);
		return true;
	}

	template <typename _Tp>
	bool CBE_ConfigFile::modifyNode(void *ele, cv::Complex<_Tp> &value, UserLevelType level)
	{
		checkLevel(ele, level);

		if (ele == NULL)
			return false;

		setAttr(ele, enumEvoBeComplex);

		std::string value_str;
		double d_r = value.re, d_i = value.im;

		std::stringstream ss;
		ss << d_r << " " << d_i;
		value_str = ss.str();

		setText(ele, value_str);
		return true;
	}

	template <typename _Tp>
	bool CBE_ConfigFile::modifyNode(void *ele, cv::Rect_<_Tp> &value, UserLevelType level)
	{
		checkLevel(ele, level);

		if (ele == NULL)
			return false;

		setAttr(ele, enumEvoBeRect);

		std::string value_str;
		double d_x = value.x, d_y = value.y, d_w = value.width, d_h = value.height;

		std::stringstream ss;
		ss << d_x << " " << d_y << " " << d_w << " " << d_h;
		value_str = ss.str();

		setText(ele, value_str);
		return true;
	}

	template <typename _Tp, int cn>
	bool CBE_ConfigFile::modifyNode(void *ele, cv::Vec<_Tp, cn> &value, UserLevelType level)
	{
		checkLevel(ele, level);

		if (ele == NULL)
			return false;

		setAttr(ele, enumEvoBeVec);

		std::string value_str;
		std::stringstream ss;
		for (int i = 0; i != cn; ++i)
		{
			ss << value[i] << " ";
		}
		value_str = ss.str();
		value_str = value_str.substr(0, value_str.size() - 1);

		setText(ele, value_str);
		return true;
	}

	template <typename _Tp>
	bool CBE_ConfigFile::modifyNode(void *ele, cv::Scalar_<_Tp> &value, UserLevelType level)
	{
		checkLevel(ele, level);

		if (ele == NULL)
			return false;

		setAttr(ele, enumEvoBeScalar);

		std::string value_str;
		std::stringstream ss;
		for (int i = 0; i != 4; ++i)
		{
			ss << value[i] << " ";
		}
		value_str = ss.str();
		value_str = value_str.substr(0, value_str.size() - 1);

		setText(ele, value_str);
		return true;
	}

	template <typename _Tp>
	bool CBE_ConfigFile::checkType(XMLDataType node_type, _Tp &val)
	{
		switch (node_type)
		{
		case evo_be::enumEvoBeFloat:
			return typeid(_Tp) == typeid(float);
		case evo_be::enumEvoBeDouble:
			return typeid(_Tp) == typeid(double);
		case evo_be::enumEvoBeInt:
			return typeid(_Tp) == typeid(int);
		case evo_be::enumEvoBeString:
			return typeid(_Tp) == typeid(std::string);
		case evo_be::enumEvoBeMat:
			return typeid(_Tp) == typeid(cv::Mat);
		case evo_be::enumEvoBePoint:
			return (typeid(_Tp) == typeid(cv::Point)) || (typeid(_Tp) == typeid(cv::Point2f)) ||
				   (typeid(_Tp) == typeid(cv::Point2d)) || (typeid(_Tp) == typeid(cv::Point2i));
		case evo_be::enumEvoBePoint3:
			return (typeid(_Tp) == typeid(cv::Point3f)) ||
				   (typeid(_Tp) == typeid(cv::Point3d)) || (typeid(_Tp) == typeid(cv::Point3i));
		case evo_be::enumEvoBeSize:
			return typeid(_Tp) == typeid(cv::Size);
		case evo_be::enumEvoBeComplex:
			return (typeid(_Tp) == typeid(cv::Complexd)) || (typeid(_Tp) == typeid(cv::Complexf));
		case evo_be::enumEvoBeRect:
			return typeid(_Tp) == typeid(cv::Rect);
		case evo_be::enumEvoBeVec:
			return (typeid(_Tp) == typeid(cv::Vec2b)) || (typeid(_Tp) == typeid(cv::Vec2d)) ||
				   (typeid(_Tp) == typeid(cv::Vec2f)) || (typeid(_Tp) == typeid(cv::Vec2i)) ||
				   (typeid(_Tp) == typeid(cv::Vec2s)) || (typeid(_Tp) == typeid(cv::Vec2w)) ||
				   (typeid(_Tp) == typeid(cv::Vec3b)) || (typeid(_Tp) == typeid(cv::Vec3d)) ||
				   (typeid(_Tp) == typeid(cv::Vec3f)) || (typeid(_Tp) == typeid(cv::Vec3i)) ||
				   (typeid(_Tp) == typeid(cv::Vec3s)) ||
				   (typeid(_Tp) == typeid(cv::Vec3w)) || (typeid(_Tp) == typeid(cv::Vec4b)) ||
				   (typeid(_Tp) == typeid(cv::Vec4d)) || (typeid(_Tp) == typeid(cv::Vec4f)) ||
				   (typeid(_Tp) == typeid(cv::Vec4i)) || (typeid(_Tp) == typeid(cv::Vec4s)) ||
				   (typeid(_Tp) == typeid(cv::Vec4w));
		case evo_be::enumEvoBeScalar:
			return typeid(_Tp) == typeid(cv::Scalar);
		case evo_be::enumEvoBeRange:
			return typeid(_Tp) == typeid(cv::Range);
		case evo_be::enumEvoBeNon:
			return false;
		default:
			break;
		}
	}

	template <class _Tp>
	int CBE_ConfigFile::readFromNodeName(std::string node, _Tp &value, UserLevelType level)
	{
		std::deque<void *> nodes;
		nodes.push_back(getRoot());
		while (!nodes.empty())
		{
			void *curr_node = nodes.front();
			std::string curr_name = getNodeName(curr_node);

			if (curr_name == node)
			{
				auto type_str = getAttribute(curr_node, "node_type");
				XMLDataType type_evo = XMLDataType(std::stoi(type_str));

				if (!checkType(type_evo, value))
					return 2;

				read(curr_node, value, level);
				return 0;
			}

			nodes.pop_front();

			void *child = getFirstChild(curr_node);
			while (child)
			{
				nodes.push_back(child);
				child = getNextBrother(child);
			}
		}

		return 1;
	}

	class CBE_ConfigLoader
	{
	public:
		static bool load(std::string inputFile, BE_CalibData &calibData);
		static bool write(BE_CalibData calibData, CBE_ConfigFile *ptr_configFile);

		static bool fileCopy(string dst, string src);
	};

    class CBE_DataServer
    {
    public:
        static CBE_DataServer *create();
        static CBE_DataServer *create(string ipAddr);
        static CBE_DataServer *create(string folderPath, int rate);

        virtual ~CBE_DataServer();

        virtual bool pubsub_info_group() = 0;

        // for fake server
        virtual deque<BE_GeneralData>& getDataList_fake() = 0;
        virtual int getDataListNum() = 0;
        virtual bool getData_IncludeImage_listId(int listId, BE_GeneralData& bedata) = 0;
        virtual bool getData_IncludeImage_frameId(int frameId, BE_GeneralData& bedata) = 0;
        virtual bool getData_ExcludeImage_listId(int listId, BE_GeneralData& bedata) = 0;
        virtual bool getData_ExcludeImage_frameId(int frameId, BE_GeneralData& bedata) = 0;
        virtual bool pub_info_listId_fake(int listId) = 0;
        virtual bool pub_info_frameId_fake(int frameId) = 0;
        virtual bool pub_info_fake() = 0;
    };

	//class CBE_Service
	//{
	//public:
	//	CBE_Device_Service();
	//	virtual ~CBE_Device_Service();

	//	virtual void requestToTransFile() = 0;

	//	virtual bool sendFile(char* fileName) = 0;

	//	virtual bool recvFile(char* fileName) = 0;

	//	virtual unsigned int GetIpAddr() = 0;
	//};
} // namespace evo_be
#endif //_EVO_BE_COMMON_INTERNAL_H_