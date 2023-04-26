#pragma once
#ifndef _EVO_BE_IMAGEPROCESS_CORE_H_
#define _EVO_BE_IMAGEPROCESS_CORE_H_

#include "evo_be_Common.h"

//#define EIGEN_NO_DEBUG
#include "Eigen/Eigen"
#include "Eigen/Geometry"

#include <opencv2/core/core.hpp>
#include <opencv2/core/eigen.hpp>

//#include <opencv2/imgproc/imgproc.hpp>
//#include <opencv2/xfeatures2d.hpp>
#include <map>
#include <memory>

#ifdef _WIN32
#ifdef EVO_BE_IMAGEPROCESS_CORE_EXPORTS
#define EVO_BE_IMAGEPROCESS_CORE_API __declspec(dllexport)
#else
#define EVO_BE_IMAGEPROCESS_CORE_API __declspec(dllimport)
#endif // EVO_BE_IMAGEPROCESS_CORE_EXPORTS
#else
#define EVO_BE_IMAGEPROCESS_CORE_API
#endif // _WIN32

namespace evo_be
{
	static const float WIDE_ANGLE = 61;	  // 相机水平视场角(根据标定结果2 * arctan(h / 2 / fx))
	static const float HEIGHT_ANGLE = 35; // 相机垂直视场角

	class EVO_BE_IMAGEPROCESS_CORE_API BE_Functions
	{
	public:
		/**
		 * @brief Convert a pair of stereo images into a blue-red anaglyph
		 * @param  srcImage         In-place modification of input image (left half = left camera, right half = right camera); left half stays untouched; right half becomes anaglyph
		 */
		static void anaglyphFromStereo(cv::Mat &srcImage);
		/**
		 * @brief Verify rectifies stereo images by checking horizontal alignment of epipolar lines; visualization is based on cv::imshow();
		 * @param  imgLeft          Input image of rectifed left image
		 * @param  imgRight         Input image of rectifed right image
		 * @param  scale            Rescale the resulting stereo image by a factor between 0.0 and 1.0
		 */
		static void checkStereoRectify(const cv::Mat &imgLeft, const cv::Mat &imgRight, float scale = 0.5);
		/**
		 * @brief Convert IMU data of a BE_ImageGeneralInfo struct into a rotation matrix in SO(3) ("d" for double-type)
		 * @param  data             Input data struct (see <evo_be.h>)
		 * @param  flipPitch        Flip sign of pitch angle of the input data
		 * @param  flipRoll         Flip sign of roll angle of the input data
		 * @param  flipYaw          Flip sign of yaw angle of the input data
		 * @return Eigen::Matrix3d
		 */
		static Eigen::Matrix3d BE_IMU2SO3d(const evo_be::BE_GeneralData &data, bool flipPitch = false, bool flipRoll = false, bool flipYaw = false);

		static cv::Mat TransTo3DLByL(cv::Mat &left, cv::Mat &right); // display image by linebyline
		static cv::Mat TransTo3DLByL(cv::Mat &image);				 // display image(left&right) by linebyline
	};

