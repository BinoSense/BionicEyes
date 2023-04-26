#pragma once
#ifndef _EVO_BE_UTILITIES_H_
#define _EVO_BE_UTILITIES_H_

#include <evo_be.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "Eigen/Core"

#ifdef _WIN32
#ifdef EVO_BE_UTILITIES_EXPORTS
#define EVO_BE_UTILITIES_API __declspec(dllexport)
#else
#define EVO_BE_UTILITIES_API __declspec(dllimport)
#endif // EVO_BE_IMAGEPROCESS_CORE_EXPORTS
#else
#define EVO_BE_UTILITIES_API
#endif // _WIN32

namespace evo_be
{
    /**
     * @brief Static class API for commonly used utility functions
     */
    class EVO_BE_UTILITIES_API BE_Utilities
    {
    public:
        /**
         * @brief Extract the substring between two delimiters.
         * @param  s                Input string
         * @param  start_delim      First delimiter 
         * @param  stop_delim       Second delimiter
         * @return std::string 
         */
        static std::string getStrBetweenTwoStr(const std::string &s, const std::string &start_delim, const std::string &stop_delim);
        /**
         * @brief Load files of certain extension from path. Files must be zero-padded (e.g. "000.bmp" and "001.bmp")
         * @param  path             Input directory such as "/xxx/yyy"
         * @param  ext              Input extension, currently limited to ".bmp" and ".png"
         * @param  imgFiles_sort    Output file names  {".png"}
         */
        static void loadFiles(std::string path, std::string ext, std::vector<std::string> &imgFiles_sort);
        static void SplitString(std::vector<std::string> &tokens, const std::string &str, const std::string &delimiters = " ", bool trim_empty_str = true);
        static std::string GetProgramOptionAsString(int argc, char **argv, const std::string &option, const std::string &default_value = "");
        static int GetProgramOptionAsInt(int argc, char **argv, const std::string &option, const int default_value = 0);
        static double GetProgramOptionAsDouble(int argc, char **argv, const std::string &option, const double default_value = 0.0);
        static Eigen::VectorXd GetProgramOptionAsEigenVectorXd(int argc, char **argv, const std::string &option, const Eigen::VectorXd default_value = Eigen::VectorXd::Zero(0));
        static bool ProgramOptionExists(int argc, char **argv, const std::string &option);
        static bool ProgramOptionExistsAny(int argc, char **argv, const std::vector<std::string> &options);
        /**
         * @brief Convert degree to radian
         * @param  deg              Input degree
         * @return float 
         */
        static float deg2rad(float deg);
        /**
         * @brief Convert degree to radian
         * @param  deg              Input degree
         * @return double 
         */
        static double deg2rad(double deg);
        /**
         * @brief Compute inverse of a R|t pair on SE(3) group
         * \f[
         * \mathbf{\mathsf{T}}=\begin{bmatrix}
         * \mathbf{\mathsf{R}}^{\phantom{\top}} & \mathbf{t}\\
         * \mathbf{0}^\top & 1        
         * \end{bmatrix}\Rightarrow
         * \mathbf{\mathsf{T}}^{-1}=\begin{bmatrix}
         * \mathbf{\mathsf{R}}^{\top} & -\mathbf{\mathsf{R}}^{\top}\mathbf{t}\\
         * \mathbf{0}^\top & 1        
         * \end{bmatrix}
         * \f]
         * @param  R                Input rotation matrix of a homogenous transform
         * @param  t                Input translation vector of a homogenous transform
         * @param  ROut             Ouput rotation matrix of the inverse of this homogenous transform
         * @param  tOut             Ouput translation vector of the inverse of this homogenous transform
         */
        static void RtInv(const cv::Mat &R, const cv::Mat &t, cv::Mat &ROut, cv::Mat &tOut);
        /**
         * @brief Right-composition two R|t pairs. Let  
         * \f[
         * \mathbf{\mathsf{T}}_a=\begin{bmatrix}
         * \mathbf{\mathsf{R}}^{\phantom{\top}}_a & \mathbf{t}_a\\
         * \mathbf{0}^\top & 1        
         * \end{bmatrix},\quad
         * \mathbf{\mathsf{T}}_b=\begin{bmatrix}
         * \mathbf{\mathsf{R}}^{\phantom{\top}}_b & \mathbf{t}_b\\
         * \mathbf{0}^\top & 1        
         * \end{bmatrix}~.
         * \f]
         * This function performs the following,
         * \f[
         * \mathbf{\mathsf{T}}_a=\mathbf{\mathsf{T}}_a\mathbf{\mathsf{T}}_b
         * \f]
         * @param  Ra               Input rotation matrix of the first homogenous transform; will be modified in-place after right-composition
         * @param  ta               Input translation vector of the first homogenous transform; will be modified in-place after right-composition
         * @param  Rb               Input rotation matrix of the second homogenous transform
         * @param  tb               Input translation vector of the second homogenous transform
         */
        static void RtRightTimes(cv::Mat &Ra, cv::Mat &ta, const cv::Mat &Rb, const cv::Mat &tb);
        static void RtInv(const cv::Matx33d &R, const cv::Matx31d &t, cv::Matx33d &Rout, cv::Matx31d &tout);
        static void RtRightTimes(cv::Matx33d &Ra, cv::Matx31d &ta, const cv::Matx33d &Rb, const cv::Matx31d &tb);
        /**
         * @brief Convert a R|t pair into a homogenous matrix:	    
         * \f[
         * \mathbf{\mathsf{T}}=\begin{bmatrix}
         * \mathbf{\mathsf{R}}^{\phantom{\top}} & \mathbf{t}\\
         * \mathbf{0}^\top & 1        
         * \end{bmatrix}
         * \f]        
         * @param  R                Input rotation 3-by-3 matrix in SO(3) group
         * @param  t                Input translation 3-by-1 vector in T(3) group
         * @param  T                Output homogenous 4-by-4 matrix in SE(3) group
         */
        static void Rt2T(const Eigen::Matrix3f &R, const Eigen::Vector3f &t, Eigen::Matrix4f &T);
        /**
         * @brief Rt2T(R,t,T) when R is the identity matrix
         * @param  t                My Param doc
         * @param  T                My Param doc
         */
        static void Rt2T(const Eigen::Vector3f &t, Eigen::Matrix4f &T);
        /**
         * @brief Rt2T(R,t,T) when t is a zero vector
         * @param  R                My Param doc
         * @param  T                My Param doc
         */
        static void Rt2T(const Eigen::Matrix3f &R, Eigen::Matrix4f &T);
        static void Rt2T(const Eigen::Matrix3d &R, const Eigen::Vector3d &t, Eigen::Matrix4d &T);
        static void Rt2T(const Eigen::Vector3d &t, Eigen::Matrix4d &T);
        static void Rt2T(const Eigen::Matrix3d &R, Eigen::Matrix4d &T);


