/*****************************************************************************
*  Eyevolution BionicEye series products SDK.\n
*  Copyright (C) 2018, Eyevolution, all rights reserved.
*
* @brief This is for general use of Eyevolution BionicEye series products.
* @author Openking <wang.kaifang@ilooktech.com>
* @date <2018/10/01>
* @version <1.0.1>
* @note This file is very important! Please Do Not Modify it!!!
*****************************************************************************/

#pragma once
#ifndef _EVO_BE_H_
#define _EVO_BE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <sys/time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>

#ifndef M_PI
#define M_PI (3.14159265358979323846f)
#endif

#define DEGToRAD(x) (x / 180.0f * M_PI)
#define RADToDEG(x) (x / M_PI * 180.0f)

#define MAXCAMERASInDEVICE  4
#define MAXIMUBUFFER        16

namespace evo_be
{
#ifdef _WIN32
    /**************************************************
     * @brief BionicEye device project path
     * @note Don't modify it!!!
     **************************************************/
	static const std::string Be_Project_Path = "D:/Evo_BionicEyes/";
#else
    /**************************************************
     * @brief BionicEye device project path
     **************************************************/
    static const std::string Be_Project_Path = "/usr/Evo_BionicEyes/";
#endif
    /**************************************************
     * @brief BionicEye device input&output files path
     **************************************************/
    static const std::string Be_Io_Path = Be_Project_Path + "io_path/";
    /**************************************************
     * @brief BionicEye device log file path
     **************************************************/
    static const std::string Be_Log_Path = Be_Project_Path + "log_path/";
    /**************************************************
     * @brief BionicEye device tmp file path
     **************************************************/
    static const std::string Be_Tmp_Path = Be_Project_Path + "tmp_path/";
    /**************************************************
     * @brief BionicEye device resource file path
     **************************************************/
    static const std::string Be_Resource_Path = Be_Project_Path + "resource/";

	/**************************************************
	 * @brief  Run Site type
	 **************************************************/
	enum BE_RunSite_Type
	{
		enumLocal = 0,
		enumRemote = 1,
	};

    /**************************************************
     * @brief BionicEye Device Type
     **************************************************/
	enum BE_Device_Type
    {
        enumUnknownDevice = 0,          /**< Unknown device */
        enumHumanEye_Serial = 1,        /**< BE device which is serial structure type */
        enumHumanEye_Parallel = 2,      /**< BE device which is parallel structure type */
        enumEagleEye = 3,               /**< Bionic eagle eye device */
        enumDevice_ThreeAxis = 4,       /**< Three axis platform device */
    };

	/**************************************************
	 * @brief Enumeration of BE motor type
	 **************************************************/
	enum MotorType
	{
		enumRightPitch = 0,     /**< Right Pitch Motor */
		enumRightRoll = 1,	    /**< Right Roll Motor */
		enumRightYaw = 2,	    /**< Right Yaw Motor */
		enumLeftPitch = 3,	    /**< Left Pitch Motor */
		enumLeftRoll = 4,	    /**< Left Roll Motor */
		enumLeftYaw = 5,	    /**< Left Yaw Motor */
		enumAllMotor = 6,	    /**< All Motor */
		enumNoMotor = 7,	    /**< No Motor */
	};

    /**************************************************
     * @brief Enumeration of BE neck motor typ
     **************************************************/
	enum MotorType_Neck
	{
		enumNeckPitch = 0,	  /**< Neck Pitch Motor */
		enumNeckRoll = 1,	  /**< Neck Roll Motor */
		enumNeckYaw = 2,	  /**< Neck Yaw Motor */
		enumNeckAllMotor = 3, /**< Neck All Motor */
		enumNeckNoMotor = 4,  /**< Neck No Motor */
	};

    /**************************************************
     * @brief Motor Name
     **************************************************/
	static std::string MotorName[6] = {
		"RightPitch",
		"RightRoll",
		"RightYaw",
		"LeftPitch",
		"LeftRoll",
		"LeftYaw"};