	struct DiffPRY
	{
		float PitchDiff, YawDiff, RollDiff;			 /**< Errors of frames(left and right) */
		float PitchDiffPre, YawDiffPre, RollDiffPre; /**< Errors of frames(before and after) */
		int width, height;
		cv::Mat DistInfo;
		DiffPRY(float dp, float dy, float dr) : PitchDiff(dp), YawDiff(dy), RollDiff(dr)
		{
			PitchDiffPre = 0;
			YawDiffPre = 0;
			RollDiffPre = 0;
		};
		DiffPRY(float dp, float dy, float dr, float pdp, float pdy, float pdr) : PitchDiff(dp), YawDiff(dy), RollDiff(pdr), PitchDiffPre(pdp), YawDiffPre(pdy), RollDiffPre(pdr){};
	};
	/**
	 * @brief 6D poses of active stereo vision\n
	 * Rotation 3x3 matrices:
	 * \f{eqnarray*}{
	 * \textrm{R\_rl}&=&\mathbf{\mathsf{R}}_{\mathrm{right}\leftarrow\mathrm{left}}\\
	 * \textrm{R\_r0}&=&\mathbf{\mathsf{R}}_{\mathrm{right}\leftarrow\mathrm{default}}\\
	 * \textrm{R\_l0}&=&\mathbf{\mathsf{R}}_{\mathrm{left}\leftarrow\mathrm{default}}
	 * \f}
	 * Translation 3x1 vectors:
	 * \f{eqnarray*}{
	 * \textrm{t\_rl}&=&\mathbf{t}_{\mathrm{right}\leftarrow\mathrm{left}}\\
	 * \textrm{t\_r0}&=&\mathbf{t}_{\mathrm{right}\leftarrow\mathrm{default}}\\
	 * \textrm{t\_l0}&=&\mathbf{t}_{\mathrm{left}\leftarrow\mathrm{default}}
	 * \f}
	 */
	struct RtStereo
	{
		cv::Matx33d R_rl, R_r0, R_l0;
		cv::Matx31d t_rl, t_r0, t_l0;
		cv::Matx33d J_l, J_r;
	};
	// class EVO_BE_IMAGEPROCESS_CORE_API CBE_Calibration
	// {
	// public:
	// 	static int countChessboards(cv::Mat img, std::vector<cv::Size> patternSizes);
	// };

	class EVO_BE_IMAGEPROCESS_CORE_API CBE_EagleEye
	{
	public:
		static std::shared_ptr<CBE_EagleEye> create(

			std::string path2json,
			std::string deviceName,
			double scale = 1.0,
			size_t zoomLevels = 10

		);
		virtual void pointAt(const cv::Point2f &srcPixel, float pitch_current, float yaw_current, const cv::Point2f &dstPixel, float &pitch_set, float &yaw_set,
							 size_t cam_id = 0,
							 bool undist = false) = 0;
		virtual void undist(cv::Mat src, cv::Mat dst, size_t id_cam, double scale = 1.0) = 0;
		virtual void redist(cv::Mat src_undist, cv::Mat dst_redist, size_t id_cam, double scale = 1.0) = 0;
		virtual std::vector<cv::Point2f> warpPoints(std::vector<cv::Point2f> srcPoints, size_t id_src, size_t id_dst) = 0;
		virtual void warpImage(cv::Mat srcImage, cv::Mat dstImage, size_t id_src, size_t id_dst) = 0;
		virtual void warpImageRaw2Raw(cv::Mat srcImage, cv::Mat dstImage, size_t id_src, size_t id_dst, int zoomLevel = 0, bool modifyDstImage = true) = 0;
		virtual void T2BGR(cv::Mat temp, cv::Mat &out32FC1, cv::Mat &out8UC1, float minTemp = -20, float maxTemp = 200) = 0;
		virtual void T2BGR(cv::Mat temp, size_t id_cam, cv::Mat &out32FC1, cv::Mat &out8UC1, float minTemp = -20, float maxTemp = 200) = 0;

		virtual void zoom(cv::Mat src0, cv::Mat src1, cv::Mat src2, cv::Mat dst, double s) = 0;

		virtual void autoAdjustExposure(evo_be::CBionicEyes *device, float exp_lo = 50, float exp_hi = 1500) = 0;

		// virtual void fromSerial2Parallel(float motorData_PRYPRY[6], float motorData_RGBRGB[6]) = 0;
		// virtual void fromParallel2Serial(float motorData_RGBRGB[6], float motorData_PRYPRY[6]) = 0;
		// virtual void getCameraIntrinsics(cv::Mat &K_R_, cv::Mat &D_R_, cv::Mat &K_L_, cv::Mat &D_L_, double scale = 1.0) = 0;
		// virtual void calibrateBias(evo_be::CBionicEyes *device, double angleRange = 20) = 0;
		// virtual void calibrateCamera2Hand(evo_be::CBionicEyes *device) = 0;
		// virtual void pointAt(cv::Point2d src, cv::Point2d dst, evo_be::CBionicEyes *device) = 0;
		// virtual void goInitPosition(evo_be::CBionicEyes *device) = 0;
		virtual double getZoom(size_t zoom_level) const = 0;

