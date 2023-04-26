#pragma once
#ifndef _EVO_BE_IMAGEPROCESS_CORE_GPU_H_
#define _EVO_BE_IMAGEPROCESS_CORE_GPU_H_

#include "evo_be_Common.h"

//#define EIGEN_NO_DEBUG
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/ximgproc.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/cudaimgproc.hpp>
//#include <opencv2/core/eigen.hpp>

// #include <opencv2/core/core.hpp>
// //#include <opencv2/imgproc/imgproc.hpp>
// //#include <opencv2/xfeatures2d.hpp>
// #include <map>
#include <memory>
//#include "evo_be_ImageProcess_Core.h"

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
	/**
	 * @brief Class API for the well-known dense stereo-matching algorithm, semi-global matching (SGM) 
	 */
	class EVO_BE_IMAGEPROCESS_CORE_API CBE_StereoMatching
	{
	public:
		/**
		 * @brief Allocate stereo-matching object for a fixed image size
		 * @param  size             Image size of a single camera
		 * @return std::shared_ptr<CBE_StereroMatching> 
		 */
		static std::shared_ptr<CBE_StereoMatching> create(cv::Size size = cv::Size(960, 540));
		/**
		 * @brief Destroy the stereo-matching object
		 */
		virtual ~CBE_StereoMatching();
		/**
		 * @brief 
		 * @param  left             Rectified left image
		 * @param  right            Rectified right image
		 * @param  disp             Disparity image
		 */
		virtual void calc(const cv::Mat &srcLeft, const cv::Mat &srcRight, cv::Mat &dstDisp, cv::Mat &mask) = 0;
		/**
		 * @brief  Set the P1 parameter of SGM
		 * @param  P1_input         Recommended 10 to 80
		 */
		virtual void setP1(int P1_input) = 0;
		/**
		 * @brief  Set the P2 parameter of SGM
		 * @param  P2_input         Recommended 120 to 250
		 */
		virtual void setP2(int P2_input) = 0;
		/**
		 * @brief Set the uniqueness parameter of SGM
		 * @param  uniqueness_input Between 0.0 and 1.0, recommended 0.75 to 0.97
		 */
		virtual void setUniqueness(float uniqueness_input) = 0;
		/**
		 * @brief Set the width of input image
		 * @param  width_input      960x540 (or 1024x769) for speed consideration
		*/
		virtual void setWidth(int width_input) = 0;
		/**
		 * @brief Set the height of input image
		 * @param  height_input     960x540 (or 1024x768) for speed consideration
		 */
		virtual void setHeight(int height_input) = 0;
		/**
		 * @brief Reset P1, P2, uniqueness to default values
		 */
		virtual void reset() = 0;
		/**
		 * @brief Convert disparity map from float to uchar for display purpose
		 * @param  src32FC1         Input disparity map of float type
		 * @param  dst8UC1          Output disparity map of uchar type
		 */
		virtual void calc(const cv::Mat &srcLeft, const cv::Mat &srcRight,
						  const cv::Matx33d &KLeft, const cv::Matx33d &RRectLeft, const cv::Matx34d &PRectLeft,
						  const cv::Matx44d &Q, double scale,
						  cv::Mat &dstXYZ, cv::Mat &mask8UC1) = 0;

		virtual void disp32fTo8u(cv::Mat &src32FC1, cv::Mat &dst8UC1) = 0;
		/**
		 * @brief Convert disparity map (float) to depth map (float) in real-world units
		 * @param  src32FC1_disp    Input disparity map (float)
		 * @param  Q32FC1           Input disparity-to-depth matrix (see CBE_Calibrator::rectifyUndistortRemap)
		 * @param  dst32FC1_depth   Output depth map (float) in millimeter (mm)
		 */
		virtual void disp32fTo3D(cv::Mat &src32FC1_disp, cv::Matx44f Q32FC1, cv::Mat &dst32FC1_depth) = 0;
		/**
		 * @brief Convert disparity map (float) to depth map (float) and colored point cloud in real-world units
		 * @param  src32FC1_disp    Input disparity map (float)
		 * @param  src8UC3_left     Input undistored, stereo-rectified image of left eye
		 * @param  Q32FC1           Input disparity-to-depth matrix (see CBE_Calibrator::rectifyUndistortRemap)
		 * @param  dst32FC1_depth   Output depth map (float) in millimeter (mm)
		 * @param  dst32FC3_pcd_xyz Output coordinate matrix of the point cloud, each "pixel" of which has
		 * three float channels, i.e., the (x,y,z) coordinates in millimeter (mm)
		 * @param  dst8UC3_pcd_rgb  Output color (channel order = RGB, NOT BGR!) matrix of the point cloud;
		 * there is an entry-wise correspondence between dst32FC3_pcd_xyz and dst32FC3_pcd_rgb; 
		 */
		virtual void disp32fTo3D(cv::Mat &src32FC1_disp, cv::Mat &src8UC3_left, cv::Matx44f Q32FC1,
								 cv::Mat &dst32FC1_depth, cv::Mat &dst32FC3_pcd_xyz, cv::Mat &dst8UC3_pcd_rgb) = 0;


        virtual void virtualCalc(const cv::Mat &srcLeft, const cv::Mat &srcRight,const cv::Matx44d &Q,cv::Mat& disp, cv::Mat &dstXYZ, cv::Mat &mask8UC1) = 0;
	};

	class EVO_BE_IMAGEPROCESS_CORE_API CBE_BallTracking
	{
	public:
		enum FilterMethod
		{
			enumHSV = 0,
			enumBGR = 1
		};

		static std::shared_ptr<CBE_BallTracking> create();
		virtual ~CBE_BallTracking();
		virtual void detectBalls(cv::Mat srcImage, std::vector<cv::Vec3f> &circles, std::vector<int> &labels) = 0;
		virtual void setDp(float dp) = 0;
		virtual void setMinDist(float minDist) = 0;
		virtual void setCannyThrshold(int cannyThreshold) = 0;
		virtual void setAccThreshold(int accThreshold) = 0;
		virtual void setMinRadius(int minRadius) = 0;
		virtual void setMaxRadius(int maxRadius) = 0;
		virtual void setHSVRange(cv::Scalar lo, cv::Scalar hi) = 0;
		virtual void setColorMethod(FilterMethod method) = 0;
		virtual void drawCircles(cv::Mat srcImage, const std::vector<cv::Vec3f> &circles, const std::vector<int> &labels) = 0;
		virtual void updateBuffer(cv::Vec3f point) = 0;
		virtual cv::Vec3f estimateVelocity(cv::Mat test) = 0;
		virtual std::vector<cv::Vec3f> clusterCircles(const std::vector<cv::Vec3f> &circles, const std::vector<int> &labels) = 0;
	};

} // namespace evo_be

#endif //_EVO_BE_IMAGEPROCESS_CORE_H_