    /**************************************************
     * @brief Motor Axis Tags
     **************************************************/
	static std::string MotorTags[3] = {
		"Pitch",
		"Roll",
		"Yaw"};

    /**************************************************
     * @brief Eye Type Tags
     **************************************************/
	static std::string EyeTags[2] = {
		"Right",
		"Left"};

    /**************************************************
     * @brief Enumeration of IMU Data Type
     **************************************************/
	enum IMUDataType
	{
		enumAcc = 0,           /**< Acceleration */
		enumGyo = 1,           /**< Gyroscope */
		enumMag = 2,           /**< Magnetometer */
		enumEuler = 3,         /**< Euler angle */
		enumQuat = 4,          /**< Quaternion */
		enumLaser = 5,         /**< LaserData */
	};

    /**************************************************
     * @brief Enumeration of general axis type
     **************************************************/
	enum AxisType
	{
		enumPitch = 0,
		enumRoll = 1,
		enumYaw = 2,
		enumX = 0,
		enumY = 1,
		enumZ = 2,
		enumA = 0,
		enumB = 1,
		enumC = 2,
		enumD = 3,
	};

    /**************************************************
     * @brief Enumeration of image color type
     **************************************************/
	enum ImageColorType
	{
		enumMono = 0,
		enumColor = 1,
	};

    /**************************************************
     * @brief Enumeration of camera type
     **************************************************/
	enum BE_ImageSensorType
    {
        enumCamera_Wide = 0,            /**< Wide Angle camera */
        enumCamera_Mid = 1,             /**< Medium focal length camera */
        enumCamera_Tele = 2,            /**< Telephoto lens camera */
        enumThermalInfraredImager = 3,  /**< Thermal infrared imager */
    };

    /**************************************************
     * @brief Enumeration of camera index type
     **************************************************/
	enum CameraIndex
	{
		enumLeft = 0,
		enumRight = 1,
		enumBoth = 2,
	};

    /**************************************************
     * @brief Enumeration of GPS Data Type
     **************************************************/
	enum GpsDataType
	{
		enumLongitude = 0,
		enumLatitude = 1,
		enumGpsHeight = 2,
		enumGpsGMT = 3,
	};

    /**************************************************
     * @brief Enumeration of Standard Vergence type,
     **************************************************/
	enum SVType
	{
		enumGlobalSV = 0,         /**< Global sv */
		enumCentreSV = 1,         /**< Centre sv */
		enumAreaSV = 2,	          /**< Area sv */
	};

#pragma pack(1)
    /**************************************************
     * @brief Bionic Eye ip information structure
     **************************************************/
	struct BE_IpInfo
	{
        uchar deviceType;               /**< deviceType, @see BE_Device_Type */
		char deviceName[31];	        /**< bionic eye device name */
		unsigned int ipAddrInt;         /**< ip addr in uint (like 0x01010101) */
		char ipAddrStr[20];		        /**< ip addr in string (like "1.1.1.1") */
	};

	struct BE_ImageInfo
    {
	    ImageColorType colorType;   /**< Image color type, @see ImageColorType */
	    long imageLen;              /**< Image length (Bytes) */
	    int imageTranQualityLevel;  /**< image transfer quality level */
	    cv::Size imageSize;		    /**< Image size of this image */
	    cv::Size imageSize_Trans;   /**< Image Size of network transform image */
    };

    /**************************************************
     * Bionic Eye basic data structure, include all synchronized data
     * @brief Bionic eye general data information
     * @note Do not modify this struct!!!
     **************************************************/
	struct BE_GeneralData
	{
	    cv::Mat image[MAXCAMERASInDEVICE];              /**< Image(sidebyside) */

		uint32_t id;			                        /**< unique global id */
		uint32_t timeStamp;		                        /**< timestamp (100 us) */
		BE_IpInfo be_device_info;                       /**< BE device info, @see BE_IpInfo */