		virtual double getZoom(size_t zoom_level, int &zoomLevel_Cam2Begins_) const = 0;
		virtual ~CBE_EagleEye();
	};

	class EVO_BE_IMAGEPROCESS_CORE_API CBE_SPM
	{
	public:
		static std::shared_ptr<CBE_SPM> create();

		virtual void fromSerial2Parallel(float motorData_PRYPRY[6], float motorData_RGBRGB[6]) = 0;
		virtual void fromParallel2Serial(float motorData_RGBRGB[6], float motorData_PRYPRY[6]) = 0;
		virtual void getCameraIntrinsics(cv::Mat &K_R_, cv::Mat &D_R_, cv::Mat &K_L_, cv::Mat &D_L_, double scale = 1.0) = 0;
		virtual void calibrateBias(evo_be::CBionicEyes *device, double angleRange = 20) = 0;
		virtual void calibrateCamera2Hand(evo_be::CBionicEyes *device) = 0;
		virtual void pointAt(cv::Point2d src, cv::Point2d dst, evo_be::CBionicEyes *device) = 0;
		virtual void goInitPosition(evo_be::CBionicEyes *device) = 0;

		virtual ~CBE_SPM();
	};

	class EVO_BE_IMAGEPROCESS_CORE_API CBE_Gaze
	{
	public:
		static std::shared_ptr<CBE_Gaze> create();
		virtual ~CBE_Gaze();
		// virtual bool gaze(const cv::Mat &imageLeft, const cv::Mat &imageRight,
		// 				  cv::Mat KLeft, cv::Mat KRight, RtStereo Rt,
		// 				  cv::Rect &ROILeft, cv::Rect &ROIRight) const = 0;
		virtual bool gaze(const cv::Mat &imageUndistLeft, const cv::Mat &imageUndistRight,
						  RtStereo Rt, cv::Rect &ROILeft, cv::Rect &ROIRight) const = 0;
		virtual void setClickWindow_OPENCV(const std::string &name) = 0;
		virtual std::vector<cv::Point> getLastClick_OPENCV() const = 0;
		virtual void setKD(cv::Matx33d KLeft, cv::Matx33d KRight, cv::Matx<double, 5, 1> DLeft, cv::Matx<double, 5, 1> DRight) = 0;
	};

	class EVO_BE_IMAGEPROCESS_CORE_API CBE_Calibrator
	{
	public:
		static std::shared_ptr<CBE_Calibrator> create(uchar *ptr, cv::Size beImageSize = cv::Size(2048, 1536));