        /** @brief 虚平行视面立体矫正方法 v1.0
         *  @note 此方法使用前需去畸变，此部分不在方法内实现
         *  @param K1: 左相机内参
         *  @param K2: 右相机内参
         *  @param imageSize: 原始图像大小
         *  @param matR： 左右相机外参旋转矩阵
         *  @param matT:  左右相机外参平移矩阵
         *  @param R1:  左相机图像旋转矩阵
         *  @param R2:  右相机图像旋转矩阵
         *  @param P1: 虚平行视面左图的投影矩阵
         *  @param P2: 虚平行视面右图的投影矩阵
         *  @param Q: 视差转深度
         *  @param flags: CALIB_ZERO_DISPARITY,左右图像的主点相同
         *  @param alpha: 有效区域尺度，默认-1取虚拟焦距中点，0到1为参数，0-只选择有效区域no blcak area 1-所有像素全部保留
            @param newImageSize: 新的图像大小
            @param validPixROI1：左图有效区域矩形
            @param validPixROI2: 右图有效区域矩形
            @param left_image_center: 虚平行视面左图像的像素主点
            @param right_image_center: 虚平行视面右图像的像素主点
        */
        static void virtualStereoRectify(const cv::Mat &K1, const cv::Mat &K2,
                                  const cv::Mat &D1, const cv::Mat &D2,
                                  const cv::Size& imageSize, const cv::Mat &matR, const cv::Mat &matT,
                                  cv::Mat& R1, cv::Mat& R2,
                                  cv::Mat &P1, cv::Mat &P2,
                                  cv::Mat &Q, int flags,
                                  double alpha, cv::Size newImageSize,
                                  cv::Rect& validPixROI1, cv::Rect& validPixROI2,
                                  cv::Point2d* left_image_center=0,cv::Point2d* right_image_center=0);
    };
} // namespace evo_be

#endif