        bool imageFlag[MAXCAMERASInDEVICE];             /**< BE image exist flag */
		BE_ImageInfo imageInfo[MAXCAMERASInDEVICE];     /**< BE image info, @see BE_ImageInfo */

		float motorData[6];                             /**< Motor Coder Data */
		float imuData[MAXIMUBUFFER][4];                 /**< IMU Data */
		double gpsData[4];	                            /**< GPS Data(longitude, latitude, height, GMT) */

		bool isMovingFastly;                            /**< whether eyes move fastly when grab this data */

		BE_GeneralData()
        {
		    memset(&(this->id), 0, sizeof(BE_GeneralData) - sizeof(this->image));
        }

        inline BE_GeneralData operator=(BE_GeneralData &tmp) /**< @note shallow copy */
		{
		    memcpy(&(this->id), &(tmp.id), sizeof(BE_GeneralData) - sizeof(this->image));
			for(int i = 0; i< MAXCAMERASInDEVICE; i++)
			{
			    if (!tmp.image[i].empty() && tmp.imageFlag[i])
			        this->image[i] = tmp.image[i];
			}
			return *this;
		}

		inline BE_GeneralData &operator=(const BE_GeneralData &tmp)
		{
			if (this != &tmp)
			{
			    memcpy(&(this->id), &(tmp.id), sizeof(BE_GeneralData) - sizeof(this->image));
			    for(int i = 0; i< MAXCAMERASInDEVICE; i++)
			    {
			        if (!tmp.image[i].empty() && tmp.imageFlag[i])
			            this->image[i] = tmp.image[i];
			    }
			}
			return *this;
		}

		inline void copyTo(BE_GeneralData &tmp) const /**< @note deep copy, use this first, more efficient */
		{
		    memcpy(&(tmp.id), &(this->id), sizeof(BE_GeneralData) - sizeof(this->image));
			for(int i = 0; i< MAXCAMERASInDEVICE; i++)
			{
			    if (!this->image[i].empty() && this->imageFlag[i])
			        this->image[i].copyTo(tmp.image[i]);
			}
		}

		inline BE_GeneralData clone() const // deep copy
		{
		    BE_GeneralData tmp;
			copyTo(tmp);
			return tmp;
		}

        inline static void writeHead(std::string folderPath, std::ofstream *ofile)
        {
            // Get Data & Time
            struct timeval tv;
            struct tm *tm1;
            char buf[64];
            gettimeofday(&tv, NULL);
            tm1 = localtime(&tv.tv_sec);
            strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S", tm1);

            *ofile << buf << std::endl;
            *ofile << "id"<<","
                      << "timeStamp"<<","
                      << "be_device_info(deviceType)" <<","
                      << "be_device_info(deviceName)"<<","
                      << "be_device_info(ipAddrInt)"<<","
                      << "be_device_info(ipAddrStr)"<<",";
            for(int i = 0; i < MAXCAMERASInDEVICE; i++)
            {
                *ofile << "imageFlag_"<< i <<",";
            }
            for(int i = 0; i< MAXCAMERASInDEVICE; i++)
            {
                *ofile  << "colorType_" << i << ","
                        << "imageLen_" << i << ","
                        << "imageTranQualityLevel_" << i << ","
                        << "imageSize.width_" << i << ","
                        << "imageSize.height_" << i << ","
                        << "imageSize_Trans.width_" << i << ","
                        << "imageSize_Trans.height_" << i << ",";
            }

            for(int i = 0; i < 6; i++)
                *ofile <<"motorData("<<i<<"-"<<MotorName[i]<<")"<<",";

            for(int i = 0; i < MAXIMUBUFFER; i++)
            {
                *ofile  << "imuData_" << i << "_A,"
                        << "imuData_" << i << "_B,"
                        << "imuData_" << i << "_C,"
                        << "imuData_" << i << "_D,";
            }

            *ofile  << "gpsData_Longitude"<<","
                    << "gpsData_Latitude"<<","
                    << "gpsData_GpsHeight"<<","
                    << "gpsData_GpsGMT"<<","
                    << "isMovingFastly"<<","
                    << "description"<<std::endl;
            std::cout << "Start saving be data... <<folder path: "<< folderPath << ">>" << std::endl;
        }