		virtual ~CBE_Calibrator();
		/**
		 * @brief For internal debugging
		 * @param  str              Input debug string
		 * @return uchar *
		 */
		virtual uchar *debug(const std::string str) = 0;
		/**
		 * @brief Index of left or right eye
		 * @param  eye              "Right" or "Left"
		 * @return int
		 */
		virtual int eyeId(std::string eye) const = 0;
		/**
		 * @brief Index of pitch, roll or yaw motor
		 * @param  motor            "Pitch", "Roll" or "Yaw"
		 * @return int
		 */
		virtual int motorId(std::string motor) const = 0;
		/**
		 * @brief Index of pitch, roll or yaw motor of left or right eye
		 * @param  eye              "Right" or "Left"
		 * @param  motor            "Pitch", "Roll" or "Yaw"
		 * @return int
		 */
		virtual int eyeMotorId(std::string eye, std::string motor) const = 0;
		/**
		 * @brief Rotation of default pose (left to right) when all motor values are zero
		 * @return cv::Matx33d
		 */
		virtual cv::Matx33d R_default() const = 0;
		/**
		 * @brief Translation of default pose (left to right) when all motor values are zero
		 * @return cv::Matx31d
		 */
		virtual cv::Matx31d t_default() const = 0;
		/**
		 * @brief Standard camera intrinsic matrix
		 * @param  index            Index of left or right eye
		 * @param  scale            Current image width (or height) / max image width (or height); assume aspect ratio remains unchanged;
		 * @return cv::Matx33d
		 */
		virtual cv::Matx33d K(int index, double scale = 1.0) const = 0;
		/**
		 * @brief Standard camera distortion coefficient array
		 * @param  index            Index of left or right eye
		 * @return cv::Matx<double, 5, 1>
		 */
		virtual cv::Matx<double, 5, 1> D(int index) const = 0;
		/**
		 * @brief Convert motor values to [R|t] poses
		 * @param  degree           Input array of 6 motor degrees
		 * @param  Rt               Output 6D poses (see struct RtStereo)
		 * @param  ang_vel_frame    Frame of angular velocity , 1 = base frame, 0 = local frame of current camera
		 */
		virtual void getRt(const float degree[], RtStereo &Rt, int ang_vel_frame = 0) const = 0;
		/**
		 * @brief Similar to void getRt(const float degree[], RtStereo &Rt) const
		 * @param  degree           Input array of 6 motor degrees
		 * @return RtStereo
		 */
		virtual RtStereo getRt(const float degree[], int ang_vel_frame = 0) const = 0;
		/**
		 * @brief
		 * @param  R      	        Input R of left-to-right [R|t]
		 * @param  t        		Input t of left-to-right [R|t]
		 * @param  scale            Current image width (or height) / max image width (or height)
		 * @param  srcImageLeft     Input image of left eye
		 * @param  srcImageRight    Input image of right eye
		 * @param  dstImageLeft     Output undistorted, stereo-rectified image of left eye
		 * @param  dstImageRight    Output undistorted, stereo-rectified image of right eye
		 * @param  RLeft            Output rotation matrix (see R1 of cv::stereoCalibrate)
		 * @param  PLeft            Output projection matrix (see P1 of cv::stereoCalibrate)
		 * @param  QOut             Output disparity-to-depth mapping matrix (see Q of cv::stereoCalibrate)
		 */
		virtual void rectifyUndistortRemap(cv::Matx33d R, cv::Matx31d t, double scale,
										   const cv::Mat &srcImageLeft, const cv::Mat &srcImageRight,
										   cv::Mat &dstImageLeft, cv::Mat &dstImageRight,
										   cv::Matx33d &RLeft, cv::Matx34d &PLeft, cv::Matx44d &QOut) const = 0;
		/**
		 * @brief Bring srcPixel to dstPixel after applying rotation Rout
		 * Consider an object point P which corresponds to pixel coordinates "dstPixel" BEFORE rotation.
		 * Move camera to a new pose Rout (see definition).
		 * Object point P should have pixel coordinates "srcPixel" AFTER rotation.
		 *
		 * @param srcRt    Current camera pose struct
		 * @param srcPixel Normally central pixel coordinates
		 * @param dstPixel Target pixel coordinates in current image
		 * @param scale    Current image width (or height) / max image width (or height)
		 * @param cam      Left or right eye
		 * @param a	       Some direction in current eye frame (such as +y axis (0,1,0))
		 * @param b        Some direction in base frame (ideally b, if transformed to the world frame, should be constant such as gravity)
		 * @return cv::Matx33d Rout is a rotation transformation FROM camera TO base (inverse of Rt.R_r0 or Rt.R_l0)
		 */
		virtual cv::Matx33d invKin_Pix2q(RtStereo srcRt, cv::Vec3d srcPixel, cv::Vec3d dstPixel, double scale, evo_be::CameraIndex cam,
										 cv::Vec3d a = cv::Vec3d(0, 0, 0), cv::Vec3d b = cv::Vec3d(0, 0, 0)) const = 0;
		/**
		 * @brief Compute encoder values such that they will produce camera pose srcR
		 *
		 * @param srcR Desired rotation transformation FROM camera TO base (inverse of Rt.R_r0 or Rt.R_l0)
		 * @param cam  Left or right eye
		 * @return cv::Vec3d Encoder values (pitch, roll, yaw)
		 */
		virtual cv::Vec3d invKin_Rot2q(cv::Matx33d srcR, evo_be::CameraIndex cam) const = 0;

