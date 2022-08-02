/*
 * @Description: 
 * @Version: 1.0
 * @Autor: chen zhan
 * @Date: 2022-07-19 16:53:24
 * @LastEditors: chen zhan
 * @LastEditTime: 2022-07-20 21:14:54
 */

#ifndef _Detector_num
#define _Detector_num
#include <opencv2/opencv.hpp>  
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core_c.h>
#include <iostream> 
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <fmt/core.h>
#include <fmt/color.h>
#include <string>

using namespace std;
using namespace cv;

inline bool cmp(cv::RotatedRect &roi_1,cv::RotatedRect &roi_2){
    if (abs(roi_1.center.y-roi_2.center.y)>20)
    {
      return roi_1.center.y<roi_2.center.y;
    }
    else{
      return roi_1.center.x<roi_2.center.x;
	}
}
 

 class Detector_num
 {
 private:
    /* data */
    cv::Mat _image;
    vector<cv::RotatedRect>_roi_images_rect;
    vector<cv::Mat>_roi_images_sort;
    string green_flag=fmt::format(fg(fmt::color::green)| fmt::emphasis::bold ,"Detector_number");
    string red_flag=fmt::format(fg(fmt::color::red)| fmt::emphasis::bold ,"Detector_number");
    bool sort_roi_image();
 public:

   vector<unsigned char> detect_response;

    Detector_num(const cv::Mat &image);

    cv::Mat roi_img_finder();

    int response_num(vector<unsigned char> &send_number_detect_response);

    cv::Mat RotatedRect_adjust(cv::RotatedRect &rect );

    void template_matching(cv::Mat &matching_img,vector<int> &response);

    int getPixelSum(Mat& image);

    int svmpredict(Mat &image);

    ~Detector_num();

    
 };
 

#endif // !Detector_num