        inline void writeData(std::string folderPath, std::ofstream *ofile, std::string description, bool writeImg = true, uchar* imcode[] = NULL)
        {
            *ofile  << id << ","
                    << timeStamp << ","
                    << (int)be_device_info.deviceType << ","
                    << be_device_info.deviceName << ","
                    << be_device_info.ipAddrInt << ","
                    << be_device_info.ipAddrStr << ",";

            for(int i = 0; i < MAXCAMERASInDEVICE; i++)
            {
                *ofile << imageFlag[i] << ",";
            }

            for(int i = 0; i < MAXCAMERASInDEVICE; i++)
            {
                *ofile << imageInfo[i].colorType << ","
                       << imageInfo[i].imageLen << ","
                       << imageInfo[i].imageTranQualityLevel << ","
                       << imageInfo[i].imageSize.width << ","
                       << imageInfo[i].imageSize.height << ","
                       << imageInfo[i].imageSize_Trans.width << ","
                       << imageInfo[i].imageSize_Trans.height << ",";
            }

            for(int i = 0; i < 6; i++)
                *ofile << motorData[i]<<",";
            for(int i = 0; i < MAXIMUBUFFER; i++)
                for(int j = 0; j < 4; j++)
                    *ofile << imuData[i][j]<<",";
            for(int i = 0; i < 4; i++)
                *ofile << gpsData[i]<<",";
            *ofile << isMovingFastly<<",";
            *ofile<<description<<std::endl;

            if(writeImg)
            {
                for(int i = 0; i < MAXCAMERASInDEVICE; i++)
                {
                    if(imageFlag[i])
                    {
                        if( NULL == imcode[i])
                        {
                            if(be_device_info.deviceType == enumEagleEye && i == enumThermalInfraredImager)
                            {
                                std::string picName = folderPath + "Cam_" + std::to_string(i) + "/" +std::to_string(id) + ".beInfra";
                                FILE *picFile = fopen(picName.c_str(), "w");
                                fwrite(image[i].data, 1, imageInfo[i].imageLen, picFile);
                                fclose(picFile);
                            }
                            else
                            {
                                std::string picName = folderPath + "Cam_" + std::to_string(i) + "/" +std::to_string(id) + ".bmp";
                                cv::imwrite(picName, image[i]);
                            }
                        }
                        else
                        {
                            std::string picName = folderPath + "Cam_" + std::to_string(i) + "/" +std::to_string(id) + ".jpg";
                            FILE *picFile = fopen(picName.c_str(), "w");
                            fwrite(imcode[i], 1, imageInfo[i].imageLen, picFile);
                            fclose(picFile);
                        }
                    }
                }
            }
        }