		virtual RtStereo estimateRtFromChessboard(cv::Mat srcStereoImage, cv::Size chessboardSize, float squareSize) const = 0;
	};

	enum TrackType
	{
		enumCT = 0,	   // 压缩追踪
		enumKCF = 1,   // KCF追踪
		enumThres = 2, // 简单阈值法追踪（内部测试用）
		enumCSRT = 3,  // CSRT追踪
	};

	// 追踪函数接口函数
	// 输入图像必须为彩色图
	class EVO_BE_IMAGEPROCESS_CORE_API CBionicEyesTrackInterface
	{
	public:
		static CBionicEyesTrackInterface *create(TrackType type);
		virtual ~CBionicEyesTrackInterface();

		// 初始化帧
		virtual void init(cv::Mat &_frame, cv::Rect &_objectBox) = 0;
		// 追踪并返回结果
		virtual cv::Rect processFrame(cv::Mat &_frame, cv::Rect &_objectBox) = 0;
		// 跟踪置信度（目前仅对kcf有效）
		virtual float trackConfidence() = 0;
	};

	// 图像方法增稳核心类
	class EVO_BE_IMAGEPROCESS_CORE_API CBionicEyesKSIInterface
	{
	public:
		static CBionicEyesKSIInterface *create();
		virtual ~CBionicEyesKSIInterface();
		virtual cv::Mat stabImage(cv::Mat &inputImage) = 0;
		virtual cv::Mat getStabParm(cv::Mat &inputImage) = 0;
		virtual cv::Mat fitStabImage(cv::Mat &inputImage, cv::Mat &stabPara) = 0;
		virtual void resetAlgo() = 0;
	};

	/**  SV function class
	 * @brief Standard Vergence type, mainly include global sv and certre sv
	 */
	class EVO_BE_IMAGEPROCESS_CORE_API CBE_StandardVergence
	{
	public:
		static CBE_StandardVergence *create(cv::Size size);

		virtual ~CBE_StandardVergence();

		// SV开关，默认开启
		virtual void SwitchSV(bool ONOFF) = 0;

		// 返回sv的开关状态
		virtual bool isSVOpened() = 0;

		// DP开关，默认关闭（Display）
		virtual void SwitchDP(bool ONOFF) = 0;

		// 特征点距离计算函数开关（默认关闭）
		virtual void SwitchFPDC(bool ONOFF) = 0;

		// 初始化roi区域(以左图为基准)
		virtual void InitROIArea(cv::Rect rect, cv::Size size = cv::Size(1920, 1080)) = 0;

		// 标准辐辏 主函数
		virtual DiffPRY SV_main(cv::Mat &inImage, SVType type) = 0;
		virtual DiffPRY SV_main(cv::Mat &LImage, cv::Mat &RImage, SVType type) = 0;
	};

	class EVO_BE_IMAGEPROCESS_CORE_API CBE_SparseMatching
	{
	public:
		static std::shared_ptr<CBE_SparseMatching> create();
		virtual ~CBE_SparseMatching();
		virtual void getKeypoints(cv::Mat gray, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors) = 0;
		virtual void getMatches(
			std::vector<cv::DMatch> &matches,
			std::vector<cv::KeyPoint> kpL, std::vector<cv::KeyPoint> kpR,
			cv::Mat dpL, cv::Mat dpR, double reprojectionThreshold) = 0;
		virtual void setMaxFeatures(int maxFeatures) = 0;
		virtual void setNLevels(int nLevels) = 0;
		virtual void setScaleFactor(double scaleFactor) = 0;
		virtual void setFastThreshold(int initialFastThreshold, int minimumFastThreshold) = 0;
	};
} // namespace evo_be

#endif //_EVO_BE_IMAGEPROCESS_CORE_H_