        inline bool readData(std::string _lineData, std::string folderPath, bool readImg = true, uchar* imcode[] = NULL)
        {
            std::stringstream ss(_lineData);
            std::string _sub;
            std::vector<std::string> subArray;

            while (getline(ss, _sub, ','))
                subArray.push_back(_sub);

            int tempI = 0;
            id = stoi(subArray[tempI++]);
            timeStamp = stoll(subArray[tempI++]);
            be_device_info.deviceType = (uchar)(stoi(subArray[tempI++]));
            strncpy(be_device_info.deviceName, subArray[tempI].c_str(), subArray[tempI].length() + 1);
            tempI++;
            be_device_info.ipAddrInt = stoll(subArray[tempI++]);
            strncpy(be_device_info.ipAddrStr, subArray[tempI].c_str(), subArray[tempI].length() + 1);
            tempI++;

            for(int i = 0; i < MAXCAMERASInDEVICE; i++)
            {
                imageFlag[i] = stoi(subArray[tempI++]);
            }

            for(int i = 0; i < MAXCAMERASInDEVICE; i++)
            {
                imageInfo[i].colorType = (ImageColorType)stoi(subArray[tempI++]);
                imageInfo[i].imageLen = stol(subArray[tempI++]);
                imageInfo[i].imageTranQualityLevel = stoi(subArray[tempI++]);
                imageInfo[i].imageSize.width = stoi(subArray[tempI++]);
                imageInfo[i].imageSize.height = stoi(subArray[tempI++]);
                imageInfo[i].imageSize_Trans.width = stoi(subArray[tempI++]);
                imageInfo[i].imageSize_Trans.height = stoi(subArray[tempI++]);
            }


            for(int i = 0; i < 6; i++)
                motorData[i] = stof(subArray[tempI++]);

            for(int i = 0; i < MAXIMUBUFFER; i++)
            {
                for(int j = 0; j < 4; j++)
                    imuData[i][j] = stof(subArray[tempI++]);
            }

            for(int i = 0; i < 4; i++)
                gpsData[i] = stod(subArray[tempI++]);
            isMovingFastly = stoi(subArray[tempI++]);

            if(readImg)
            {
                for(int i = 0; i < MAXCAMERASInDEVICE; i++)
                {
                    if(imageFlag[i])
                    {
                        if(imageInfo[i].imageTranQualityLevel == 100)
                        {
                            if(be_device_info.deviceType == enumEagleEye && i == enumThermalInfraredImager)
                            {
                                FILE *picFile;
                                std::string picName = folderPath + "Cam_" + std::to_string(i) + "/" +std::to_string(id) + ".beInfra";
                                if ((picFile = fopen(picName.c_str(), "r")) == NULL)
                                {
                                    std::cout << "Warning: No_"<< id << " beInfra file not exists! Will pass it..." << std::endl;
                                    return false;
                                }

                                fseek(picFile, 0L, SEEK_END);
                                int len = ftell(picFile);
                                rewind(picFile);

                                if(imageInfo[i].imageLen != len)
                                {
                                    std::cout<< "Warning: Reading image doesn't fit to record! Will pass it..."<<std::endl;
                                    return false;
                                }

                                if (fread(imcode[i], 1, len, picFile) != len)
                                {
                                    std::cout << "Warning: No_"<< id << " beInfra file read fail! Will pass it..." << std::endl;
                                    return false;
                                }
                                fclose(picFile);
                            }
                            else
                            {
                                std::string picName = folderPath + "Cam_" + std::to_string(i) + "/" +std::to_string(id) + ".bmp";
                                if(imageInfo[i].colorType == enumColor)
                                    image[i] = cv::imread(picName, cv::IMREAD_COLOR);
                                else
                                    image[i] = cv::imread(picName, cv::IMREAD_GRAYSCALE);
                                if(imageInfo[i].imageLen != image[i].total() * image[i].channels())
                                {
                                    std::cout<< "Warning: Reading image doesn't fit to record! Will pass it..."<<std::endl;
                                    return false;
                                }
                            }

                        } else
                        {
                            FILE *picFile;
                            std::string picName = folderPath + "Cam_" + std::to_string(i) + "/" +std::to_string(id) + ".jpg";
                            if ((picFile = fopen(picName.c_str(), "r")) == NULL)
                            {
                                std::cout << "Warning: No_"<< id << " jpg file not exists! Will pass it..." << std::endl;
                                return false;
                            }

                            fseek(picFile, 0L, SEEK_END);
                            int len = ftell(picFile);
                            rewind(picFile);

                            if(imageInfo[i].imageLen != len)
                            {
                                std::cout<< "Warning: Reading image doesn't fit to record! Will pass it..."<<std::endl;
                                return false;
                            }

                            if (fread(imcode[i], 1, len, picFile) != len)
                            {
                                std::cout << "Warning: No_\"<< bedata.id << \" jpg file read fail! Will pass it..." << std::endl;
                                return false;
                            }
                            fclose(picFile);
                        }
                    }

                }
            }
            return true;
        }

    };

#pragma pack()
} // namespace evo_be
#endif //_EVO_BE_H_